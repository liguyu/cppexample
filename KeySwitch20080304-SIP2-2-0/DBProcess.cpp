// DBProcess.cpp: implementation of the CDBProcess class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "KeySwitch.h"
#include "DBProcess.h"
#include "KeySwitchDlg.h"
#include "PublicDef.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern char		pDbConnStr[150];

CDBProcess::CDBProcess()
{
	CoInitialize(NULL);
	try
	{
		pConn=_ConnectionPtr("ADODB.Connection");
		pRs.CreateInstance(__uuidof(Recordset));
	}
	catch(_com_error e)
	{
		CString errormessage; 
		errormessage.Format("�������ݿ�ʧ��!\r\n������Ϣ:%s",e.ErrorMessage()); 
		AfxMessageBox(errormessage);
	}
	strcpy(pConnStr,pDbConnStr);
	isConnect=false;			
}

CDBProcess::~CDBProcess()
{
	if(isConnect)
	{
		pConn->Close();								
	}
}

/***********************************************************************
���ܣ��������ݿ�
***********************************************************************/
BOOL	CDBProcess::ConnectDB()
{
	try
	{
		pConn->Open(pConnStr,"","",adConnectUnspecified);
		isConnect=true;		//����Ѿ����������ݿ�
	}
	catch(_com_error e)
	{
		CString errormessage; 
		errormessage.Format("�������ݿ�ʧ��!\r\n������Ϣ:%s",e.ErrorMessage()); 
		AfxMessageBox(errormessage);
		return false;
	}
	return	true;
}

/***********************************************************************
���ܣ�ִ��һ���������ݵ�SQL���
***********************************************************************/
BOOL	CDBProcess::ExcueteSQL(char *SQL)
{
	if(isConnect==false)	return false;	//δ�������ݿ�
	try
	{
		pConn->Execute(SQL,0,adCmdText);
	}
	catch(_com_error e)
	{
		CString errormessage; 
		errormessage.Format("�������ݿ�ʧ��!\r\n������Ϣ:%s",e.ErrorMessage()); 
		AfxMessageBox(errormessage);
		return false;
	}
	return true;
}

/***********************************************************************
���ܣ�ִ��һ����ѯ��䣬�����ؼ�¼����
***********************************************************************/
BOOL	CDBProcess::ExcueteQuery(char *SQL)		
{
	if(isConnect==false) return false;	//δ�������ݿ�
	try
	{
		pRs=pConn->Execute(SQL,0,adCmdText);
	}
	catch(_com_error e)
	{
		CString errormessage; 
		errormessage.Format("�������ݿ�ʧ��!\r\n������Ϣ:%s",e.ErrorMessage()); 
		AfxMessageBox(errormessage);
		return false;
	}
	return true;
}

/***********************************************************************
���ܣ��ر����ݿ�����
***********************************************************************/
void	CDBProcess::CloseDB()										
{
	pConn->Close();
	isConnect=false;
	return;
}

/***********************************************************************
���ܣ����������ַ���
***********************************************************************/
void	CDBProcess::SetConnectStr(char *pSourceStr)						
{
	strcpy(pConnStr,pSourceStr);
	return;
}


/***********************************************************************
���ܣ���ȡ��¼��
***********************************************************************/
long	CDBProcess::getRecCount()
{
	long	mResult;
	mResult=0;
	pRs->MoveFirst();
	while(!pRs->EndOfFile)
	{
		mResult++;
		pRs->MoveNext();
	}
	pRs->MoveFirst();
	return mResult;
}

