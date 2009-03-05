/****************************************************************************/
/* Copyright (c) 2004,陈文越												*/
/* 所有权利保留,未经作者允许,任何组织和个人不得将些代码用于商业盈利			*/
/* 用途,否则由此产生的一切法律责任由此组织或个人承担.						*/
/* 文件名:	xkkernel.h														*/
/* 作	者: 	陈文越                                                      */
/* 版	本: 	1.0                                                         */
/* 创建日期:		2006.10.30                                              */
/* 描述:	运行内核头文件													*/
/* 历史信息:                                                                */
/*		修改日期:															*/
/*		修改人:                                                             */
/*		修改内容:															*/
/****************************************************************************/
#if !defined(__XK_MSGQUEU_H)
#define __XK_MSGQUEU_H

#include "xkdef.h"
#include "xkthread.h"
#include "xkmsgdef.h"
/* 发送消息缓冲区 */
class CXKTransBuf
{
public:
	CXKTransBuf();
	virtual ~CXKTransBuf();
public:
	// 创建
	Tbool Create(Tuint nSize);
	// 释放资源
	void Destroy();
	// 得到消息缓冲区
	Tbyte* GetBuffer() { return m_pData; }
	// 得到缓冲区尺寸
	Tuint GetSize() const { return m_nSize; }
	
	// 设置消息数据,从网络收到的数据调用此函数
	void CopyData(const Tbyte *ptr, Tuint nLen);
	// 读取字段值
	Tbool GetKey(Tuint nPos, Tbyte &data);
	Tbool GetKey(Tuint nPos, Tint &data);
	Tbool GetKey(Tuint nPos, Tuint &data);
	Tbool GetKey(Tuint nPos, Tfloat &data);
	Tbool GetKey(Tuint nPos, Tdouble &data);
	Tbool GetKey(Tuint nPos, Tshort &data);
	Tbool GetKey(Tuint nPos, Tbool &data);
	Tbool GetKey(Tuint nPos, string &data);
	Tbool GetKey(Tuint nPos, Tbyte *data, Tuint nL);
//#if defined BIG_EDIAN
	// 设置字段值
	Tbool SetKey(Tuint nPos, Tbyte data);
	Tbool SetKey(Tuint nPos, Tint data);
	Tbool SetKey(Tuint nPos, Tuint data);
	Tbool SetKey(Tuint nPos, Tfloat data);
	Tbool SetKey(Tuint nPos, Tdouble data);
	Tbool SetKey(Tuint nPos, Tshort data);
	Tbool SetKey(Tuint nPos, Tbool data);
	Tbool SetKey(Tuint nPos, const string &data);
	Tbool SetKey(Tuint nPos, const Tbyte *data, Tuint nL);
protected:
	int IsLittleEndian() { Tint x = 1; return *((Tbyte *)&x);}
private:
	Tuint	m_nSize;		// 数据长度
	Tbyte	*m_pData;		// 消息数据
};

class CXKTransKeyInfo
{
public:
	CXKTransKeyInfo();
	virtual ~CXKTransKeyInfo();
public:
	Tuint	m_nKeyID;		// 字段ID
	Tbyte	m_nType;		// 数据类型
	Tuint	m_nPos;			// 数据位置
	Tuint	m_nLen;			// 数据长度
};

class CXKTransKey
{
public: 
	CXKTransKey();
	virtual ~CXKTransKey();
public:
	Tbool Create(Tuint nCnt);
	void Destroy();

	CXKTransKeyInfo *GetKeys(){return m_Keys;}
	Tuint GetCount() {return m_nCnt;}
	Tuint GetIndex() { return m_nIndex; }

	void IncIndex() { m_nIndex++; }
protected:
	CXKTransKeyInfo	*m_Keys;
	Tuint			m_nCnt;
	Tuint			m_nIndex;			// 构造消息时代表字段的索引
private:
	CXKtransKey(const CXKTransKey& other);
	CXKTransKey& operator=(CXKTransKey& other);
};

typedef map<Tuint, CXKTransKey>	XK_TRANSMSG_MAP;
/* 消息 */
class CXKTransData
{
public:
	CXKTransData();
	virtual ~CXKTransData();
public:
	enum TransDataType{TDT_BYTE, TDT_INT, TDT_UINT, TDT_FLOAT, TDT_DOUBLE, TDT_SHORT, TDT_BOOL, TDT_STRING, TDT_BYTEARRAY};
	// 设置消息ID
	Tbool SetMsgID(Tuint nMsgID);
	// 得到消息ID
	Tuint GetMSgID() { return m_nMsgID; }
	// 得到消息缓冲区
	Tbyte* GetBuffer() { return m_data.GetBuffer(); }
	// 得到缓冲区尺寸
	Tuint GetSize() const { return m_data.GetSize(); }

	// 设置消息数据,从网络收到的数据调用此函数
	Tbool CopyData(const Tbyte *ptr, Tuint nLen);
	// 读取字段值
	Tbool GetKey(Tuint nKeyID, Tbyte &data);
	Tbool GetKey(Tuint nKeyID, Tint &data);
	Tbool GetKey(Tuint nKeyID, Tuint &data);
	Tbool GetKey(Tuint nKeyID, Tfloat &data);
	Tbool GetKey(Tuint nKeyID, Tdouble &data);
	Tbool GetKey(Tuint nKeyID, Tshort &data);
	Tbool GetKey(Tuint nKeyID, Tbool &data);
	Tbool GetKey(Tuint nKeyID, string &data);
	Tbool GetKey(Tuint nKeyID, Tbyte *data, Tuint nLen);

	// 设置字段值
	Tbool SetKey(Tuint nKeyID, Tbyte data);
	Tbool SetKey(Tuint nKeyID, Tint data);
	Tbool SetKey(Tuint nKeyID, Tuint data);
	Tbool SetKey(Tuint nKeyID, Tfloat data);
	Tbool SetKey(Tuint nKeyID, Tdouble data);
	Tbool SetKey(Tuint nKeyID, Tshort data);
	Tbool SetKey(Tuint nKeyID, Tbool data);
	Tbool SetKey(Tuint nKeyID, const string &data);
	Tbool SetKey(Tuint nKeyID, const Tbyte *data, Tuint nLen);

	/* 安装消息解析结构体 */
	static Tbool InstallMsg(Tuint nMsgID, Tbyte nCnt);
	/* 安装消息键结构 */
	static Tint	AddKey(Tuint nMsgID, Tuint nKeyID, Tbyte nType, Tuint nLen = 0);
protected:
	static XK_TRANSMSG_MAP& GetTransKeyMap();
private:
	Tuint	m_nMsgID;		// 消息ID

	CXKTransBuf	m_data;		// 消息缓冲区
};

/* 消息队列 */
class CXKMsgQueue
{
	typedef queue< CXKTransData* > XK_MSG_QUEUE;
public:
	CXKMsgQueue();
	virtual ~CXKMsgQueue();
public:
	/* 弹出个消息 */
	CXKTransData* Pop();
	/* 压入个消息到消息队列 */
	void Push(CXKTransData	*pTransData);
	/* 判断队列是否为空 */
	Tbool IsEmpty();
private:
	void Destroy();
private:
	XK_MSG_QUEUE	m_queue;				// 消息队列
	CXKMutex		m_mutex;				// 同步链表互斥体
};

#endif // __XK_MSGQUEU_H