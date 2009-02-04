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
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COutLineSetDlg)
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	DDX_Control(pDX, IDC_OUT_LINE_NO, m_LineNo);
	//}}AFX_DATA_MAP
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
		MessageBox("系统读取设备信息失败!!","Error",MB_OK);
		OnCancel();
		return false;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
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
	if(objDb.pRs->EndOfFile)
	{
		MessageBox("该设备不存在!!","Error",MB_OK);
		return false;
	}
	itoa(mDeviceNo,mTemp,10);
	this->SetDlgItemText(IDC_OUT_CHANNEL_NO,mTemp);
	while(!objDb.pRs->EndOfFile)
	{
		mTemp1=(_variant_t)objDb.pRs->GetCollect("fLineNo");
		if(mTemp1.vt!=VT_NULL)
			strcpy(mValue,(char *)_bstr_t(mTemp1));
		else
			strcpy(mValue,"");
		this->SetDlgItemText(IDC_OUT_LINE_NO,mValue);

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
		mTemp1=(_variant_t)objDb.pRs->GetCollect("fisenable");
		if(mTemp1.vt!=VT_NULL)
			strcpy(mValue,(char *)_bstr_t(mTemp1));
		else
			strcpy(mValue,"");
		if(strcmp(mValue,"启用")==0)
			m_Enable.SetCurSel(1);
		else
			m_Enable.SetCurSel(0);
		objDb.pRs->MoveNext();
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
	strcat(mSQL,"' ");
	strcat(mSQL,",fisenable='");
	if(m_Enable.GetCurSel()==1)
		strcat(mSQL,"启用");
	else
		strcat(mSQL,"禁用");
	strcat(mSQL,"' where flineno='");
	itoa(mDeviceNo,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"' and fdspno='");
	itoa(mDspNo,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"'");
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
	if(strcmp(mValue,"")==-1)	return false;
	return true;
}


void COutLineSetDlg::OnOK() 
{
	// TODO: Add extra validation here
	if(!VerifyData())
	{
		MessageBox("电话号码不能为空!","Error",MB_OK);
		return;
	}
	if(!SaveLineInfo())	return;
	pResult=true;
	this->CloseWindow();	
	CDialog::OnOK();
}

void COutLineSetDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	pResult=false;
	this->CloseWindow();
	CDialog::OnCancel();
}
