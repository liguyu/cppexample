/****************************************************************************/
/* File name:	xknetunit.h													*/
/* Author: 	chenwenyue													    */
/* Version: 	1.0															*/
/* Date:		2006.10.30													*/
/* Description:	网元类定义头文件											*/
/* History:																	*/
/*	   Date:																*/
/*	   Author:																*/
/*	   Modification:														*/
/****************************************************************************/
#ifndef __XK_NETUNIT_H
#define __XK_NETUNIT_H

#include "xkdef.h"
#include "xksocket.h"
#include "xkthreadqueue.h"
#include "xkthread.h"

#define __LISTEN_SOCKET_PORT__				2008
#define __LOGMGR_CLIENT_ID__				0x0002
#define __LOGMGR_CLIENT_TYPE__				0x0002
#define __SERVER_IP__						"192.168.10.45" //"127.0.0.1"//
#define __LOGMGR_SERVER_ID__				0x0001
#define __LOGMGR_SERVER_TYPE__				0x0001
// 网元结点信息
class CXKNetUnitInfo
{
public:
	CXKNetUnitInfo():m_pSocket(XK_NULL){}
	virtual ~CXKNetUnitInfo(){}
public:
	void SetID(Tuint nID) { m_nSrcID = nID; }
	void SetType(Tuint nType) { m_nSrcType	= nType; }
	void SetDesc(string &desc) { m_strDesc	= desc; }
	void SetSocket(CXKTcpSocket* pSocket) { m_pSocket = pSocket; } 
	Tuint GetID()	{ return m_nSrcID; }
	Tuint GetType() { return m_nSrcType; }
	string& GetDesc() { return m_strDesc; }
	CXKTcpSocket* GetSocket() { return m_pSocket; }
private:
	Tuint	m_nSrcID;					// 对端网元ID
	Tuint	m_nSrcType;					// 对端网元类型
	string	m_strDesc;					// 对端网元描述信息

	CXKTcpSocket	*m_pSocket;			// 链路
};

class CXKNetUnitTbl				// 路由表
{
public:
	CXKNetUnitTbl( );
	virtual ~CXKNetUnitTbl();
public:
	/* 通过网元ID,查找网元信息
		成功返回找到的网元结点信息指针，失败返回XK_NULL*/
	CXKNetUnitInfo*	GetByID( Tuint nID );
	/* 通过网元类型,查找网元信息
		成功返回找到的网元结点信息指针，失败返回XK_NULL*/
	CXKNetUnitInfo*	GetByType( Tuint nType );
	/* 通过套接字查找网元信息 */
	CXKNetUnitInfo* GetBySocket( CXKTcpSocket* pSocket );
	/* 插入一个网元信息 
		传入的参数是网元ID,网元类型，链路，连接方向
		成功返回XK_TRUE,失败返回XK_FALSE*/
	Tbool	Insert( Tuint nID, Tuint nType, CXKTcpSocket *pSocket, string &strDesc);
	/* 移除一个网元信息结点
		传入类型,是否释放链路资源
		成功返回XK_TRUE,失败返回XK_FALSE*/
	Tbool	Remove( Tuint nType );
	Tbool	Remove( CXKTcpSocket* pSocket );
	
	void	RemoveAll();
private:
	CXKNetUnitTbl( const CXKNetUnitTbl & other);
	CXKNetUnitTbl & operator = ( const CXKNetUnitTbl & other);
private:
	void Lock() { m_mutex.Lock(); }
	void Unlock() { m_mutex.Unlock(); }
	
	typedef map<Tuint, CXKNetUnitInfo*>			XK_NETUNITTBL_MAP;
	XK_NETUNITTBL_MAP	m_Items;					// 路由表
	CXKMutex	m_mutex;				// 路由表资源互斥量
};
/* 网元类 
	1: 网元包含一张路由表,由程序启动时通过网元结点间登录消息动态创建
	2: 包含*/
class CXKNetUnit
{
public:
	CXKNetUnit();
	virtual ~CXKNetUnit();
public:
	/* 编码数据包 */
	virtual Tbool CodePacket( Tuint nID, Tbyte *msgbuf, Tint nNumSend );
	/* 解码数据包 */
	virtual void DecodePacket( Tbyte *msgbuf, Tint nNumRecv );
	/* 初始化 */
	Tbool Create(CXKThreadQueue *pThreadQueue, Tuint nID, Tuint nType, string& strDesc);
	/* 清除资源 */
	void Destroy();
	/* 增加服务端 
		传入本地服务端口号，等待客户链接队列大小 
		成功返回XK_TRUE,失败返回XK_FALSE */
	Tbool AddServer( int nPortID );
	/* 增加客户端
		传入本地客户端口号 
		成功返回链路,失败返回XK_NULL */
	Tbool AddClient( string &strIPAddress, int nPortID );

	virtual Tbool OnLogon( Tuint nID, Tuint nType, CXKTcpSocket *pSocket, string &strDesc );
	/* 增加客户端 : 启动线程，接收客户端消息*/
	virtual void OnAddClient(void* pWorkItem, void* pNull);
	/* 移除本地套接字 : 清除在路由表中的记录*/
	virtual void OnRemoveSocket(void* p, void* pNull);
protected:
	CXKNetUnitTbl	m_tNetUnitTbl;		// 路由表

	CXKThreadQueue	*m_pThreadQueue;	// 线程管理类
private:
	Tuint	m_nID;					// 网元ID,在网络上唯一标识
	Tuint	m_nType;				// 网元类型
	string	m_strDesc;				// 描述信息
};
#endif // !defined(__XK_NETUNIT_H)
