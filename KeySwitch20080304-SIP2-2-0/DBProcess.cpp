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
		errormessage.Format("连接数据库失败!\r\n错误信息:%s",e.ErrorMessage()); 
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
功能：连接数据库
***********************************************************************/
BOOL	CDBProcess::ConnectDB()
{
	try
	{
		pConn->Open(pConnStr,"","",adConnectUnspecified);
		isConnect=true;		//标记已经连接上数据库
	}
	catch(_com_error e)
	{
		CString errormessage; 
		errormessage.Format("连接数据库失败!\r\n错误信息:%s",e.ErrorMessage()); 
		AfxMessageBox(errormessage);
		return false;
	}
	return	true;
}

/***********************************************************************
功能：执行一条更新数据的SQL语句
***********************************************************************/
BOOL	CDBProcess::ExcueteSQL(char *SQL)
{
	if(isConnect==false)	return false;	//未连接数据库
	try
	{
		pConn->Execute(SQL,0,adCmdText);
	}
	catch(_com_error e)
	{
		CString errormessage; 
		errormessage.Format("连接数据库失败!\r\n错误信息:%s",e.ErrorMessage()); 
		AfxMessageBox(errormessage);
		return false;
	}
	return true;
}

/***********************************************************************
功能：执行一个查询语句，并返回记录集合
***********************************************************************/
BOOL	CDBProcess::ExcueteQuery(char *SQL)		
{
	if(isConnect==false) return false;	//未连接数据库
	try
	{
		pRs=pConn->Execute(SQL,0,adCmdText);
	}
	catch(_com_error e)
	{
		CString errormessage; 
		errormessage.Format("连接数据库失败!\r\n错误信息:%s",e.ErrorMessage()); 
		AfxMessageBox(errormessage);
		return false;
	}
	return true;
}

/***********************************************************************
功能：关闭数据库连接
***********************************************************************/
void	CDBProcess::CloseDB()										
{
	pConn->Close();
	isConnect=false;
	return;
}

/***********************************************************************
功能：设置连接字符串
***********************************************************************/
void	CDBProcess::SetConnectStr(char *pSourceStr)						
{
	strcpy(pConnStr,pSourceStr);
	return;
}


/***********************************************************************
功能：获取记录数
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

