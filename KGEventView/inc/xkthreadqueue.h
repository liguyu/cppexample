#ifndef __XK_THREAD_QUEUE_H
#define __XK_THREAD_QUEUE_H

#include <queue>
#include "xkdef.h"
#include "xkthread.h"

class CXKThreadQueue;
/*------------------------------------------------------------------------
CXKWorkItemBase	

this is the basic WorkItem that the Work Queue Use its interface
This class should be inherited and these virtual abstract functions 
implemented.

  DoWork()

  virtual abstract function is the function that is called when the
  work item turn has came to be poped out of the queue and be processed.


  Abort ()

  This function is called, when the Destroy function is called, for each of the WorkItems
  That are left in the queue.
  
	
------------------------------------------------------------------------*/
class CXKWorkItemBase
{
public:
	CXKWorkItemBase(){}
	virtual ~CXKWorkItemBase(){}
public:
	virtual void   DoWork(void* pThreadContext)   = 0;
	virtual void   Abort () = 0;
	friend CXKThreadQueue;
};
/*
class CXKThreadExt : public CXKThread
{
public:
	CXKThreadExt(CXKThreadQueue	*tThreadQueue, void *pParam);
	virtual ~CXKThreadExt();
public:
	virtual void Run();
private:
	CXKThreadQueue	*m_tThreadQueue;
	void			*m_pParam;
};*/

typedef std::queue<CXKWorkItemBase*>			WaitItemQueue,*PWaitItemQueue;
typedef std::list<CXKWorkItemBase*>				WorkItemQueue,*PWorkItemQueue;
/*------------------------------------------------------------------------
CXKThreadQueue  

  This is the WorkOueue class also known as thread pool,
  the basic idea of this class is creating thread that are waiting on a queue
  of work item when the queue is inserted with items the threads wake up and 
  perform the requered work and go to sleep again.
------------------------------------------------------------------------*/
class  CXKThreadQueue 
{
	friend class CXKThreadExt;
public:
	CXKThreadQueue();
	virtual ~CXKThreadQueue();
public:
	enum SincType{ SINC_INVALID, SINC_ABORT, SINC_ADDITEM };
	SincType GetSincType() { return m_SincFlag; }
	Tbool Create(const unsigned int nNumberOfThreads, void **pThreadDataArray  = XK_NULL);
	
	Tbool InsertWaitItem(CXKWorkItemBase* pWorkItem);
	void RemoveAll();
	void Destroy();
private:
	static THREAD_RET_T ThreadFunc( void* pParam );
	
	inline	void Wait() { m_tSincObject.Wait(); }
	
	CXKWorkItemBase* RemoveWaitItem();
	Tbool RemoveWorkItem(CXKWorkItemBase* pWorkItem);
	Tbool InsertWorkItem(CXKWorkItemBase* pWorkItem);
	
	CXKThread				**m_phThreads;
	unsigned int			m_nNumberOfThreads;
	
	CXKSemaphore			m_tSincObject;
	SincType				m_SincFlag;
	
	CXKMutex							m_tWaitMutex;
	WaitItemQueue						m_tWaitItemQueue;

	CXKMutex							m_tWorkMutex;
	WorkItemQueue						m_tWorkItemQueue;
};

#endif // __XK_THREAD_QUEUE_H
