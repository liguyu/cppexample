/****************************************************************************/
/* File name:	xknetunit.cpp												*/
/* Author: 	chenwenyue													    */
/* Version: 	1.0															*/
/* Date:		2006.10.30													*/
/* Description:	网元类实现文件												*/
/* History:																	*/
/*	   Date:																*/
/*	   Author:																*/
/*	   Modification:														*/
/****************************************************************************/
#include "../StdAfx.h"
#include "xkdef.h"
#include "xknetunit.h"
#include "xkthreadqueue.h"
#include "xkexception.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class CXKWorkSocket : public CXKWorkItemBase
{
public:
	CXKWorkSocket(CXKNetUnit *pNetUnit);
	virtual ~CXKWorkSocket();
public:
	CXKWorkSocket(const CXKWorkSocket& other);
	CXKWorkSocket& operator =( const CXKWorkSocket& other );
	void SetSocket(CXKTcpSocket *pSocket){ m_hSocket = pSocket; }
	void SetIsServer( Tbool bServer ) { m_bServer = bServer; }

	virtual void   DoWork(void* p);
	virtual void   Abort ();
protected:

private:
	CXKTcpSocket	*m_hSocket;		// 连接对象
	Tbool			m_bServer;		// 是否服务器

	CXKNetUnit		*m_pNetUnit;
};

//////////////////////////////////////////////////////////////////////
CXKWorkSocket::CXKWorkSocket(CXKNetUnit *pNetUnit)
: m_hSocket(XK_NULL)
, m_bServer(XK_FALSE)
{
	m_pNetUnit	= pNetUnit;
}

CXKWorkSocket::CXKWorkSocket(const CXKWorkSocket& other)
{
	m_pNetUnit	= other.m_pNetUnit;
	m_bServer	= other.m_bServer;
	m_hSocket	= XK_NULL;
}

CXKWorkSocket& CXKWorkSocket::operator =( const CXKWorkSocket& other )
{
	m_pNetUnit	= other.m_pNetUnit;
	m_bServer	= other.m_bServer;
	m_hSocket	= XK_NULL;

	return *this;
}

CXKWorkSocket::~CXKWorkSocket()
{
	if( XK_NULL != m_hSocket )
	{
		delete m_hSocket;
		m_hSocket	= XK_NULL;
	}
//	xkPrint(XK_LEVEL_PRINT, "CXKWorkSocket::~CXKWorkSocket()\n");
}

void CXKWorkSocket::DoWork(void *p)
{
	if( m_bServer )
	{
		CXKWorkSocket	*pWorkSocket	= XK_NULL;
		CXKTcpSocket	*pClient		= XK_NULL;
		string clientHost;
		if( XK_NULL != m_hSocket )
		{
			m_hSocket->BindSocket();
		}
		xkPrint(XK_LEVEL_PRINT, "server finishes binding process... \n");
		while(1)
		{
			clientHost	= "";
			if( XK_TRUE != m_hSocket->ListenToClient() )
			{
				break;
			}
			xkPrint(XK_LEVEL_PRINT, "server is listening to the port (%d)... \n", m_hSocket->GetPortNumber());
			
			// 等待客户连接
			xkPrint(XK_LEVEL_PRINT, "server is waiting for client connnection ...\n ");

			if( XK_NULL != m_hSocket )
			{
				pClient	= m_hSocket->AcceptClient(clientHost);
				xkPrint(XK_LEVEL_PRINT, "==> A client from [%s] is connected!\n", clientHost.c_str());
			}

			if( XK_NULL != pClient )
			{
				pWorkSocket	= new CXKWorkSocket(*this);
				if( XK_NULL == pWorkSocket )
				{
					delete pClient;
					pClient		= XK_NULL;
				}
				pWorkSocket->SetSocket(pClient);
				pWorkSocket->SetIsServer(XK_FALSE);
				// 建立路由表
				m_pNetUnit->OnLogon( __LOGMGR_CLIENT_ID__, __LOGMGR_CLIENT_TYPE__, pClient,  string("客户端监视界面"));
				m_pNetUnit->OnAddClient(pWorkSocket, 0);
			}
			else
			{
				//xkPrint(XK_LEVEL_PRINT, "==> Cancel Listen Thread by Accept failed!\n");
				break;
			}
		}
	}
	else
	{
		Tint	nNumRecv	= 0;
		Tbyte	msgbuf[MAX_MSG_LEN];
		// 建立路由表
		m_pNetUnit->OnLogon( __LOGMGR_CLIENT_ID__, __LOGMGR_CLIENT_TYPE__, m_hSocket,  string("客户端监视界面"));
		do
		{
			if( XK_NULL != m_hSocket )
				nNumRecv	= m_hSocket->RecieveMessage(msgbuf, sizeof(msgbuf));
			if( nNumRecv <= 0 )
				//continue;
				break;

			xkPrint(XK_LEVEL_PRINT, "==> recieve msg, len: %d!\n", nNumRecv);
			// 解析报文
			if( NULL != m_pNetUnit )
				m_pNetUnit->DecodePacket( msgbuf, nNumRecv );
		}while( nNumRecv > 0 );		// 不允许空报文

		if( XK_NULL != m_pNetUnit/* && m_hSocket != XK_NULL*/ )
			m_pNetUnit->OnRemoveSocket(m_hSocket, XK_NULL);
	}

//	xkPrint(XK_LEVEL_PRINT, "CXKWorkSocket::DoWork(void *p)\n");
	delete this;
}

void CXKWorkSocket::Abort()
{
	CXKTcpSocket	*pTmp	= m_hSocket;
	if( XK_NULL != m_hSocket )
	{
		m_hSocket	= XK_NULL;
		delete pTmp;
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CXKNetUnit::CXKNetUnit()
: m_pThreadQueue(XK_NULL)
{

}

CXKNetUnit::~CXKNetUnit()
{
	Destroy();
}
/*CXKNetUnit::Destroy-----------------------------------------------------*/
void CXKNetUnit::Destroy()
{
	// clear rout table
	xkPrint(XK_LEVEL_PRINT, "NetUnit Clear rout table ...\n");		
	m_tNetUnitTbl.RemoveAll();
#ifdef XK_WORK_ON_WIN_PLATFORM
    // Close the winsock library
	if (WSACleanup())
	{
		xkPrint(XK_LEVEL_PRINT, "NetUnit Close the winsock library fail!\n");
	}
	else
	{
		xkPrint(XK_LEVEL_PRINT, "NetUnit Destroy~!\n");
	}
#endif
}
/*CXKNetUnit::Create-----------------------------------------------------*/
Tbool CXKNetUnit::Create(CXKThreadQueue* pThreadQueue, Tuint nID, Tuint nType, string& strDesc)
{
	Tbool bRet	= XK_TRUE;
#ifdef XK_WORK_ON_WIN_PLATFORM
	// 初始化socket库
	WSADATA wsaData;
    xkPrint(XK_LEVEL_PRINT, "net unit start ...\n");
	xkPrint(XK_LEVEL_PRINT, "initialize the winsock library ...\n");
	
	if (WSAStartup(0x101, &wsaData))
	{
		xkPrint(XK_LEVEL_PRINT, "load the winsock library fail!\n");
		bRet	= XK_FALSE;
	}
	else
	{
		xkPrint(XK_LEVEL_PRINT, "load the winsock library successful\n");
	}
#endif

	if( XK_NULL == pThreadQueue )	bRet	= XK_FALSE;
	else	m_pThreadQueue	= pThreadQueue;

	m_nID		= nID;
	m_nType		= nType;
	m_strDesc	= strDesc;

	return bRet;
}
/*CXKNetUnit::AddServer-----------------------------------------------------*/
Tbool CXKNetUnit::AddServer(int nPortID)
{
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

		m_pThreadQueue->InsertWaitItem( pWorkSocket);
		
		string desc	= "listen socket.";
		m_tNetUnitTbl.Insert(m_nID, nPortID, pServer, desc);
	}
	else
	{
		delete pServer;
		bRet	= XK_FALSE;
	}

	return bRet;
}
/*CXKNetUnit::AddClient-----------------------------------------------------*/
Tbool CXKNetUnit::AddClient(string &strIPAddress, int nPortID)
{
	char strTemp[100];
	TRACE0("CXKNetUnit::AddClient()===>\n");
	Tbool bRet	= XK_TRUE;
	// 得到本地地址信息
	xkPrint(XK_LEVEL_PRINT, "Retrieve the localHost [CLIENT] name and address:\n");
	TRACE0("Retrieve the localHost [CLIENT] name and address:\n");
    CXKHostInfo uHostAddress;
	string localHostName = uHostAddress.GetHostName();
    string localHostAddr = uHostAddress.GetHostIPAddress();
    xkPrint(XK_LEVEL_PRINT, "Name: %s\n", localHostName.c_str());
    xkPrint(XK_LEVEL_PRINT, "Address: %s\n", localHostAddr.c_str());
	sprintf(strTemp,"Name: %s\nAddress: %s\n", localHostName.c_str(), localHostAddr.c_str());
	TRACE0(strTemp);
	// 得到服务器信息
	xkPrint(XK_LEVEL_PRINT, "Retrieve the remoteHost [SERVER] name and address:\n");
	TRACE0("Retrieve the remoteHost [SERVER] name and address:\n");
	xkPrint(XK_LEVEL_PRINT, "		==> the given address is %s\n", strIPAddress.c_str());
	sprintf(strTemp,"		==> the given address is %s\n", strIPAddress.c_str());
	TRACE0(strTemp);
    CXKHostInfo serverInfo(strIPAddress, ADDRESS);
	string serverName,serverAddr;
	char* pInfo		= serverInfo.GetHostName();
	if( XK_NULL != pInfo )	serverName	= pInfo;
	pInfo	= serverInfo.GetHostIPAddress();
	if( XK_NULL != pInfo ) serverAddr	= pInfo;
    xkPrint(XK_LEVEL_PRINT, "Name: %s\n", serverName.c_str());
    xkPrint(XK_LEVEL_PRINT, "Address: %s\n", serverAddr.c_str());

    // 创建本地套接字
    CXKTcpSocket *pClient	= new CXKTcpSocket(nPortID);
	pClient->SetLingerOnOff(true);
	pClient->SetLingerSeconds(10);

	xkPrint(XK_LEVEL_PRINT, "client configuation: \n");
	
    // 连接服务器
	xkPrint(XK_LEVEL_PRINT, "connecting to the server [%s] ... \n", serverName.c_str());
	if( XK_TRUE != pClient->ConnectToServer(serverAddr,ADDRESS) )
	{
		delete pClient;
		bRet	= XK_FALSE;
	}
	else
	{
		TRACE0("Listen Success,now the socket begin receive data.开始接收数据。 ");
		// 起动线程接收数据
		CXKWorkSocket *pWorkSocket	= new CXKWorkSocket(this);
		if( XK_NULL != pWorkSocket )
		{
			XK_ASSERT(m_pThreadQueue != XK_NULL);
			pWorkSocket->SetSocket(pClient);
			pWorkSocket->SetIsServer(XK_FALSE);
			
			m_pThreadQueue->InsertWaitItem( pWorkSocket);
		}
		else
		{
			delete pClient;
			bRet	= XK_FALSE;
		}
	}
	
	return bRet;
}
/*CXKNetUnit::CodePacket-----------------------------------------------------*/
Tbool CXKNetUnit::CodePacket(  Tuint nID, Tbyte *msgbuf, Tint nNumSend )
{
	Tbool bRet	= XK_TRUE;
	// 得到接收者ID,从路由表中查找到接收者连接信息，将消息发送
	CXKNetUnitInfo* pUnitInfo	= m_tNetUnitTbl.GetByID( nID );
	if( XK_NULL != pUnitInfo )
	{
		Tint iRet	= pUnitInfo->GetSocket()->SendMessage(msgbuf, nNumSend);
		if( iRet < 0 )
			bRet	= XK_FALSE;
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
/*CXKNetUnit::DecodePacket-----------------------------------------------------*/
void CXKNetUnit::DecodePacket( Tbyte *msgbuf, Tint nNumRecv )
{
	if( nNumRecv<= 0 ) return;
	// 增加解码消息
}
/*CXKNetUnit::OnAddClient-----------------------------------------------------*/
void CXKNetUnit::OnAddClient(void *pWorkItem, void *pNull)
{
	XK_ASSERT( XK_NULL != pWorkItem );
	if( XK_NULL != pWorkItem )
	{
		CXKWorkSocket*	pWorkSocket	= (CXKWorkSocket *) pWorkItem;
		m_pThreadQueue->InsertWaitItem( pWorkSocket);
	}
}
Tbool CXKNetUnit::OnLogon( Tuint nID, Tuint nType, CXKTcpSocket *pSocket, string &strDesc )
{
	return m_tNetUnitTbl.Insert(nID, nType, pSocket, strDesc);
}
/*CXKNetUnit::OnAddClient-----------------------------------------------------*/
void CXKNetUnit::OnRemoveSocket(void *p, void *pNull)
{
//	XK_ASSERT( XK_NULL != p );
	if( XK_NULL != p )
	{
		CXKTcpSocket* pSocket	= (CXKTcpSocket* ) p;
		m_tNetUnitTbl.Remove(pSocket);
		xkPrint(XK_LEVEL_PRINT, "==>remove socket ~!\n");
	}
}

/*CXKNetUnitTbl::CXKNetUnitTbl-----------------------------------------------------*/
CXKNetUnitTbl::CXKNetUnitTbl()
{
}
/*CXKNetUnitTbl::~CXKNetUnitTbl-----------------------------------------------------*/
CXKNetUnitTbl::~CXKNetUnitTbl()
{
	RemoveAll();
}
/*CXKNetUnitTbl::GetByID-----------------------------------------------------*/
CXKNetUnitInfo* CXKNetUnitTbl::GetByID( Tuint nID )
{
	CXKNetUnitInfo*		pUnitInfo	= XK_NULL;
	XK_NETUNITTBL_MAP::iterator	it;
	Lock();
	for(it	= m_Items.begin(); it != m_Items.end(); it++ )
	{
		pUnitInfo	= (*it).second;
		XK_ASSERT( XK_NULL != pUnitInfo );
		if( pUnitInfo->GetID() == nID )
		{
			break;
		}
		
		pUnitInfo	= XK_NULL;
	}
	Unlock();

	return pUnitInfo;
}
/*CXKNetUnitTbl::~CXKNetUnitTbl-----------------------------------------------------*/
CXKNetUnitInfo* CXKNetUnitTbl::GetByType( Tuint nType )
{
	CXKNetUnitInfo*		pUnitInfo	= XK_NULL;
	XK_NETUNITTBL_MAP::iterator	it;
	Lock();
	it	= m_Items.find( nType);
	if( it != m_Items.end() )
	{
		pUnitInfo	= (*it).second;
		XK_ASSERT( pUnitInfo != XK_NULL );
	}
	Unlock();

	return pUnitInfo;
}
/*CXKNetUnitTbl::~CXKNetUnitTbl-----------------------------------------------------*/
CXKNetUnitInfo* CXKNetUnitTbl::GetBySocket( CXKTcpSocket* pSocket )
{
	CXKNetUnitInfo*		pUnitInfo	= XK_NULL;
	XK_NETUNITTBL_MAP::iterator	it;
	Lock();
	for(it	= m_Items.begin(); it != m_Items.end(); it++ )
	{
		pUnitInfo	= (*it).second;
		XK_ASSERT( XK_NULL != pUnitInfo );
		if( pUnitInfo->GetSocket() == pSocket )
		{
			break;
		}

		pUnitInfo	= XK_NULL;
	}
	Unlock();

	return pUnitInfo;
}
/*CXKNetUnitTbl::~CXKNetUnitTbl-----------------------------------------------------*/
Tbool CXKNetUnitTbl::Insert( Tuint nID, Tuint nType, CXKTcpSocket *pSocket, string &strDesc )
{
	Tbool	bRet	= XK_TRUE;
	XK_ASSERT(XK_NULL != pSocket );
	if( XK_NULL != pSocket )
	{
		CXKNetUnitInfo* pUnitInfo	= new CXKNetUnitInfo();
		if( XK_NULL != pUnitInfo )
		{
			Lock();
			m_Items.insert(XK_NETUNITTBL_MAP::value_type(nType, pUnitInfo));
			pUnitInfo->SetID(nID);
			pUnitInfo->SetType(nType);
			pUnitInfo->SetSocket(pSocket);
			if( !strDesc.empty() )
			{
				pUnitInfo->SetDesc(strDesc);
			}
			Unlock();
		}
		else
		{
			bRet	= XK_FALSE;
		}
	}
	else
	{
		bRet	= XK_FALSE;
	}

	return bRet;
}
/*CXKNetUnitTbl::~CXKNetUnitTbl-----------------------------------------------------*/
Tbool CXKNetUnitTbl::Remove( Tuint nType )
{
	Tbool	bRet	= XK_TRUE;
	CXKNetUnitInfo*		pUnitInfo	= XK_NULL;
	XK_NETUNITTBL_MAP::iterator	it;
	Lock();
	it	= m_Items.find( nType);
	if( it != m_Items.end() )
	{
		pUnitInfo	= (*it).second;
		XK_ASSERT( pUnitInfo != XK_NULL );
		XK_ASSERT( pUnitInfo->GetType() == nType );
		XK_ASSERT( pUnitInfo->GetSocket() != XK_NULL );

		m_Items.erase(it);
		delete pUnitInfo;
	}
	else
	{
		bRet	= XK_FALSE;
	}
	Unlock();

	return bRet;
}
/*CXKNetUnitTbl::~CXKNetUnitTbl-----------------------------------------------------*/
Tbool CXKNetUnitTbl::Remove( CXKTcpSocket* pSocket )
{
	Tbool	bRet	= XK_FALSE;
	CXKNetUnitInfo*		pUnitInfo	= XK_NULL;
	XK_NETUNITTBL_MAP::iterator	it;
	Lock();
	for(it	= m_Items.begin(); it != m_Items.end(); it++ )
	{
		pUnitInfo	= (*it).second;
		XK_ASSERT( XK_NULL != pUnitInfo );
		XK_ASSERT( pUnitInfo->GetSocket() != XK_NULL );
		if( pUnitInfo->GetSocket() == pSocket )
		{
			m_Items.erase(it);
			delete pUnitInfo;

			bRet	= XK_TRUE;
			break;
		}
	}
	Unlock();

	return bRet;
}

void CXKNetUnitTbl::RemoveAll()
{
	CXKNetUnitInfo*		pUnitInfo	= XK_NULL;
	XK_NETUNITTBL_MAP::iterator	it;
	Lock();
	for(it	= m_Items.begin(); it != m_Items.end(); it++ )
	{
		pUnitInfo	= (*it).second;
		XK_ASSERT( XK_NULL != pUnitInfo );
		XK_ASSERT( pUnitInfo->GetSocket() != XK_NULL );
		delete pUnitInfo;
	}
	m_Items.clear();
	Unlock();
}