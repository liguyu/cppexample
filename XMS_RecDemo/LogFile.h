#ifndef __LOG_FILE_H__
#define __LOG_FILE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#define	LOG_BUF_LEN_MAX			1024				//每条日志消息的最大长度
#define LOGFILE_PATH_LEN_MAX	256					//日志文件路径的最大长度

// 日志报警等级
typedef enum
{
	LEVEL_DEBUG = 0,	//调试,级别最低,最多日志消息,在系统实际运行过程中一般都是不输出的
	LEVEL_INFO  = 1,	//信息,反馈系统的当前状态给最终用户的
	LEVEL_WARN  = 2,	//警告,这时进行一些修复性的工作,系统能够恢复,可以继续运行下去
	LEVEL_ERROR = 3,    //错误,导致一个无法修复的错误
	LEVEL_NONE  = 4		//无,  不打印日志消息. 
		
} LEVEL_TYPE;
// 不同日志等级对应的字符串
static const char LOG_ERROR[][8] = {
    "DEBUG",
	"INFO ",
	"WARN ",
	"ERROR",
	"NONE "
};
// 日志配置项
typedef struct LOG_FILE_CFG
{
	FILE	*pLogFileHandle;						//日志句柄
	char	s8LogBuf[LOG_BUF_LEN_MAX];				//日志缓存区
	char	s8LogFileName[LOGFILE_PATH_LEN_MAX];    //日志文件去路径名称
	char	s8LogPath[LOGFILE_PATH_LEN_MAX];		//日志文件路经
	int		s32LogSize;								//日志大小
	int		s32LogOn;								//1:开启日志;0:不打印日志
	int		s32LogLevel;							//日志级别

#ifdef WIN32
	CRITICAL_SECTION m_cs;							//函数的重入控制
#else

#endif

}LOG_FILE_CFG, *PLOG_FILE_CFG;

// 对外函数声明
int InitLogCfg( void );
int WriteLog( LEVEL_TYPE logLevel, char *pMsgStr );

#endif

