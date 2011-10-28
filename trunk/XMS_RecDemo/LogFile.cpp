#include "stdafx.h"
#include "LogFile.h"

PLOG_FILE_CFG	g_pLogCfg;		//日志配置变量
extern int cfg_LogOn;
extern int cfg_LogLevel;
int InitLogCfg( void )
{
	char pBuf[LOGFILE_PATH_LEN_MAX]	={0};		//存放路径的变量      
	struct tm *nowtime;
	time_t long_time;

	if((g_pLogCfg = (PLOG_FILE_CFG)malloc(sizeof(LOG_FILE_CFG))) != NULL)
	{
		memset( g_pLogCfg,0, sizeof(LOG_FILE_CFG) );	//空间分配成功
	}else
	{
		return -1;
	}

#ifdef WIN32
	GetCurrentDirectory(LOGFILE_PATH_LEN_MAX,pBuf);	//Windows获取程序的当前目录
	strcat(pBuf,"\\");
#else
	memcpy( pBuf,"./",sizeof("./") );				//Linux获取程序的当前目录
#endif	
	memcpy(g_pLogCfg->s8LogPath, pBuf, sizeof(pBuf));

	g_pLogCfg->s32LogSize = 10*1024*1024;		//20M,日志最大大小
	g_pLogCfg->s32LogLevel = cfg_LogLevel;		//日志级别，输出所有日志
	g_pLogCfg->s32LogOn = cfg_LogOn;					//是否开启日志

#ifdef WIN32
	InitializeCriticalSection(&g_pLogCfg->m_cs);
#else
	pthread_mutex_init(&g_pLogCfg->m_cs, NULL);
#endif

	time( &long_time );                
	nowtime = localtime( &long_time );			//获取当前时间

#ifdef WIN32
	sprintf( g_pLogCfg->s8LogFileName, "%sLogFile-%04d%02d%02d.txt",
		g_pLogCfg->s8LogPath, nowtime->tm_year+ 1900,nowtime->tm_mon + 1,nowtime->tm_mday); 
#else
	sprintf( g_pLogCfg->s8LogFileName, "%s/LogFile-%04d%02d%02d.txt",
		g_pLogCfg->s8LogPath, nowtime->tm_year+ 1900,nowtime->tm_mon + 1,nowtime->tm_mday); 
#endif

	if( g_pLogCfg->s32LogOn==1 && g_pLogCfg->s8LogFileName!=NULL )     //如果开启了日志，创建日志文件
	{
		g_pLogCfg->pLogFileHandle = fopen( g_pLogCfg->s8LogFileName, "a+" );
		if (g_pLogCfg->pLogFileHandle == NULL)
		{
			return -1;
		}
	}
	return 1;
}

int  WriteLog( LEVEL_TYPE logLevel, char *pMsgStr )
{
	long	logMsgLen = 0;		//每条日志消息长度
	long	s32Pos	  = 0;		//日志位置
	struct tm *nowtime;
	time_t long_time;

	if ( g_pLogCfg == NULL )	//日志变量未初始化
	{
		return -1;
	}
	// 等级控制，例如选择日志级别LEVEL_ERROR，那么WARN信息不会输出日志 
	if( logLevel < g_pLogCfg->s32LogLevel || g_pLogCfg->pLogFileHandle == NULL) //是否记日志
	{
		return -1;
	}
#ifdef WIN32
	SYSTEMTIME	sysTime;			
	GetLocalTime( &sysTime );			//获得系统当前时间
	sprintf( g_pLogCfg->s8LogBuf, "%04d%02d%02d-%02d:%02d:%02d:%03d [%5s] " ,
		sysTime.wYear, sysTime.wMonth, sysTime.wDay, 
		sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds,LOG_ERROR[logLevel] );
#else
	struct timeval t;
       gettimeofday(&t, NULL);
	time( &long_time );                
	nowtime = localtime( &long_time );	//获得系统当前时间
	sprintf( g_pLogCfg->s8LogBuf, "%04d%02d%02d-%02d:%02d:%02d:%03ld [%5s] ",
		nowtime->tm_year+ 1900,nowtime->tm_mon + 1,nowtime->tm_mday,
		nowtime->tm_hour,nowtime->tm_min,nowtime->tm_sec,(t.tv_usec/1000), LOG_ERROR[logLevel]);
#endif

	if( pMsgStr != NULL )
	{	
		logMsgLen = strlen( pMsgStr ) + 32;		//32为时间等信息的长度
		if (logMsgLen > LOG_BUF_LEN_MAX)		//日志消息太长了
		{
			return -1;
		}
		strcat( g_pLogCfg->s8LogBuf, pMsgStr );
	}
	strcat( g_pLogCfg->s8LogBuf, "\n" );

#ifdef WIN32
	EnterCriticalSection(&g_pLogCfg->m_cs);
#else
	pthread_mutex_lock(&g_pLogCfg->m_cs);
#endif
	
	fseek( g_pLogCfg->pLogFileHandle, 0, SEEK_END );
	s32Pos = ftell( g_pLogCfg->pLogFileHandle );
	if( s32Pos >= g_pLogCfg->s32LogSize ) //超过日志文件大小
	{
		fclose( g_pLogCfg->pLogFileHandle );//关闭文件句柄
		memset( g_pLogCfg->s8LogFileName, 0, sizeof( g_pLogCfg->s8LogFileName ) );			
		time( &long_time );                
		nowtime = localtime( &long_time );			//获取当前时间
		
#ifdef WIN32
		sprintf( g_pLogCfg->s8LogFileName, "%sLogFile-%04d%02d%02d.txt",
			g_pLogCfg->s8LogPath, nowtime->tm_year+ 1900,nowtime->tm_mon + 1,nowtime->tm_mday); 
#else
		sprintf( g_pLogCfg->s8LogFileName, "%s/LogFile-%04d%02d%02d.txt",
			g_pLogCfg->s8LogPath, nowtime->tm_year+ 1900,nowtime->tm_mon + 1,nowtime->tm_mday); 
#endif
		
		if( g_pLogCfg->s32LogOn==1 && g_pLogCfg->s8LogFileName!=NULL )     //如果开启了日志，创建日志文件
		{
			g_pLogCfg->pLogFileHandle = fopen( g_pLogCfg->s8LogFileName, "a+" );
			if (g_pLogCfg->pLogFileHandle == NULL)
			{
				return -1;
			}
		}
	}
	
	fprintf( g_pLogCfg->pLogFileHandle, "%s", g_pLogCfg->s8LogBuf );
	fflush( g_pLogCfg->pLogFileHandle );
#ifdef WIN32	
	LeaveCriticalSection(&g_pLogCfg->m_cs);
#else	
	pthread_mutex_unlock(&g_pLogCfg->m_cs);
#endif		
	return 0;
}

