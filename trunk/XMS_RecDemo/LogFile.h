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

#define	LOG_BUF_LEN_MAX			1024				//ÿ����־��Ϣ����󳤶�
#define LOGFILE_PATH_LEN_MAX	256					//��־�ļ�·������󳤶�

// ��־�����ȼ�
typedef enum
{
	LEVEL_DEBUG = 0,	//����,�������,�����־��Ϣ,��ϵͳʵ�����й�����һ�㶼�ǲ������
	LEVEL_INFO  = 1,	//��Ϣ,����ϵͳ�ĵ�ǰ״̬�������û���
	LEVEL_WARN  = 2,	//����,��ʱ����һЩ�޸��ԵĹ���,ϵͳ�ܹ��ָ�,���Լ���������ȥ
	LEVEL_ERROR = 3,    //����,����һ���޷��޸��Ĵ���
	LEVEL_NONE  = 4		//��,  ����ӡ��־��Ϣ. 
		
} LEVEL_TYPE;
// ��ͬ��־�ȼ���Ӧ���ַ���
static const char LOG_ERROR[][8] = {
    "DEBUG",
	"INFO ",
	"WARN ",
	"ERROR",
	"NONE "
};
// ��־������
typedef struct LOG_FILE_CFG
{
	FILE	*pLogFileHandle;						//��־���
	char	s8LogBuf[LOG_BUF_LEN_MAX];				//��־������
	char	s8LogFileName[LOGFILE_PATH_LEN_MAX];    //��־�ļ�ȥ·������
	char	s8LogPath[LOGFILE_PATH_LEN_MAX];		//��־�ļ�·��
	int		s32LogSize;								//��־��С
	int		s32LogOn;								//1:������־;0:����ӡ��־
	int		s32LogLevel;							//��־����

#ifdef WIN32
	CRITICAL_SECTION m_cs;							//�������������
#else

#endif

}LOG_FILE_CFG, *PLOG_FILE_CFG;

// ���⺯������
int InitLogCfg( void );
int WriteLog( LEVEL_TYPE logLevel, char *pMsgStr );

#endif

