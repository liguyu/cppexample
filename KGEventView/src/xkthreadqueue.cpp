#include "xkthreadqueue.h"

typedef struct tagXKThreadContext
{
	CXKThreadQueue* pWorkQueue;
	void*       pThreadData;
} XK_THREAD_CONTEXT,*PXK_THREAD_CONTEXT;
/*
CXKThreadExt::CXKThreadExt(CXKThreadQueue *tThreadQueue, void *pParam)
: m_tThreadQueue(tThreadQueue)
, m_pParam(pParam)
{
}

CXKThreadExt::~CXKThreadExt()
{
}

void CXKThreadExt::Run()
{
	if( XK_NULL == m_tThreadQueue ) return;
	XK_ASSERT( m_tThreadQueue != XK_NULL );
	m_tThreadQueue->ThreadFunc(m_pParam);
}
*/
CXKThreadQueue::CXKThreadQueue()
: m_nNumberOfThreads(0)
, m_phThreads(XK_NULL)
, m_SincFlag(SINC_INVALID)
{

}

CXKThreadQueue::~CXKThreadQueue()
{
	Destroy();
}
/*------------------------------------------------------------------------
Create

  Initialize the work queue mechanism
  
	const unsigned int  nNumberOfThreads      - Number of threads to create in the work queue, 
	void*         *ThreadData                 - This is data for each thread that will be passed
	to the DoWork function for each WorkItem, this  can be used to give each thread a work area
	to increase epitomization of allocations and free.
	
------------------------------------------------------------------------*/
Tbool CXKThreadQueue::Create(const unsigned int  nNumberOfThreads, 
							 void*         *ThreadData      /*=XK_NULL*/)
{	
	if( m_SincFlag != SINC_INVALID ) return XK_FALSE;
	//allocate thread handle array
	m_phThreads = new CXKThread*[nNumberOfThreads];
	
	if(m_phThreads == XK_NULL)
	{    
		return XK_FALSE;
	}
	
	m_nNumberOfThreads = nNumberOfThreads;
	
	PXK_THREAD_CONTEXT pThreadsContext ;
	
	int nRet	= 0;
	//create all threads
	for(int i = 0 ; i < nNumberOfThreads ; i++ )
	{  
		//init each thread context
		pThreadsContext = new XK_THREAD_CONTEXT;
		pThreadsContext->pWorkQueue  = this;
		pThreadsContext->pThreadData = ThreadData == XK_NULL? XK_NULL : ThreadData[i];
		
		//create thread
		m_phThreads[i]	= new CXKThread;//CXKThreadExt(this, (void*)pThreadsContext);
		if( XK_NULL != m_phThreads[i])
			nRet	= m_phThreads[i]->Create( CXKThreadQueue::ThreadFunc, (void *)pThreadsContext );
			//nRet	= m_phThreads[i]->Create( );
				
		if( nRet )
		{  
			delete pThreadsContext;
			m_nNumberOfThreads = i;
			Destroy();
			return XK_FALSE;
		}
	}
	
	return XK_TRUE;
}


/*------------------------------------------------------------------------
InsertWorkItem

  Insert the work Item to the queue for precessing
------------------------------------------------------------------------*/
Tbool CXKThreadQueue::InsertWaitItem(CXKWorkItemBase* pWorkItem)
{
//	xkPrint(XK_LEVEL_PRINT, "CXKThreadQueue::InsertWaitItem() in\n");
	XK_ASSERT(pWorkItem != NULL);
	//lock
	m_tWaitMutex.Lock();
	if( m_SincFlag	== SINC_ABORT )
	{
		m_tWaitMutex.Unlock();
		return XK_FALSE;
	}
	//insert the item to queue
	m_tWaitItemQueue.push(pWorkItem);
	m_SincFlag		= SINC_ADDITEM;
	//unlock
	m_tWaitMutex.Unlock();
	
	//signal semaphore 
	if ( m_tSincObject.Post() ) 
	{ 
		XK_ASSERT( XK_FALSE );
		return XK_FALSE;     
	} 
//	xkPrint(XK_LEVEL_PRINT, "CXKThreadQueue::InsertWaitItem() out\n");
	return XK_TRUE;	
}

/*------------------------------------------------------------------------
RemoveWorkItem()

  extract the net item from the queue and return it
------------------------------------------------------------------------*/
CXKWorkItemBase*  CXKThreadQueue::RemoveWaitItem()
{
	
	CXKWorkItemBase* pWorkItem;
	
	//lock
	m_tWaitMutex.Lock();
    
	//remove the item to queue 
	if( !m_tWaitItemQueue.empty() )
	{
		pWorkItem = m_tWaitItemQueue.front();
		m_tWaitItemQueue.pop();
	}
	
	//release lock
	m_tWaitMutex.Unlock();
	
	XK_ASSERT(pWorkItem != NULL);
	
	return pWorkItem;
}

Tbool CXKThreadQueue::InsertWorkItem(CXKWorkItemBase* pWorkItem)
{
	
	XK_ASSERT(pWorkItem != NULL);
	//lock
	m_tWorkMutex.Lock();
	//insert the item to queue
	m_tWorkItemQueue.push_back(pWorkItem);
	//unlock
	m_tWorkMutex.Unlock();

	return XK_TRUE;	
}

Tbool  CXKThreadQueue::RemoveWorkItem(CXKWorkItemBase* pWorkItem)
{
	//lock
	m_tWorkMutex.Lock();
	WorkItemQueue::iterator	it;
	if( !m_tWorkItemQueue.empty() )
	{
		for( it = m_tWorkItemQueue.begin();it != m_tWorkItemQueue.end();it++ )
		{
			XK_ASSERT( (*it) != XK_NULL );
			if( (*it) == pWorkItem )
			{
//				xkPrint(XK_LEVEL_PRINT, "if( (*it) == pWorkItem )\n");
//				delete	(*it);
				m_tWorkItemQueue.erase(it);
				break;
			}
		}
	}
	//release lock
	m_tWorkMutex.Unlock();

//	xkPrint(XK_LEVEL_PRINT, "CXKThreadQueue::RemoveWorkItem()\n");

	return XK_TRUE;
}
/*------------------------------------------------------------------------
ThreadFunc

  This is the thread function that is waiting on the 
  arrival of work item to process them, or to an abort event
  in case the work queue is destroied
------------------------------------------------------------------------*/
THREAD_RET_T CXKThreadQueue::ThreadFunc( void*  pParam )
{
	
	
	PXK_THREAD_CONTEXT       pThreadContext		=  (PXK_THREAD_CONTEXT)pParam;
	
	CXKWorkItemBase*         pWorkItem			= NULL;

	CXKThreadQueue*          pWorkQueue			= pThreadContext->pWorkQueue;
	
	void*                 pThreadData			= pThreadContext->pThreadData;
	
	for(;;)
	{
		//wait for one of the two
		// 1. Abort Event -
		pWorkQueue->Wait();
	//	xkPrint(XK_LEVEL_PRINT, "pWorkQueue->Wait() %d\n", pWorkQueue->GetSincType());
		switch(pWorkQueue->GetSincType())
		{ 
		case SINC_ABORT:
			delete pThreadContext;
//			pWorkQueue->m_tSincQuit.Post();
			return 0;
		case SINC_ADDITEM:
			//take the alerted first work item from queue
			pWorkItem	= pWorkQueue->RemoveWaitItem();
			//auto_ptr<CXKWorkItemBase> pWorkItem(pWorkQueue->RemoveWorkItem());
			// 将其加入到工作队列中
			pWorkQueue->InsertWorkItem(pWorkItem);
		
			if(pWorkItem == XK_NULL)
			{
				XK_ASSERT(XK_FALSE);
				break;
			}
			//do all that needed with the work item
			pWorkItem->DoWork(pThreadData);
//			xkPrint(XK_LEVEL_PRINT, "pWorkItem->DoWork(pThreadData);\n");
			// 从工作队列中删除
			pWorkQueue->RemoveWorkItem(pWorkItem);
			break;
		default:
			XK_ASSERT(XK_FALSE);
			delete pThreadContext;
//			pWorkQueue->m_tSincQuit.Post();
			return 0;
		}
	}
    //delete the context of this thread
	delete pThreadContext;

//	pWorkQueue->m_tSincQuit.Post();
//	xkPrint(XK_LEVEL_PRINT, "ThreadQueue thread quit~!;\n");
	THREAD_RETURN(0);
}

/*------------------------------------------------------------------------
Destroy	

  set the abort event and wait for all threads to finish
  then cleans up the module
------------------------------------------------------------------------*/
void CXKThreadQueue::Destroy()
{
	m_SincFlag	= SINC_ABORT;
	//clean queue
	CXKWorkItemBase* pItem	= XK_NULL;
	m_tWaitMutex.Lock();
	while(!m_tWaitItemQueue.empty())
	{
		pItem	= m_tWaitItemQueue.front();
		pItem->Abort();
		delete pItem;
		m_tWaitItemQueue.pop();
	}
	m_tWaitMutex.Unlock();
	
	m_tWorkMutex.Lock();
	WorkItemQueue::iterator	it;
	for( it = m_tWorkItemQueue.begin();it != m_tWorkItemQueue.end();it++ )
	{
		XK_ASSERT( (*it) != XK_NULL );
		(*it)->Abort();
	}
	m_tWorkMutex.Unlock();

	for(int j = 0 ; j < m_nNumberOfThreads ; j++)
	{
		m_tSincObject.Post();
	}

//	m_tSincObject.Reset(SEM_VALUE_MAX-1);
	for(int i = 0 ; i < m_nNumberOfThreads ; i++)
	{
//		m_tSincObject.Post();
//		Sleep(100);
//		m_phThreads[i]->Kill();
//		Sleep(100);
//		xkPrint(XK_LEVEL_PRINT, "m_phThreads[%d]->Join() in;\n",i);
		m_phThreads[i]->Join();
//		m_tSincQuit.Wait();
//		xkPrint(XK_LEVEL_PRINT, "m_phThreads[%d]->Join() out;\n",i);
		delete m_phThreads[i];
	}
	delete [] m_phThreads;
	m_phThreads			= XK_NULL;
	m_nNumberOfThreads	= 0;

//	XK_ASSERT(m_tWaitItemQueue.empty());
//	XK_ASSERT(m_tWorkItemQueue.empty());

	m_SincFlag	= SINC_INVALID;

//	xkPrint(XK_LEVEL_PRINT, "CXKThreadQueue::Destroy()\n");
}

void CXKThreadQueue::RemoveAll()
{
	CXKWorkItemBase* pItem	= XK_NULL;
	m_tWaitMutex.Lock();
	while(!m_tWaitItemQueue.empty())
	{
		pItem	= m_tWaitItemQueue.front();
		pItem->Abort();
		delete pItem;
		m_tWaitItemQueue.pop();
	}
	m_tWaitMutex.Unlock();
	
	m_tWorkMutex.Lock();
	WorkItemQueue::iterator	it;
	for( it = m_tWorkItemQueue.begin();it != m_tWorkItemQueue.end();it++ )
	{
		XK_ASSERT( (*it) != XK_NULL );
		(*it)->Abort();
	}
	m_tWorkMutex.Unlock();
}