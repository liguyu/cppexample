/****************************************************************************/
/* Copyright (c) 2004,����Խ												*/
/* ����Ȩ������,δ����������,�κ���֯�͸��˲��ý�Щ����������ҵӯ��			*/
/* ��;,�����ɴ˲�����һ�з��������ɴ���֯����˳е�.						*/
/* �ļ���:	xkkernel.h														*/
/* ��	��: 	����Խ                                                      */
/* ��	��: 	1.0                                                         */
/* ��������:		2006.10.30                                              */
/* ����:	�����ں�ͷ�ļ�													*/
/* ��ʷ��Ϣ:                                                                */
/*		�޸�����:															*/
/*		�޸���:                                                             */
/*		�޸�����:															*/
/****************************************************************************/
#if !defined(__XK_MSGQUEU_H)
#define __XK_MSGQUEU_H

#include "xkdef.h"
#include "xkthread.h"
#include "xkmsgdef.h"
/* ������Ϣ������ */
class CXKTransBuf
{
public:
	CXKTransBuf();
	virtual ~CXKTransBuf();
public:
	// ����
	Tbool Create(Tuint nSize);
	// �ͷ���Դ
	void Destroy();
	// �õ���Ϣ������
	Tbyte* GetBuffer() { return m_pData; }
	// �õ��������ߴ�
	Tuint GetSize() const { return m_nSize; }
	
	// ������Ϣ����,�������յ������ݵ��ô˺���
	void CopyData(const Tbyte *ptr, Tuint nLen);
	// ��ȡ�ֶ�ֵ
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
	// �����ֶ�ֵ
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
	Tuint	m_nSize;		// ���ݳ���
	Tbyte	*m_pData;		// ��Ϣ����
};

class CXKTransKeyInfo
{
public:
	CXKTransKeyInfo();
	virtual ~CXKTransKeyInfo();
public:
	Tuint	m_nKeyID;		// �ֶ�ID
	Tbyte	m_nType;		// ��������
	Tuint	m_nPos;			// ����λ��
	Tuint	m_nLen;			// ���ݳ���
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
	Tuint			m_nIndex;			// ������Ϣʱ�����ֶε�����
private:
	CXKtransKey(const CXKTransKey& other);
	CXKTransKey& operator=(CXKTransKey& other);
};

typedef map<Tuint, CXKTransKey>	XK_TRANSMSG_MAP;
/* ��Ϣ */
class CXKTransData
{
public:
	CXKTransData();
	virtual ~CXKTransData();
public:
	enum TransDataType{TDT_BYTE, TDT_INT, TDT_UINT, TDT_FLOAT, TDT_DOUBLE, TDT_SHORT, TDT_BOOL, TDT_STRING, TDT_BYTEARRAY};
	// ������ϢID
	Tbool SetMsgID(Tuint nMsgID);
	// �õ���ϢID
	Tuint GetMSgID() { return m_nMsgID; }
	// �õ���Ϣ������
	Tbyte* GetBuffer() { return m_data.GetBuffer(); }
	// �õ��������ߴ�
	Tuint GetSize() const { return m_data.GetSize(); }

	// ������Ϣ����,�������յ������ݵ��ô˺���
	Tbool CopyData(const Tbyte *ptr, Tuint nLen);
	// ��ȡ�ֶ�ֵ
	Tbool GetKey(Tuint nKeyID, Tbyte &data);
	Tbool GetKey(Tuint nKeyID, Tint &data);
	Tbool GetKey(Tuint nKeyID, Tuint &data);
	Tbool GetKey(Tuint nKeyID, Tfloat &data);
	Tbool GetKey(Tuint nKeyID, Tdouble &data);
	Tbool GetKey(Tuint nKeyID, Tshort &data);
	Tbool GetKey(Tuint nKeyID, Tbool &data);
	Tbool GetKey(Tuint nKeyID, string &data);
	Tbool GetKey(Tuint nKeyID, Tbyte *data, Tuint nLen);

	// �����ֶ�ֵ
	Tbool SetKey(Tuint nKeyID, Tbyte data);
	Tbool SetKey(Tuint nKeyID, Tint data);
	Tbool SetKey(Tuint nKeyID, Tuint data);
	Tbool SetKey(Tuint nKeyID, Tfloat data);
	Tbool SetKey(Tuint nKeyID, Tdouble data);
	Tbool SetKey(Tuint nKeyID, Tshort data);
	Tbool SetKey(Tuint nKeyID, Tbool data);
	Tbool SetKey(Tuint nKeyID, const string &data);
	Tbool SetKey(Tuint nKeyID, const Tbyte *data, Tuint nLen);

	/* ��װ��Ϣ�����ṹ�� */
	static Tbool InstallMsg(Tuint nMsgID, Tbyte nCnt);
	/* ��װ��Ϣ���ṹ */
	static Tint	AddKey(Tuint nMsgID, Tuint nKeyID, Tbyte nType, Tuint nLen = 0);
protected:
	static XK_TRANSMSG_MAP& GetTransKeyMap();
private:
	Tuint	m_nMsgID;		// ��ϢID

	CXKTransBuf	m_data;		// ��Ϣ������
};

/* ��Ϣ���� */
class CXKMsgQueue
{
	typedef queue< CXKTransData* > XK_MSG_QUEUE;
public:
	CXKMsgQueue();
	virtual ~CXKMsgQueue();
public:
	/* ��������Ϣ */
	CXKTransData* Pop();
	/* ѹ�����Ϣ����Ϣ���� */
	void Push(CXKTransData	*pTransData);
	/* �ж϶����Ƿ�Ϊ�� */
	Tbool IsEmpty();
private:
	void Destroy();
private:
	XK_MSG_QUEUE	m_queue;				// ��Ϣ����
	CXKMutex		m_mutex;				// ͬ����������
};

#endif // __XK_MSGQUEU_H