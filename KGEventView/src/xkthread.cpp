/****************************************************************************/
/* Copyright (c) 2004,chenwenyue											*/
/* All rights reserved.													    */
/* File name:	xkthread.cpp												*/
/* Author: 	chenwenyue													    */
/* Version: 	1.0															*/
/* Date:		2006.10.30													*/
/* Description:	线程相关类实现文件											*/
/* History:																	*/
/*	   Date:																*/
/*	   Author:																*/
/*	   Modification:														*/
/****************************************************************************/
#include "xkthread.h"
#include <errno.h>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// CXKThread::CXKThread ------------------------------------------------------------ 
CXKThread::CXKThread() 
: m_thread(InvalidHandle)
, m_joinable(XK_FALSE)
{
}

// CXKThread::~CXKThread ------------------------------------------------------------ 
CXKThread::~CXKThread() 
{
	if( XK_TRUE == m_joinable )
	{
#if defined(XK_WORK_ON_WIN_PLATFORM)
		CLOSE_HANDLE(m_thread);
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
		pthread_detach(m_thread); 
#endif
	}
}
// CXKThread::Exit ------------------------------------------------------------ 
void CXKThread::Exit()
{ 
#if defined(XK_WORK_ON_WIN_PLATFORM)
    EXIT_THREAD;
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
    pthread_exit(0);
#endif
}
// CXKThread::Cancel ------------------------------------------------------------ 	
void CXKThread::Cancel()
{ 
#if defined(XK_WORK_ON_WIN_PLATFORM)
    Sleep(0);
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
    pthread_testcancel();
#endif 
}
// CXKThread::Self ------------------------------------------------------------ 	
THREAD_HANDLE CXKThread::Self()
{ 
	return m_thread;
}
// CXKThread::Create ------------------------------------------------------------ 	
int CXKThread::Create(
		Handler       Function
		, void *Data							 /* = XK_NULL */
		, const Tbyte		   & StackSize       /* = 0*/
		, const Tbool          & CancelEnable    /* = XK_FALSE   // UNUSED */
		, const Tbool          & CancelAsync     /* = XK_FALSE   // UNUSED */
		)
{
#if defined(XK_WORK_ON_WIN_PLATFORM)
	m_thread	= CREATE_THREAD(StackSize,Function, Data);
	
	if ( m_thread == CREATE_THREAD_FAILED )
	{
		m_thread = InvalidHandle;
		return CREATE_THREAD_ERROR;
	}
	m_joinable	= XK_TRUE;

	return 0;
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	M_Create().Lock();
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	if ( StackSize )
		pthread_attr_setstacksize(&attr,StackSize);
	
	Instance I(0,Function, Data, CancelEnable,CancelAsync);
	
	int R = pthread_create((pthread_t *)m_thread,&attr,ThreadMainHandler,(void *)&I);
	pthread_attr_destroy(&attr);
	
	if ( !R ) 
	{
		S_Create().Wait();
		m_joinable	= XK_TRUE;
	}
	else if ( m_thread )
	{
		*m_thread = InvalidHandle;
	}
	
	M_Create().Unlock();

	return errno;
#endif
}
// CXKThread::Create ------------------------------------------------------------ 	
int CXKThread::Create(
		const Tbyte		  & StackSize       /* = 0 */
		, const Tbool          & CancelEnable    /* = XK_FALSE   // UNUSED */
		, const Tbool          & CancelAsync     /* = XK_FALSE   // UNUSED */
		)
{
#if defined(XK_WORK_ON_WIN_PLATFORM)

	m_thread	= CREATE_THREAD(StackSize,ThreadMainHandler, this);
	
	if ( m_thread == CREATE_THREAD_FAILED )
	{
		m_thread = InvalidHandle;
		return CREATE_THREAD_ERROR;
	}
	m_joinable	= XK_TRUE;

	return 0;
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	M_Create().Lock();
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	
	if ( StackSize )
		pthread_attr_setstacksize(&attr,StackSize);
	
	Instance I(const_cast<CXKThread *>(this), XK_NULL, XK_NULL, CancelEnable,CancelAsync);
	
	int R = pthread_create((pthread_t *)m_thread,&attr,ThreadMainHandler,(void *)&I);
	pthread_attr_destroy(&attr);
	
	if ( !R ) 
	{
		S_Create().Wait();
		m_joinable	= XK_TRUE;
	}
	else if ( m_thread )
	{
		*m_thread = InvalidHandle;
	}
	
	M_Create().Unlock();
	return errno;
#endif
}
// CXKThread::Join ------------------------------------------------------------ 	
int CXKThread::Join( )
{
	XK_ASSERT( m_joinable == XK_TRUE );
#if defined(XK_WORK_ON_WIN_PLATFORM)
	DWORD R = WaitForSingleObject(m_thread,INFINITE);
	
	if ( (R == WAIT_OBJECT_0) || (R == WAIT_ABANDONED) )
	{
		CLOSE_HANDLE(m_thread);
		m_joinable	= XK_FALSE;
		return 0;
	}
	
	if ( R == WAIT_TIMEOUT ) return EAGAIN;

	return EINVAL;
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	m_joinable	= XK_FALSE;
	return pthread_join(m_thread,0);
#endif
}
// CXKThread::Kill ------------------------------------------------------------ 	
int CXKThread::Kill( )
{ 
#if defined(XK_WORK_ON_WIN_PLATFORM)
	return TerminateThread((HANDLE)m_thread,0) ? 0 : EINVAL;
#elif defined(XK_WORK_ON_LINUX_PLATFORM)	
    return pthread_cancel(m_thread);
#endif 
}
// CXKThread::Detach ------------------------------------------------------------ 	
int CXKThread::Detach( )
{
	m_joinable	= XK_FALSE;
#if defined(XK_WORK_ON_WIN_PLATFORM)
	return (CLOSE_HANDLE(m_thread)?0:EINVAL);
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	return pthread_detach(m_thread); 
#endif  
}
#if defined(XK_WORK_ON_WIN_PLATFORM)
// CXKThread::ThreadMainHandler ------------------------------------------------------------ 	
THREAD_RET_T THREAD_CALL CXKThread::ThreadMainHandler( CXKThread *Param )
{
	Param->Run();
	Exit();
	THREAD_RETURN(0);
}
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
THREAD_RET_T CXKThread::ThreadMainHandler( Instance *Param )
{
	Instance  I(*Param);
	S_Create().Post();
	
	if ( I.Flags & 1 /*CancelEnable*/ )
	{
		pthread_setcancelstate(PTHREAD_CANCEL_ENABLE,XK_NULL);
		
		if ( I.Flags & 2 /*CancelAsync*/ )
			pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,XK_NULL);
		else
			pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED,XK_NULL);
	}
	else
	{
		pthread_setcancelstate(PTHREAD_CANCEL_DISABLE,XK_NULL);
	}
	
	if ( I.Owner )
		I.Owner->Run();
	else
		I.pFN(I.pData);
	
	return 0;
}
#endif
// CXKMutex::CXKMutex ------------------------------------------------------------
CXKMutex::CXKMutex()
{
#if defined(XK_WORK_ON_WIN_PLATFORM)
	InitializeCriticalSection(&m_c);
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	pthread_mutexattr_t attr;
	pthread_mutexattr_init(&attr);
	pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE);
	pthread_mutex_init(&M,&attr);
	pthread_mutexattr_destroy(&attr);
#endif
}
// CXKMutex::~CXKMutex ------------------------------------------------------------
CXKMutex::~CXKMutex()
{
#if defined(XK_WORK_ON_WIN_PLATFORM)
	DeleteCriticalSection(&m_c);
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	pthread_mutex_unlock(&M); 
	pthread_mutex_destroy(&M); 
#endif
}
// CXKMutex::Lock ------------------------------------------------------------
int CXKMutex::Lock() const
 { 
#if defined(XK_WORK_ON_WIN_PLATFORM)
	EnterCriticalSection(&m_c); 
	return 0; 
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	return pthread_mutex_lock(&M); 
#endif
}
#if defined(XK_WORK_ON_WIN_PLATFORM)
// CXKMutex::Lock_Try ------------------------------------------------------------
#if(_WIN32_WINNT >= 0x0400)
int CXKMutex::Lock_Try() const
{
	return (TryEnterCriticalSection(&m_c)?0:EBUSY); 
}
#endif
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
int CXKMutex::Lock_Try() const
{ 
	return pthread_mutex_trylock(&M); 
}
#endif
// CXKMutex::Unlock ------------------------------------------------------------
int CXKMutex::Unlock() const
{
#if defined(XK_WORK_ON_WIN_PLATFORM)
	LeaveCriticalSection(&m_c); 
	return 0;
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	return pthread_mutex_unlock(&M); 
#endif 
}
// CXKSemaphore::CXKSemaphore ------------------------------------------------------------
CXKSemaphore::CXKSemaphore( int init )
{
#if defined(XK_WORK_ON_WIN_PLATFORM)
	m_hHandle = CreateSemaphore(0,init,SEM_VALUE_MAX,0);
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	sem_init(&m_s,0,init); 
#endif 
}
// CXKSemaphore::~CXKSemaphore ------------------------------------------------------------
CXKSemaphore::~CXKSemaphore()
{
#if defined(XK_WORK_ON_WIN_PLATFORM)
	CloseHandle(m_hHandle); 
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	sem_destroy(&m_s);
#endif 
}
// CXKSemaphore::Wait ------------------------------------------------------------
void CXKSemaphore::Wait() const
{
#if defined(XK_WORK_ON_WIN_PLATFORM)
	WaitForSingleObject((HANDLE)m_hHandle,INFINITE); 
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	sem_wait((sem_t *)&m_s);
#endif 
}
// CXKSemaphore::Wait_Try ------------------------------------------------------------
int CXKSemaphore::Wait_Try() const
{
#if defined(XK_WORK_ON_WIN_PLATFORM)
	return ((WaitForSingleObject((HANDLE)m_hHandle,INFINITE)==WAIT_OBJECT_0)?0:EAGAIN);
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	return (sem_trywait((sem_t *)&m_s)?errno:0);
#endif  
}
// CXKSemaphore::Post ------------------------------------------------------------
int CXKSemaphore::Post() const
{
#if defined(XK_WORK_ON_WIN_PLATFORM)
	return (ReleaseSemaphore((HANDLE)m_hHandle,1,0)?0:ERANGE); 
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	return (sem_post((sem_t *)&m_s)?errno:0); 
#endif 
}
// CXKSemaphore::Value ------------------------------------------------------------
long CXKSemaphore::Value() const
{ 
	long V = -1;
#if defined(XK_WORK_ON_WIN_PLATFORM)
	ReleaseSemaphore((HANDLE)m_hHandle,0,&V); 
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	sem_getvalue((sem_t *)&m_s,&V); return V; 
#endif
	return V; 
}
// CXKSemaphore::Reset ------------------------------------------------------------
void CXKSemaphore::Reset( int init )
{
#if defined(XK_WORK_ON_WIN_PLATFORM)
	CloseHandle(m_hHandle);
    	m_hHandle = CreateSemaphore(0,init,SEM_VALUE_MAX,0);
#elif defined(XK_WORK_ON_LINUX_PLATFORM)
	sem_destroy(&m_s); 
	sem_init(&m_s,0,init); );
#endif
}