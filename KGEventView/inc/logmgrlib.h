#ifndef __LOGMGRLIB__
#define	__LOGMGRLIB__

#ifdef __cplusplus
extern "C" {
#endif

#define __XK_DLL_LIB_NAME		   "DJITPLogMgr.lib"
#if defined __XK_DLL_IMPL__
	#define __XK_DRAW_API _declspec(dllexport)
#else
#if defined __NO_DLL_IMPL__
	#define __XK_DRAW_API 
#else
	#define __XK_DRAW_API _declspec(dllimport)
	#pragma comment( lib, __XK_DLL_LIB_NAME )
#endif
#endif 

/* 输出流级别(0~256) */
#define XK_LEVEL_PRINT					3			// 打印信息
#define XK_LEVEL_LOG					2			// 日志信息
#define XK_LEVEL_EXCEPTION				1			// 异常信息

int __XK_DRAW_API  InitLogMgr(int nModuleType, int nModuleID);
void __XK_DRAW_API ExitLogMgr();

void __XK_DRAW_API xkPrint(int nLevel, const char *format,...);
	/* 输出二进制消息 */
void __XK_DRAW_API xkPrintb(int nLevel, const char *pTitle, int nTitleLen, const char *pData ,int len);

#ifdef __cplusplus
}
#endif

#endif
