#ifndef __LOG_H__
#define __LOG_H__
/*
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/stat.h>
*/
#ifdef WIN32
#include <cassert>
#include <io.h>
#else
#include <unistd.h>
#include <assert.h>
#endif

#include "xkdef.h"
#include "xkthread.h"
#include "xknetunit.h"

#define XK_MAX_STREAM_LEN			512				// �쳣��Ϣ��󳤶�
#define XK_MAX_STREAM_NUM			2000			// ��ౣ����쳣����	

#define XK_BIT_PER_LEN					16

class CXKStream;
class CXKStreamOutput;
///////////////////////////////////////////////////////////////////////////////////////////////////////
/* ��Ϣ���˿���:������Ϣ������,��ʾ���û� */
class CXKStreamOutput
{
public:
	CXKStreamOutput();
	virtual ~CXKStreamOutput();
public:
	/* �õ��ӿڴ�����Ϣ�ļ��� */
	Tint GetLevel() const { return m_nLevel; }
public:
	/* ������Ϣ */
	virtual void OnStreamOut(const char *pData, const Tuint nCnt)=0;

	bool	IsAutoDel() { return m_bAutoDel; }
protected:
	Tint	m_nLevel;			// �ӿڴ�����Ϣ����
	bool	m_bAutoDel;
};
/* ��׼������ӿ� */
class CXKStdOut : public CXKStreamOutput
{
public:
	CXKStdOut() { m_nLevel = XK_LEVEL_PRINT; }
	virtual ~CXKStdOut(){}
public:
	void OnStreamOut(const char *pData, const Tuint nCnt) { if( XK_NULL != pData ) printf("%s",pData); }
};

class CXKStreamQueue	
{
public:
	CXKStreamQueue();
	virtual ~CXKStreamQueue();
public:
	/* ��ʼ����*/
	Tbool Init();
	/* ��λ */
	void Reset();
	/* �õ��쳣�洢λ��
		�ɹ������������쳣��Ϣλ��,
		ʧ�ܷ���XK_NULL
	*/
	Tbyte *Allocate(Tuint nNum);
	/* �ͷ��쳣������Դ	*/
	void Release(Tbyte* p);
	/* �����Ƿ�������*/
	Tbool IsFull() const { return ( m_nFree == 0); }
	/* �ж������Ƿ�Ϊ�� */
	Tbool IsEmpty() const { return (m_nUsed == 0); }

	Tbool GetFirstData(Tbyte *&pData, Tuint &nNum);
	Tbool GetNextData(Tbyte *&pData, Tuint &nNum);
private:
	Tuint	m_nLoopIndex;

	Tuint	m_nFree;		// ��������
	Tuint	m_nUsed;		// ��������
	Tuint	m_nQueue[XK_MAX_STREAM_NUM+1];
	Tbyte	*m_pPool;		// �쳣�����
};
/* ��Ϣ����:��ҪΪƽ̨�ڲ���Ϣ���Ĵ����ṩһ���̰߳�ȫ���� */
class CXKStream
{
public: 
	CXKStream();
	virtual ~CXKStream();
public:
	/* ����һ����Ϣ����ӿ� */
	Tbool InsertOutput(CXKStreamOutput *pOutput);
	/* �Ƴ�һ����Ϣ����ӿ� */
	Tbool RemoveOutput(CXKStreamOutput *pOutput);
	/* ����ı���Ϣ */
	void Print(int nLevel, const char *pData, int nCnt);
	/* �����������Ϣ */
	void Printb(int nLevel, const char *pTitle, int nTitleLen, const char *pData ,int len);
protected:
	enum	eLogTimeMode
	{
		TIME_TO_NULL	= 0
		, TIME_TO_YEAR
		, TIME_TO_MONTH
		, TIME_TO_HOUR
	};
	/* ��ӡϵͳʱ�� */
	int PrintTime(char *pTime);
private:
	typedef list<CXKStreamOutput *>		XK_LIST_OUTPUT;
	XK_LIST_OUTPUT		m_Items;		// �˿��б�

	int				m_iTimeFlag;	// ��ӡʱ��ĸ�ʽ

	CXKStdOut		m_stdout;		// Ĭ�ϵ���Ϣ������ӿ�
};

class CDJLogNetUnit : public CXKNetUnit
{
public:
	CDJLogNetUnit();
	virtual ~CDJLogNetUnit();
public:
	void RemoveAll();
	virtual Tbool OnLogon( Tuint nID, Tuint nType, CXKTcpSocket *pSocket, string &strDesc );
	virtual void OnAddClient(void* pWorkItem, void* pNull);
};

class CDJLogOut : public CXKStreamOutput
{
public:
	CDJLogOut();
	virtual ~CDJLogOut();
public:
	/* ������Ϣ������:���������е��쳣��Ϣ�������Ӧ�˿ڣ�����ջ����� */
	void Flush();
	/* �����һ����¼ */
	void Pop();
	/* �ж϶����Ƿ�Ϊ�� */
	Tbool IsEmpty();

	virtual void OnStreamOut(const char *pData,  const Tuint nCnt);
public:
	CDJLogNetUnit		m_tNetUnit;
protected:
	/* ͨ��Socket������Ϣ */
	void Send(const char *pData, int nCnt);
private:
	void Lock() { m_tMutex.Lock(); }
	void Unlock() { m_tMutex.Unlock(); }
private:
	CXKMutex			m_tMutex;
	CXKStreamQueue		m_tQueue;			// ��Ϣ����
};

class CXKWorkLog : public CXKWorkItemBase
{
public:
	CXKWorkLog();
	virtual ~CXKWorkLog();
public:
	CXKWorkLog(const CXKWorkLog& other);
	CXKWorkLog& operator =( const CXKWorkLog& other );

	Tbool Create( CDJLogOut* tLogOut );

	virtual void   DoWork(void* p);
	virtual void   Abort ();
protected:
	CDJLogOut* m_tLogOut;
	Tbool		m_bAbort;
private:

};

class CDJLogMgr
{
public:
	CDJLogMgr();
	virtual ~CDJLogMgr();
public:
	void Init();
	void Destroy();
	int AddServer(int nPort);
public:
	CXKThreadQueue	*m_tThreadQueue;
	CXKStream		m_tStream;
private:
	CDJLogOut		m_tLogOut;
	int				m_nServerPort;		// �����������˿�
	bool			m_bInit;
};

extern CDJLogMgr	*g_tLogMgr;

#endif //__LOG_H__

