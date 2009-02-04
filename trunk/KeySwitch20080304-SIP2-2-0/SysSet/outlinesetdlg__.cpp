// outlinesetdlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\KeySwitch.h"
#include "outlinesetdlg.h"
#include "..\DBProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COutLineSetDlg dialog


COutLineSetDlg::COutLineSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COutLineSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COutLineSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void COutLineSetDlg::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_INIT(COutLineSetDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

BEGIN_MESSAGE_MAP(COutLineSetDlg, CDialog)
//{{AFX_MSG_MAP(COutLineSetDlg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COutLineSetDlg message handlers

BOOL COutLineSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText("外线设置");
	pResult=false;
	m_LineNo.EnableWindow(false);
	if(!ReadLineInfo())
	{
		MessageBox("系统读取数据失败!!","Error",MB_OK);
		this->CloseWindow();
	}
	return TRUE; 
}

bool  COutLineSetDlg::ReadLineInfo()
{
	/*读取数据库信息到界面*/
	char	mTemp[20];
	CDBProcess		objDb;
	char	mSQL[200];
	_variant_t 		mTemp1;
	char			mValue[20];

	strcpy(mSQL,"select * from objTrunkLine where flineno='");
	itoa(mDeviceNo,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"'");
	if(!objDb.ConnectDB())	return false;
	if(!objDb.ExcueteQuery(mSQL))	return false;
	if(!objDb.pRs->EndOfFile)
	{
		MessageBox("该设备不存在!!","Error",MB_OK);
		return false;
	}
	itoa(mDeviceNo,mTemp,10);
	this->SetDlgItemText(IDC_OUT_CHANNEL_NO,mTemp);
	while(!objDb.pRs->EndOfFile)
	{
		mTemp1=(_variant_t)objDb.pRs->GetCollect("fphonenumber");
		if(mTemp1.vt!=VT_NULL)
			strcpy(mValue,(char *)_bstr_t(mTemp1));
		else
			strcpy(mValue,"");
		this->SetDlgItemText(IDC_OUT_PHONE_NUMBER,mValue);
		mTemp1=(_variant_t)objDb.pRs->GetCollect("fMemo");
		if(mTemp1.vt!=VT_NULL)
			strcpy(mValue,(char *)_bstr_t(mTemp1));
		else
			strcpy(mValue,"");
		this->SetDlgItemText(IDC_OUT_MEMO,mValue);
	}
	objDb.CloseDB();	
	return true;
}

bool COutLineSetDlg::SaveLineInfo()
{
	/*保存界面信息到数据库*/
	char	mSQL[300];
	CDBProcess	objDb;
	char	mTemp[20];
	_variant_t 		mTemp1;
	char			mValue[20];
	int			mCount;
	CString		mValue1;
	
	itoa(mDeviceNo,mTemp,10);
	strcpy(mSQL,"select count(*) as fcount from objTrunkLine where flineno<>'");
	strcat(mSQL,mTemp);
	strcat(mSQL,"' and fphonenumber='");
	this->GetDlgItemText(IDC_OUT_PHONE_NUMBER,mValue1);
	strcat(mSQL,mValue1);
	strcat(mSQL,"'");
	if(!objDb.ConnectDB())	return false;
	if(!objDb.ExcueteQuery(mSQL))
	{
		MessageBox("系统更新数据失败!!","Error",MB_OK);
		return false;
	}
	mTemp1=(_variant_t)objDb.pRs->GetCollect("fcount");
	if(mTemp1.vt!=VT_NULL)
	{
		strcpy(mValue,(char *)_bstr_t(mTemp1));
		mCount=atoi(mValue);
	}
	else
	{
		strcpy(mValue,"");
		mCount=0;
	}
	if(mCount>0)
	{
		MessageBox("对不起!该号码已经分配!","Error",MB_OK);
		return false;
	}
	strcpy(mSQL,"Update objTrunkLine set fphonenumber='");
	this->GetDlgItemText(IDC_OUT_PHONE_NUMBER,mValue1);
	strcat(mSQL,mValue1);
	strcat(mSQL,"',fmemo='");
	this->GetDlgItemText(IDC_OUT_MEMO,mValue1);
	strcat(mSQL,mValue1);
	strcat(mSQL,"' where flineno='");
	itoa(mDeviceNo,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL," and fdspno='");
	itoa(mDspNo,mTemp,10);
	strcat(mSQL,mTemp);
	if(!objDb.ExcueteSQL(mSQL))
	{
		MessageBox("系统更新数据失败!!","Error",MB_OK);
		return false;
	}
	return true;
}

bool COutLineSetDlg::VerifyData()
{
	CString		mValue;
	this->GetDlgItemText(IDC_OUT_PHONE_NUMBER,mValue);
	if(strcmp(mValue,""))	return false;
	return true;
}

