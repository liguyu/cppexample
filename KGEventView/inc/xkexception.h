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

#define XK_MAX_STREAM_LEN			512				// 异常信息最大长度
#define XK_MAX_STREAM_NUM			2000			// 最多保存的异常个数	

#define XK_BIT_PER_LEN					16

class CXKStream;
class CXKStreamOutput;
///////////////////////////////////////////////////////////////////////////////////////////////////////
/* 消息流端口类:负责将消息流导出,显示给用户 */
class CXKStreamOutput
{
public:
	CXKStreamOutput();
	virtual ~CXKStreamOutput();
public:
	/* 得到接口处理消息的级别 */
	Tint GetLevel() const { return m_nLevel; }
public:
	/* 导出消息 */
	virtual void OnStreamOut(const char *pData, const Tuint nCnt)=0;

	bool	IsAutoDel() { return m_bAutoDel; }
protected:
	Tint	m_nLevel;			// 接口处理消息级别
	bool	m_bAutoDel;
};
/* 标准输出流接口 */
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
	/* 初始化　*/
	Tbool Init();
	/* 复位 */
	void Reset();
	/* 得到异常存储位置
		成功返回链表中异常信息位置,
		失败返回XK_NULL
	*/
	Tbyte *Allocate(Tuint nNum);
	/* 释放异常链表资源	*/
	void Release(Tbyte* p);
	/* 链表是否已满　*/
	Tbool IsFull() const { return ( m_nFree == 0); }
	/* 判断链表是否为空 */
	Tbool IsEmpty() const { return (m_nUsed == 0); }

	Tbool GetFirstData(Tbyte *&pData, Tuint &nNum);
	Tbool GetNextData(Tbyte *&pData, Tuint &nNum);
private:
	Tuint	m_nLoopIndex;

	Tuint	m_nFree;		// 可用链表
	Tuint	m_nUsed;		// 已用链表
	Tuint	m_nQueue[XK_MAX_STREAM_NUM+1];
	Tbyte	*m_pPool;		// 异常缓存池
};
/* 消息流类:主要为平台内部消息流的处理提供一种线程安全机制 */
class CXKStream
{
public: 
	CXKStream();
	virtual ~CXKStream();
public:
	/* 插入一个消息输出接口 */
	Tbool InsertOutput(CXKStreamOutput *pOutput);
	/* 移除一个消息输出接口 */
	Tbool RemoveOutput(CXKStreamOutput *pOutput);
	/* 输出文本消息 */
	void Print(int nLevel, const char *pData, int nCnt);
	/* 输出二进制消息 */
	void Printb(int nLevel, const char *pTitle, int nTitleLen, const char *pData ,int len);
protected:
	enum	eLogTimeMode
	{
		TIME_TO_NULL	= 0
		, TIME_TO_YEAR
		, TIME_TO_MONTH
		, TIME_TO_HOUR
	};
	/* 打印系统时间 */
	int PrintTime(char *pTime);
private:
	typedef list<CXKStreamOutput *>		XK_LIST_OUTPUT;
	XK_LIST_OUTPUT		m_Items;		// 端口列表

	int				m_iTimeFlag;	// 打印时间的格式

	CXKStdOut		m_stdout;		// 默认的消息流处理接口
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
	/* 涮新消息缓冲区:将缓冲区中的异常信息输出到对应端口，并清空缓冲区 */
	void Flush();
	/* 输出第一条纪录 */
	void Pop();
	/* 判断队列是否为空 */
	Tbool IsEmpty();

	virtual void OnStreamOut(const char *pData,  const Tuint nCnt);
public:
	CDJLogNetUnit		m_tNetUnit;
protected:
	/* 通过Socket发送消息 */
	void Send(const char *pData, int nCnt);
private:
	void Lock() { m_tMutex.Lock(); }
	void Unlock() { m_tMutex.Unlock(); }
private:
	CXKMutex			m_tMutex;
	CXKStreamQueue		m_tQueue;			// 信息队列
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
	int				m_nServerPort;		// 服务器监听端口
	bool			m_bInit;
};

extern CDJLogMgr	*g_tLogMgr;

#endif //__LOG_H__

