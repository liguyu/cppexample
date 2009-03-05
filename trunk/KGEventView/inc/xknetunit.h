/****************************************************************************/
/* File name:	xknetunit.h													*/
/* Author: 	chenwenyue													    */
/* Version: 	1.0															*/
/* Date:		2006.10.30													*/
/* Description:	��Ԫ�ඨ��ͷ�ļ�											*/
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
// ��Ԫ�����Ϣ
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
	Tuint	m_nSrcID;					// �Զ���ԪID
	Tuint	m_nSrcType;					// �Զ���Ԫ����
	string	m_strDesc;					// �Զ���Ԫ������Ϣ

	CXKTcpSocket	*m_pSocket;			// ��·
};

class CXKNetUnitTbl				// ·�ɱ�
{
public:
	CXKNetUnitTbl( );
	virtual ~CXKNetUnitTbl();
public:
	/* ͨ����ԪID,������Ԫ��Ϣ
		�ɹ������ҵ�����Ԫ�����Ϣָ�룬ʧ�ܷ���XK_NULL*/
	CXKNetUnitInfo*	GetByID( Tuint nID );
	/* ͨ����Ԫ����,������Ԫ��Ϣ
		�ɹ������ҵ�����Ԫ�����Ϣָ�룬ʧ�ܷ���XK_NULL*/
	CXKNetUnitInfo*	GetByType( Tuint nType );
	/* ͨ���׽��ֲ�����Ԫ��Ϣ */
	CXKNetUnitInfo* GetBySocket( CXKTcpSocket* pSocket );
	/* ����һ����Ԫ��Ϣ 
		����Ĳ�������ԪID,��Ԫ���ͣ���·�����ӷ���
		�ɹ�����XK_TRUE,ʧ�ܷ���XK_FALSE*/
	Tbool	Insert( Tuint nID, Tuint nType, CXKTcpSocket *pSocket, string &strDesc);
	/* �Ƴ�һ����Ԫ��Ϣ���
		��������,�Ƿ��ͷ���·��Դ
		�ɹ�����XK_TRUE,ʧ�ܷ���XK_FALSE*/
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
	XK_NETUNITTBL_MAP	m_Items;					// ·�ɱ�
	CXKMutex	m_mutex;				// ·�ɱ���Դ������
};
/* ��Ԫ�� 
	1: ��Ԫ����һ��·�ɱ�,�ɳ�������ʱͨ����Ԫ�����¼��Ϣ��̬����
	2: ����*/
class CXKNetUnit
{
public:
	CXKNetUnit();
	virtual ~CXKNetUnit();
public:
	/* �������ݰ� */
	virtual Tbool CodePacket( Tuint nID, Tbyte *msgbuf, Tint nNumSend );
	/* �������ݰ� */
	virtual void DecodePacket( Tbyte *msgbuf, Tint nNumRecv );
	/* ��ʼ�� */
	Tbool Create(CXKThreadQueue *pThreadQueue, Tuint nID, Tuint nType, string& strDesc);
	/* �����Դ */
	void Destroy();
	/* ���ӷ���� 
		���뱾�ط���˿ںţ��ȴ��ͻ����Ӷ��д�С 
		�ɹ�����XK_TRUE,ʧ�ܷ���XK_FALSE */
	Tbool AddServer( int nPortID );
	/* ���ӿͻ���
		���뱾�ؿͻ��˿ں� 
		�ɹ�������·,ʧ�ܷ���XK_NULL */
	Tbool AddClient( string &strIPAddress, int nPortID );

	virtual Tbool OnLogon( Tuint nID, Tuint nType, CXKTcpSocket *pSocket, string &strDesc );
	/* ���ӿͻ��� : �����̣߳����տͻ�����Ϣ*/
	virtual void OnAddClient(void* pWorkItem, void* pNull);
	/* �Ƴ������׽��� : �����·�ɱ��еļ�¼*/
	virtual void OnRemoveSocket(void* p, void* pNull);
protected:
	CXKNetUnitTbl	m_tNetUnitTbl;		// ·�ɱ�

	CXKThreadQueue	*m_pThreadQueue;	// �̹߳�����
private:
	Tuint	m_nID;					// ��ԪID,��������Ψһ��ʶ
	Tuint	m_nType;				// ��Ԫ����
	string	m_strDesc;				// ������Ϣ
};
#endif // !defined(__XK_NETUNIT_H)
