// DBProcess.h: interface for the CDBProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBPROCESS_H__C672465D_72F5_4209_ADAA_83F30D5B68ED__INCLUDED_)
#define AFX_DBPROCESS_H__C672465D_72F5_4209_ADAA_83F30D5B68ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CDBProcess  
{
public:
	_ConnectionPtr		pConn;												//ADO���Ӷ���
	_RecordsetPtr		pRs;												//���๫�õ�Ψһ��ѯ�������
	BOOL				isConnect;											//��ǵ�ǰ״̬���Ƿ����������ݿ�
	char				pConnStr[150];										//���ݿ����Ӵ�
	long				mRecordcount;										//��¼��

	CDBProcess();
	virtual ~CDBProcess();

	void	SetConnectStr(char *pSourceStr);								//�������Ӵ�
	BOOL	ConnectDB();													//�������ݿ�
	BOOL	ExcueteSQL(char *SQL);											//ִ��һ�����µ�SQL���
	BOOL	ExcueteQuery(char *SQL);										//ִ��һ����ѯ
	long	getRecCount();													//��ȡ��¼����
	void	CloseDB();														//�ر����ݿ�	
};

#endif // !defined(AFX_DBPROCESS_H__C672465D_72F5_4209_ADAA_83F30D5B68ED__INCLUDED_)
