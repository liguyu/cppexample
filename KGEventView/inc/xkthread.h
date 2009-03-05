/****************************************************************************/
/* File name:	xkthread.h													*/
/* Author: 	chenwenyue													    */
/* Version: 	1.0															*/
/* Date:		2006.10.30													*/
/* Description:	线程相关类定义头文件										*/
/* History:																	*/
/*	   Date:																*/
/*	   Author:																*/
/*	   Modification:														*/
/****************************************************************************/
#if !defined(__XK_THREAD_H)
#define __XK_THREAD_H

#include "xkdef.h"

#if defined(XK_WORK_ON_WIN_PLATFORM)
#if !defined(_WINDOWS_)
  // 去除windows.h中默认包含的winsock.h
  #define WIN32_LEAN_AND_MEAN
  #include <windows.h>
  // WIN32 Excludes
  #ifdef WIN32_LEAN_AND_MEAN
  #  define VC_EXTRALEAN
  #  define WIN32_LEAN_AND_MEAN
  #  define _PRSHT_H_
  #  define  NOGDICAPMASKS     // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
  #  define  NOVIRTUALKEYCODES // VK_*
  #  define  NOWINMESSAGES     // WM_*, EM_*, LB_*, CB_*
  #  define  NOWINSTYLES       // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
  #  define  NOSYSMETRICS      // SM_*
  #  define  NOMENUS           // MF_*
  #  define  NOICONS           // IDI_*
  #  define  NOKEYSTATES       // MK_*
  #  define  NOSYSCOMMANDS     // SC_*
  #  define  NORASTEROPS       // Binary and Tertiary raster ops
  #  define  NOSHOWWINDOW      // SW_*
  #  define  OEMRESOURCE       // OEM Resource values
  #  define  NOATOM            // Atom Manager routines
  #  define  NOCLIPBOARD       // Clipboard routines
  #  define  NOCOLOR           // Screen colors
  #  define  NOCTLMGR          // Control and Dialog routines
  #  define  NODRAWTEXT        // DrawText() and DT_*
  #  define  NOGDI             // All GDI defines and routines
  #  define  NOKERNEL          // All KERNEL defines and routines
  #  define  NOUSER            // All USER defines and routines
  #  define  NONLS             // All NLS defines and routines
  #  define  NOMB              // MB_* and MessageBox()
  #  define  NOMEMMGR          // GMEM_*, LMEM_*, GHND, LHND, associated routines
  #  define  NOMETAFILE        // typedef METAFILEPICT
  #  define  NOMINMAX          // Macros min(a,b) and max(a,b)
  #  define  NOMSG             // typedef MSG and associated routines
  #  define  NOOPENFILE        // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
  #  define  NOSCROLL          // SB_* and scrolling routines
  #  define  NOSERVICE         // All Service Controller routines, SERVICE_ equates, etc.
  #  define  NOSOUND           // Sound driver routines
  #  define  NOTEXTMETRIC      // typedef TEXTMETRIC and associated routines
  #  define  NOWH              // SetWindowsHook and WH_*
  #  define  NOWINOFFSETS      // GWL_*, GCL_*, associated routines
  #  define  NOCOMM            // COMM driver routines
  #  define  NOKANJI           // Kanji support stuff.
  #  define  NOHELP            // Help engine interface.
  #  define  NOPROFILER        // Profiler interface.
  #  define  NODEFERWINDOWPOS  // DeferWindowPos routines
  #  define  NOMCX             // Modem Configuration Extensions
  #endif // WIN32_LEAN_AND_MEAN
  //
#endif
#include <process.h>
#ifdef _XK_USE_BEGINTHREAD
	#define THREAD_CALL                __cdecl	
	#define THREAD_HANDLE              uintptr_t
	#define THREAD_RET_T               void
	#define CREATE_THREAD_FAILED       (-1L)
	#define CREATE_THREAD_ERROR        (errno)
	#define CREATE_THREAD(_S,_F,_P)    ((Handle)_beginthread((void (__cdecl *)(void *))_F,_S,(void *)_P))
	#define EXIT_THREAD                _endthread()
	#define CLOSE_HANDLE(x)            1
	#define THREAD_RETURN(x)           return
#else
	#define THREAD_CALL                WINAPI
	#define THREAD_HANDLE              HANDLE
	#define THREAD_RET_T               unsigned
	#define CREATE_THREAD_FAILED       (0L)
	#define CREATE_THREAD_ERROR        (errno)
	#define CREATE_THREAD(_S,_F,_P)    ((Handle)_beginthreadex(0,_S,(unsigned (__stdcall *)(void *))_F,(void *)_P,0,0))
	#define EXIT_THREAD                _endthreadex(0)
	#define CLOSE_HANDLE(x)            CloseHandle(x)
	#define THREAD_RETURN(x)           return(x)
#endif
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
#include <pthread.h>
#define THREAD_RET_T               void *
#define THREAD_RETURN(x)           return(x)
#endif

#define InvalidHandle 0	
#define SEM_VALUE_MAX ((int) ((~0u) >> 1))

class CXKMutex
{
public:
	CXKMutex();
	virtual ~CXKMutex();
	
	int Lock() const;
#if defined(XK_WORK_ON_WIN_PLATFORM)
#if(_WIN32_WINNT >= 0x0400)
	int Lock_Try() const;
#endif
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	int Lock_Try() const;
#endif	
	int Unlock() const;
private:
	void operator=(CXKMutex &M);
	CXKMutex( const CXKMutex &M );

	// 临界区对象
#if defined(XK_WORK_ON_WIN_PLATFORM)
	mutable CRITICAL_SECTION m_c;
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	mutable pthread_mutex_t M;
#endif
};

class CXKSemaphore
{
public:
	CXKSemaphore( int init = 0 );
	
	virtual ~CXKSemaphore();
	
	void Wait() const;
	
	int Wait_Try() const;
	
	int Post() const;
	
	long Value() const;
	
	void Reset( int init = 0 );
private:
#if defined(XK_WORK_ON_WIN_PLATFORM)
	HANDLE m_hHandle;
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	sem_t m_s;
#endif	
};

class CXKThread;
/////////////////////////////////////////////////////////////////////
//  Explicit specialization, no thread parameters
//
class CXKThread
{
private:
    typedef struct Instance;
public:
#if defined(XK_WORK_ON_WIN_PLATFORM)
    typedef THREAD_HANDLE Handle;
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
    typedef pthread_t Handle;
#endif
    typedef THREAD_RET_T ( *Handler)( void *param );
	
	CXKThread();
	virtual ~CXKThread();
protected:
    /* 强行退出线程 */	
    static void Exit();
    /* 释放当前cpu时间片 */
    void Cancel();

    virtual void Run(){}	
public:
    /* 创建线程 */
    int Create(
		Handler       Function,
		void *pData								= XK_NULL,
		const Tbyte  & StackSize       = 0,
		const Tbool          & CancelEnable		= XK_FALSE,
		const Tbool          & CancelAsync		= XK_FALSE
		);
    /* 创建线程 */
    int Create(
		const Tbyte  & StackSize       = 0,
		const Tbool          & CancelEnable		= XK_FALSE,
		const Tbool          & CancelAsync		= XK_FALSE
		);
    /* 阻塞调用线程　
	  例如：当前线程A中调用B.Join(),B挂起A的运行，直到B运行完成退出。*/
    int Join( );
    /* 强行中止某一个线程 
		不释放线程相关资源*/
    int Kill( );
    /* 释放线程句柄 */
    int Detach( );
    /* 返回线程句柄 */
    Handle Self();

private:
	Handle	m_thread;
	Tbool	m_joinable;
#if defined(XK_WORK_ON_WIN_PLATFORM)
    /* 线程内部入口函数 */
    static THREAD_RET_T THREAD_CALL ThreadMainHandler( CXKThread *Param );
#elif defined(XK_WORK_ON_LINUX_PLATFORM)	
    struct Instance
    {
		Instance( CXKThread *const &O, const Handler &pH = 0, void* data = XK_NULL, const Tbool &CE=XK_FALSE, const Tbool &CA=XK_FALSE )
			: pFN(pH), pData(data), Owner(O), Flags(0) { if ( CE ) Flags|=1; if ( CA ) Flags|=2; }
		
		CXKThread					*Owner;
		Handler						pFN;
		void						*pData;
		unsigned char               Flags;
    };
    static const CXKMutex &M_Create()      { static CXKMutex M; return M; }
    static const CXKSemaphore &S_Create()  { static CXKSemaphore S; return S; }
    /* 线程内部入口函数 */
    static THREAD_RET_T ThreadMainHandler( Instance *Param );
#endif
};

#endif // !defined(__XK_THREAD_H)