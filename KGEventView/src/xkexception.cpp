#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef WIN32
#include <direct.h>
#endif

#include "xkexception.h"

CDJLogMgr	*g_tLogMgr = XK_NULL;//lmy

int g_nPortTbl[16][16]	= 
{
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
//////////////////////////////////////////////////////////////////////
int InitLogMgr(int nModuleType, int nModuleID)
{
	printf("LogMgr InitLogMgr nModuleType: %d, nModuleID: %d)\n", nModuleType, nModuleID);

	if( XK_NULL != g_tLogMgr )
	{
		return 0;
//		delete g_tLogMgr;
//		g_tLogMgr	= XK_NULL;
	}
	g_tLogMgr	= new CDJLogMgr();
	if( XK_NULL == g_tLogMgr ) return -1;
	g_tLogMgr->Init();
	// 从配置文件中读取
	if( nModuleType>16 || nModuleType<= 0 || nModuleID>16 || nModuleID<= 0 )
		return -1;

	int nPort	= g_nPortTbl[nModuleType-1][nModuleID-1];
	// 侦听
	return g_tLogMgr->AddServer(nPort);
}

void ExitLogMgr()
{
	if( XK_NULL != g_tLogMgr )
	{
		delete g_tLogMgr;
		g_tLogMgr	= XK_NULL;
	}
}

void xkPrint(int nLevel, const char *format,...)
{
	if( XK_NULL == g_tLogMgr ) return;

	char buf[XK_MAX_STREAM_LEN];
	int	nCnt	= 0;
	memset(buf, 0, sizeof(buf) );

	// 打印时间头
//	nCnt	= g_tLogMgr.m_tStream.PrintTime( buf );
	// 打印内容
	va_list ap;
	va_start( ap,format );
	nCnt	+= vsprintf( &buf[nCnt], format , ap );
	va_end( ap );
	// 打印'\0'
	XK_ASSERT( nCnt< XK_MAX_STREAM_LEN );
	buf[nCnt]	= '\0';
	nCnt++;

	g_tLogMgr->m_tStream.Print(nLevel, buf ,nCnt);
}

void xkPrintb(int nLevel, const char *pTitle,  int nTitleLen, const char *pData ,int len)
{
	if( XK_NULL == g_tLogMgr ) return ;
	g_tLogMgr->m_tStream.Printb(nLevel, pTitle, nTitleLen, pData, len);
}
////////////////////////////////////////////////////////////////////////////
CXKWorkLog::CXKWorkLog()
: m_tLogOut( XK_NULL )
, m_bAbort(XK_FALSE)
{
}

Tbool CXKWorkLog::Create( CDJLogOut* tLogOut )
{
	m_tLogOut	= tLogOut;

	return XK_TRUE;
}

CXKWorkLog::CXKWorkLog(const CXKWorkLog& other)
{
	m_tLogOut	= other.m_tLogOut;
}

CXKWorkLog& CXKWorkLog::operator =( const CXKWorkLog& other )
{
	m_tLogOut	= other.m_tLogOut;
	return *this;
}

CXKWorkLog::~CXKWorkLog()
{
//	xkPrint(XK_LEVEL_PRINT, "CXKWorkLog::~CXKWorkLog()\n");
//	m_tStream	= XK_NULL;
}

void CXKWorkLog::DoWork(void *p)
{
	if( XK_NULL == m_tLogOut )
	{
		XK_ASSERT(XK_FALSE);
		return;
	}
	int		n	= 0;
	while( 1 )
	{
		if( m_bAbort )
		{
			m_tLogOut->Flush();
//			xkPrint(XK_LEVEL_PRINT, "CXKWorkLog::DoWork(void *p) out\n");
			break;
		}

		n++;

		if( m_tLogOut->IsEmpty() )
		{
			Sleep(0);
		}
		else
		{
			m_tLogOut->Pop();
		}

		if( n%20 == 0 )
		{
//			xkPrint(XK_LEVEL_PRINT, "WorkLog %d\n", n);
			Sleep(20);
		}
	}
	//xkPrint(XK_LEVEL_PRINT, "CXKWorkLog::DoWork()\n", n);
	
	delete this;
}

void CXKWorkLog::Abort()
{
	m_bAbort	= XK_TRUE;
}

CDJLogNetUnit::CDJLogNetUnit()
{
}

CDJLogNetUnit::~CDJLogNetUnit()
{
}

void CDJLogNetUnit::RemoveAll()
{
	m_tNetUnitTbl.RemoveAll();
}

Tbool CDJLogNetUnit::OnLogon( Tuint nID, Tuint nType, CXKTcpSocket *pSocket, string &strDesc )
{
	while(m_tNetUnitTbl.Remove(__LOGMGR_CLIENT_TYPE__))
	{
		;
	}
	return CXKNetUnit::OnLogon( nID, nType, pSocket, strDesc );
}

void CDJLogNetUnit::OnAddClient(void* pWorkItem, void* pNull)
{
	CXKNetUnit::OnAddClient(pWorkItem, pNull);
}

CDJLogOut::CDJLogOut() 
{ 
	m_nLevel	= XK_LEVEL_LOG;
	m_bAutoDel	= false;

	m_tQueue.Init();
}

CDJLogOut::~CDJLogOut() 
{
	
}

void CDJLogOut::OnStreamOut(const char *pData, const Tuint nCnt)
{
	Tbyte *p	= XK_NULL;
	Tuint nNum	= 0;
	Lock();
	// 如果缓存满，则将第一条记录删除
	if( m_tQueue.IsFull() )
	{
		if( m_tQueue.GetFirstData( p, nNum ) )
		{
			m_tQueue.Release(p);
		}
	}
	XK_ASSERT( !m_tQueue.IsFull() );
	Tbyte	*pBuf	= m_tQueue.Allocate(nCnt);
	XK_ASSERT( XK_NULL != pBuf );
	XK_ASSERT( nCnt <= XK_MAX_STREAM_LEN );
	memcpy( pBuf, pData, nCnt );
	Unlock();
}

// CDJLogOut::Flush ------------------------------------------------------------
void CDJLogOut::Flush()
{
	Tbyte *p		= XK_NULL;
	Tuint nNum		= 0;

	Lock();
	Tbool bRet		= m_tQueue.GetFirstData( p, nNum );
	while( bRet )
	{
		XK_ASSERT( XK_NULL != p );
		Send((char*)p, nNum);
	
		bRet	= m_tQueue.GetNextData(p, nNum);
	}
	// 清空缓存
	m_tQueue.Reset();
	Unlock();
}
// CDJLogOut::Pop ------------------------------------------------------------
void CDJLogOut::Pop()
{
	Tbyte *p	= XK_NULL;
	Tuint nNum		= 0;
	Lock();
	if( m_tQueue.IsEmpty() ) return;
	if( m_tQueue.GetFirstData( p, nNum ) )
	{
		XK_ASSERT( XK_NULL != p );
		Send((char*)p, nNum);
		// 清除当前信息
		m_tQueue.Release(p);
	}
	Unlock();
}
// CDJLogOut::IsEmpty ------------------------------------------------------------
Tbool CDJLogOut::IsEmpty() 
{
	Lock();
	Tbool bRet	= m_tQueue.IsEmpty();
	Unlock();
	return bRet; 
}
// CDJLogOut::Print ------------------------------------------------------------
void CDJLogOut::Send(const char *pData, int nCnt)
{
	m_tNetUnit.CodePacket( __LOGMGR_CLIENT_ID__, (Tbyte*)pData, nCnt );
}

CDJLogMgr::CDJLogMgr()
: m_nServerPort(__LISTEN_SOCKET_PORT__)
, m_tThreadQueue(XK_NULL)
, m_bInit(false)
{
}

CDJLogMgr::~CDJLogMgr()
{
	Destroy();
}

void CDJLogMgr::Destroy()
{
//	xkPrintf("CDJLogMgr::Destroy()\n");
	// 清除线程
	if( XK_NULL != m_tThreadQueue )
	{
		delete m_tThreadQueue;
		m_tThreadQueue	= XK_NULL;
	}
	//
	//xkPrint(XK_LEVEL_PRINT, "m_tStream.RemoveOutput(&m_tLogOut);");
	m_tStream.RemoveOutput(&m_tLogOut);
	//
	//xkPrint(XK_LEVEL_PRINT, "m_tLogOut.m_tNetUnit.Destroy();");
	m_tLogOut.m_tNetUnit.RemoveAll();

	m_bInit		= false;
}

void CDJLogMgr::Init()
{
	if( m_bInit )
	{
		Destroy();
	}
	//
	m_tStream.InsertOutput(&m_tLogOut);

	// 起动线程:一个连接线程,一个发送线程
	m_tThreadQueue	= new CXKThreadQueue();
	m_tThreadQueue->Create(2);
	// 建立网元
	m_tLogOut.m_tNetUnit.Create(m_tThreadQueue, __LOGMGR_SERVER_ID__, __LOGMGR_SERVER_TYPE__, string("DJITPLogMgr=>msg server"));
	// 创建消息发送线程
	CXKWorkLog *tLogWorkItem	= new CXKWorkLog();
	if( tLogWorkItem != XK_NULL )
	{
		tLogWorkItem->Create(&m_tLogOut);
		m_tThreadQueue->InsertWaitItem(tLogWorkItem);
	}

	char strKey[8];
	memset(strKey, 0, sizeof(strKey));
	// 读取配置的端口号
	char szBuf[100]={0};
	char szProName[100]={0};
	int nRet = 0;
	unsigned long u32Size = 0;
	HKEY hKey;
	nRet = RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\DJXMS",&hKey);
	if(hKey != NULL)
	{
		u32Size = sizeof(szBuf);
		nRet = RegQueryValueEx(hKey,"InstallDir",0,NULL,(unsigned char*)szBuf,&u32Size);
		if(nRet)
		{
			strcpy(szBuf,"DJITPLogMgr.ini");
		}
		else
		{
			strcat(szBuf,"Ini\\DJITPLogMgr.ini");
		}
	}
	else
	{
		strcpy(szBuf,"DJITPLogMgr.ini");
	}
	RegCloseKey(hKey);
	for(int nType= 0;nType<16; nType++)
	{
		for(int nID =0;nID<16;nID++)
		{
			int nCnt	= sprintf(strKey, "%d_%d", nType+1, nID+1);
			strKey[nCnt]	= '\0';
			g_nPortTbl[nType][nID]	= GetPrivateProfileInt("SERVER_PORT", strKey, 0, szBuf);
		}
	}

	m_bInit		= true;
}

int CDJLogMgr::AddServer(int nPort)
{
	/*
		Tbool bRet	= XK_TRUE;
	// 得到本地地址信息
	xkPrint(XK_LEVEL_PRINT, "Retrieve the local host name and address:\n");
	
    CXKHostInfo uHostAddress;
	string localHostName = uHostAddress.GetHostName();
    string localHostAddr = uHostAddress.GetHostIPAddress();
	xkPrint(XK_LEVEL_PRINT, "		==> Name: %s\n", localHostName.c_str());
	xkPrint(XK_LEVEL_PRINT, "		==> Address: %s\n", localHostAddr.c_str());

	// 建立本地套接字
	CXKTcpSocket *pServer	= new CXKTcpSocket(nPortID);
	xkPrint(XK_LEVEL_PRINT, "server configuation: \n");
	XK_ASSERT( pServer != XK_NULL );

	// 起动线程接受客户端连接
    CXKWorkSocket *pWorkSocket	= new CXKWorkSocket(this);
	if( XK_NULL != pWorkSocket )
	{
		XK_ASSERT(m_pThreadQueue != XK_NULL);
		pWorkSocket->SetSocket(pServer);
		pWorkSocket->SetIsServer(XK_TRUE);

		m_pThreadQueue->InsertWorkItem( pWorkSocket);
		
		string desc	= "listen socket.";
		m_tNetUnitTbl.Insert(m_nID, nPortID, pServer, desc);
	}
	else
	{
		delete pServer;
		bRet	= XK_FALSE;
	}

	return bRet;
	*/
	m_tLogOut.m_tNetUnit.RemoveAll();
	return m_tLogOut.m_tNetUnit.AddServer(nPort);
}
////////////////////////////////////////////////////////////////////////////
//刷新日志
////////////////////////////////////////////////////////////////////////////
CXKStreamQueue::CXKStreamQueue()
:m_pPool( XK_NULL )
, m_nFree( 0 )
, m_nUsed( 0 )
, m_nLoopIndex( 0 )
{
}

CXKStreamQueue::~CXKStreamQueue()
{
	if( XK_NULL != m_pPool )
	{
		delete m_pPool;
		m_pPool	= XK_NULL;
	}

	m_nFree	= 0;
	m_nUsed	= 0;
	m_nLoopIndex	= 0;
}


Tbool CXKStreamQueue::Init()
{
	Tbool	bRet	= XK_FALSE;
	if( XK_NULL != m_pPool )
	{
		delete m_pPool;
		m_pPool	= XK_NULL;
	}

	XK_ASSERT( XK_NULL == m_pPool );
	m_pPool	= new Tbyte[XK_MAX_STREAM_NUM * (XK_MAX_STREAM_LEN + sizeof(Tuint))];

	if( XK_NULL != m_pPool)
	{
		Reset();
		bRet	= XK_TRUE;
	}

	return bRet;
}

void CXKStreamQueue::Reset()
{
	for( int i= 1;i<= XK_MAX_STREAM_NUM;i++ )
	{
		m_nQueue[i]	= i+1;
	}
	m_nQueue[XK_MAX_STREAM_NUM]	= 0;

	m_nFree	= 1;
	m_nUsed	= 0;
	m_nQueue[0]	= 0;

	m_nLoopIndex	= 0;

	memset(m_pPool, 0, sizeof(XK_MAX_STREAM_NUM * (XK_MAX_STREAM_LEN + sizeof(Tuint))));
}

Tbyte *CXKStreamQueue::Allocate(Tuint nNum)
{
	// 队列为空，返回空指针
	if( 0 == m_nFree  || m_nFree > XK_MAX_STREAM_NUM )
		return XK_NULL;

	XK_ASSERT( XK_NULL != m_pPool );
	XK_ASSERT( m_nFree <= XK_MAX_STREAM_NUM && m_nFree != 0 );

	// 调整可用资源链表
	Tuint nIndex	= m_nFree;
	m_nFree			= m_nQueue[m_nFree];
	// 调整已分配资源链表,新分配的节点放到链尾
	if( 0 == m_nUsed )
	{
		m_nQueue[nIndex]	= 0;
		m_nUsed				= nIndex;
	}
	else
	{
		XK_ASSERT( m_nQueue[0] <= XK_MAX_STREAM_NUM );
		XK_ASSERT( m_nQueue[0] != 0 );
		Tuint nT	= m_nQueue[0];
		XK_ASSERT( m_nQueue[nT] == 0 );

		m_nQueue[nIndex]	= 0;
		m_nQueue[nT]		= nIndex;
	}
	// 调整已分配队尾指针
	m_nQueue[0]	= nIndex;

	Tbyte * p	= (&m_pPool[(nIndex - 1) * (XK_MAX_STREAM_LEN + sizeof(Tuint))] + sizeof(Tuint));
	*(p-sizeof(Tuint)) = nNum;

	return p;
}

void CXKStreamQueue::Release(Tbyte* pData)
{
	XK_ASSERT( pData != XK_NULL );
	if( pData == XK_NULL ) return;
	Tbyte *p	= pData - sizeof(Tuint);
	if( p< m_pPool || p> (m_pPool + XK_MAX_STREAM_NUM * (XK_MAX_STREAM_LEN + sizeof(Tuint)) ) )
		return;

	XK_ASSERT( ( p - m_pPool )%(XK_MAX_STREAM_LEN + sizeof(Tuint)) == 0); 
	Tuint nIndex	= ( p - m_pPool )/(XK_MAX_STREAM_LEN + sizeof(Tuint)) + 1;
	XK_ASSERT( nIndex <= XK_MAX_STREAM_NUM );
	
	// 调整已分配资源链表
	if( nIndex == m_nUsed )
	{
		// 如果第一个结点，修改m_nUsed值
		m_nUsed	= m_nQueue[nIndex];
		if( nIndex == m_nQueue[0] ) m_nQueue[0]	= 0;
	}
	else
	{
		for( int nT	= m_nUsed ; ; )
		{
			if( nIndex == m_nQueue[nT] )
			{
				m_nQueue[nT]	= m_nQueue[nIndex];
				// 如果是链尾，修改m_nQueue[0]指向新的链尾
				if( nIndex == m_nQueue[0] ) m_nQueue[0]	= nT;
				break;
			}
			nT	= m_nQueue[nT];
		}
	}
	// 调整可以用资源链表
	m_nQueue[nIndex]	= (m_nFree == 0)?0:m_nFree;
	m_nFree	= nIndex;
}

Tbool CXKStreamQueue::GetFirstData(Tbyte *&pData, Tuint &nNum)
{
	if( m_nUsed == 0 ) 
	{
		return XK_FALSE;
	}
	m_nLoopIndex	= m_nUsed;
	XK_ASSERT( m_nLoopIndex <=XK_MAX_STREAM_NUM );
	pData	= &m_pPool[(m_nLoopIndex-1) * (XK_MAX_STREAM_LEN + sizeof(Tuint))] + sizeof(Tuint);
	nNum	= *(pData -sizeof(Tuint));

	return XK_TRUE;
}

Tbool CXKStreamQueue::GetNextData(Tbyte *&pData, Tuint &nNum)
{
	if( m_nLoopIndex == 0 ) return XK_FALSE;
	Tbool	bRet	= XK_FALSE;
	// 到链表尾
	if( m_nLoopIndex == m_nQueue[0] )
	{
		m_nLoopIndex = 0;
	}
	else
	{
		m_nLoopIndex	= m_nQueue[m_nLoopIndex];
		XK_ASSERT( m_nLoopIndex <= XK_MAX_STREAM_NUM );
		XK_ASSERT( m_nLoopIndex != 0 );
		pData	= &m_pPool[(m_nLoopIndex - 1) * (XK_MAX_STREAM_LEN + sizeof(Tuint))] + sizeof(Tuint);
		nNum	= *(pData - sizeof(Tuint));

		bRet	= XK_TRUE;
	}

	return bRet;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
CXKStreamOutput::CXKStreamOutput()
{ 
	m_nLevel = 0;
	m_bAutoDel	= true;
}

CXKStreamOutput::~CXKStreamOutput()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
CXKStream::CXKStream()
{
	m_iTimeFlag			= TIME_TO_NULL;

	InsertOutput(&m_stdout);
}
CXKStream::~CXKStream()
{

}
// CXKStream::InsertOutput ------------------------------------------------------------
Tbool CXKStream::InsertOutput(CXKStreamOutput *pOutput)
{
	if( XK_NULL == pOutput )	return XK_FALSE;
	// 不允许重复插入
	XK_LIST_OUTPUT::iterator it;
	for( it = m_Items.begin();it != m_Items.end(); it++ )
	{
		XK_ASSERT( *it != XK_NULL );
		if( (*it)->GetLevel()>= pOutput->GetLevel() )
		{
			break;
		}
	}

	m_Items.insert( it, pOutput );

	return XK_TRUE;
}
// CXKStream::RemoveOutput ------------------------------------------------------------
Tbool CXKStream::RemoveOutput(CXKStreamOutput *pOutput)
{
	Tbool bRet	= XK_FALSE;
	XK_LIST_OUTPUT::iterator it;
	if( m_Items.empty() ) return XK_FALSE;
	for( it = m_Items.begin();it != m_Items.end(); it++ )
	{
		XK_ASSERT( *it != XK_NULL );
		if( (*it) == pOutput )
		{
			if( (*it)->IsAutoDel() )
			{
				delete (*it);
			}
			
			m_Items.erase(it);
			bRet	= XK_TRUE;

			break;
		}
	}

	return bRet;
}
// CXKStream::Print ------------------------------------------------------------
void CXKStream::Print(int nLevel, const char *pData, int nCnt)
{
	XK_LIST_OUTPUT::iterator it;
	for( it = m_Items.begin();it != m_Items.end(); it++ )
	{
		XK_ASSERT( *it != XK_NULL );
		if( (*it)->GetLevel() == nLevel )
		{
			(*it)->OnStreamOut((char*)pData, nCnt);
			break;
		}
	}
}
// CXKStream::Printb ------------------------------------------------------------
void CXKStream::Printb(int nLevel, const char *pTitle, int nTitleLen, const char *pData ,int len)
{
	if( XK_NULL == pData ) return;
	char buf[XK_MAX_STREAM_LEN];
	int	nCnt	= 0;
	memset(buf, 0, sizeof(buf) );
	if( XK_NULL != pTitle && nTitleLen > 0 && nTitleLen< XK_MAX_STREAM_LEN )
	{
		memcpy( buf, pTitle, nTitleLen );
		nCnt	= nTitleLen;
	}

	if(XK_NULL != pData && len> 0 && len+nCnt< XK_MAX_STREAM_LEN )
	{
		memcpy( buf+nCnt, pData, len );
		nCnt += len;
	}
	XK_ASSERT( nCnt< XK_MAX_STREAM_LEN );
/*	char buf[XK_MAX_STREAM_LEN];
	int	nCnt	= 0;
	memset(buf, 0, sizeof(buf) );

	// 打印时间头
	nCnt	= PrintTime( buf );
	// 打印内容
	int nT	= ( len*2 + nCnt -1 ) >= XK_MAX_STREAM_LEN? ( XK_MAX_STREAM_LEN - nCnt -1 ) : len;
	for( int i=0; i<nT ; i++ ) 
	{
		nCnt	+= sprintf(&buf[nCnt], "%02x ", pData[i]);
		if( ((i+1) % XK_BIT_PER_LEN ) == 0  )
		{
			nCnt	+= sprintf(&buf[nCnt], "\n");
		}
	}
	// 打印'\0'
	XK_ASSERT( nCnt< XK_MAX_STREAM_LEN );
	buf[nCnt++]	= '\n';
	buf[nCnt++]	= '\0';
*/
	Print(nLevel, buf ,nCnt);
}
// CXKStream::PrintTime ------------------------------------------------------------
int CXKStream::PrintTime( char *pTime )
{
	if( TIME_TO_NULL == m_iTimeFlag )	return 0;

	time_t curr_time = time(XK_NULL);
    struct tm *pt = XK_NULL;
#ifdef WIN32
    pt = localtime(&curr_time);
#else
	struct tm t1;
	pt = localtime_r(&curr_time,&t1);
#endif
	if( !pt )	return 0;

	int n	= 0;
	switch( m_iTimeFlag )
	{
	case TIME_TO_YEAR:
		n	= sprintf( pTime ,"%04d/%02d/%02d %02d:%02d:%02d ", pt->tm_year+1900, pt->tm_mon+1 ,pt->tm_mday ,pt->tm_hour ,pt->tm_min ,pt->tm_sec );
		break;
	case TIME_TO_MONTH:
		n	= sprintf( pTime ,"/%02d/%02d %02d:%02d:%02d ", pt->tm_mon+1 ,pt->tm_mday ,pt->tm_hour ,pt->tm_min ,pt->tm_sec );
		break;
	case TIME_TO_HOUR:
		n	= sprintf( pTime ,"%02d:%02d:%02d ", pt->tm_hour ,pt->tm_min ,pt->tm_sec );
		break;
	default:
		break;
	}

	return n;
}