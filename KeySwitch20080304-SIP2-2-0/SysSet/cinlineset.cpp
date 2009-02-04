// cinlineset.cpp : implementation file
//

#include "stdafx.h"
#include "..\KeySwitch.h"
#include "cinlineset.h"
#include "..\PublicDef.h"
#include "..\DBProcess.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInLineSetDlg dialog


CInLineSetDlg::CInLineSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CInLineSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInLineSetDlg)
	//}}AFX_DATA_INIT
}


void CInLineSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInLineSetDlg)
	DDX_Control(pDX, IDC_TARGET_USER, m_TargetUser);
	DDX_Control(pDX, IDC_QUICK_DIAL, m_QuickDial);
	DDX_Control(pDX, IDC_LINE_NO, m_LineNo);
	DDX_Control(pDX, IDC_ENABLE_LONG, m_EnableLong);
	DDX_Control(pDX, IDC_ENABLE, m_Enable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInLineSetDlg, CDialog)
	//{{AFX_MSG_MAP(CInLineSetDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInLineSetDlg message handlers

BOOL CInLineSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->SetWindowText("内线设置");
	pResult=false;		//默认状态
	if(!ReadLineInfo())
	{
		MessageBox("系统读取数据失败!!","Error",MB_OK);
		OnCancel();
		return false;
	}
	m_LineNo.EnableWindow(false);	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

bool  CInLineSetDlg::ReadLineInfo()
{
	/*读取数据库信息到界面*/
	char	mTemp[20];
	CDBProcess		objDb;
	char	mSQL[200];
	_variant_t 		mTemp1;
	char			mValue[20];

	strcpy(mSQL,"select * from objUserLine where flineno='");
	itoa(mDeviceNo,mTemp,10);
	strcat(mSQL,mTemp);
	strcat(mSQL,"'");
	if(!objDb.ConnectDB())	return false;
	if(!objDb.ExcueteQuery(mSQL))	return false;
	if(objDb.getRecCount()<=0)
	{
		MessageBox("该设备不存在!!","Error",MB_OK);
		return false;
	}
	itoa(mDeviceNo,mTemp,10);
	this->SetDlgItemText(IDC_CHANNEL_NO,mTemp);
	while(!objDb.pRs->EndOfFile)
	{
		mTemp1=(_variant_t)objDb.pRs->GetCollect("fLineNo");
		if(mTemp1.vt!=VT_NULL)
			strcpy(mValue,(char *)_bstr_t(mTemp1));
		else
			strcpy(mValue,"");
		this->SetDlgItemText(IDC_LINE_NO,mValue);
		mTemp1=(_variant_t)objDb.pRs->GetCollect("fPhoneNumber");
		if(mTemp1.vt!=VT_NULL)
			strcpy(mValue,(char *)_bstr_t(mTemp1));
		else
			strcpy(mValue,"");
		this->SetDlgItemText(IDC_PHONE_NUMBER,mValue);
		mTemp1=(_variant_t)objDb.pRs->GetCollect("fIsEnable");
		if(mTemp1.vt!=VT_NULL)
			strcpy(mValue,(char *)_bstr_t(mTemp1));
		else
			strcpy(mValue,"");
		if(strcmp(mValue,"启用"))
			m_Enable.SetCurSel(0);
		else
			m_Enable.SetCurSel(1);
		mTemp1=(_variant_t)objDb.pRs->GetCollect("fLongEnable");
		if(mTemp1.vt!=VT_NULL)
			strcpy(mValue,(char *)_bstr_t(mTemp1));
		else
			strcpy(mValue,"");
		if(strcmp(mValue,"开通"))
			m_EnableLong.SetCurSel(0);
		else
			m_EnableLong.SetCurSel(1);
		mTemp1=(_variant_t)objDb.pRs->GetCollect("fName");
		if(mTemp1.vt!=VT_NULL)
			strcpy(mValue,(char *)_bstr_t(mTemp1));
		else
			strcpy(mValue,"");
		this->SetDlgItemText(IDC_USER_NAME,mValue);
		mTemp1=(_variant_t)objDb.pRs->GetCollect("fIpAddress");
		if(mTemp1.vt!=VT_NULL)
			strcpy(mValue,(char *)_bstr_t(mTemp1));
		else
			strcpy(mValue,"");
		this->SetDlgItemText(IDC_IPADDRESS,mValue);
		mTemp1=(_variant_t)objDb.pRs->GetCollect("fMemo");
		if(mTemp1.vt!=VT_NULL)
			strcpy(mValue,(char *)_bstr_t(mTemp1));
		else
			strcpy(mValue,"");
		this->SetDlgItemText(IDC_MEMO,mValue);

        mTemp1=(_variant_t)objDb.pRs->GetCollect("fSpeedDial");
		if(mTemp1.vt!=VT_NULL)
			strcpy(mValue,(char *)_bstr_t(mTemp1));
		else
			strcpy(mValue,"");
		this->SetDlgItemText(IDC_QUICK_DIAL,mValue);	

        mTemp1=(_variant_t)objDb.pRs->GetCollect("fTargetUser");
		if(mTemp1.vt!=VT_NULL)
			strcpy(mValue,(char *)_bstr_t(mTemp1));
		else
			strcpy(mValue,"");
		this->SetDlgItemText(IDC_TARGET_USER,mValue);	

		objDb.pRs->MoveNext();
	}
	objDb.CloseDB();	
	return true;
}

bool CInLineSetDlg::SaveLineInfo()
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
	strcpy(mSQL,"select count(*) as fcount from objUserLine where flineno<>'");
	strcat(mSQL,mTemp);
	strcat(mSQL,"' and fphonenumber='");
	this->GetDlgItemText(IDC_PHONE_NUMBER,mValue1);
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
	strcpy(mSQL,"Update objUserLine set fphonenumber='");
	this->GetDlgItemText(IDC_PHONE_NUMBER,mValue1);
	strcat(mSQL,mValue1);
	strcat(mSQL,"',fIsEnable='");

	if(m_Enable.GetCurSel()==1)
		strcat(mSQL,"启用',fLongEnable='");
	else
		strcat(mSQL,"禁止',fLongEnable='");
	if(m_EnableLong.GetCurSel()==1)
		strcat(mSQL,"开通',fName='");
	else
		strcat(mSQL,"禁止',fName='");
	this->GetDlgItemText(IDC_USER_NAME,mValue1);
	strcat(mSQL,mValue1);

	strcat(mSQL,"',fSpeedDial='");
	this->GetDlgItemText(IDC_QUICK_DIAL,mValue1);
	strcat(mSQL,mValue1);

	strcat(mSQL,"',fTargetUser='");
	this->GetDlgItemText(IDC_TARGET_USER,mValue1);
	strcat(mSQL,mValue1);

	strcat(mSQL,"',fIpAddress='");
	this->GetDlgItemText(IDC_IPADDRESS,mValue1);
	strcat(mSQL,mValue1);
	strcat(mSQL,"',fmemo='");
	this->GetDlgItemText(IDC_MEMO,mValue1);
	strcat(mSQL,mValue1);
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

bool CInLineSetDlg::VerifyData()
{
	CString		mValue;
	this->GetDlgItemText(IDC_PHONE_NUMBER,mValue);
	if(strcmp(mValue,"")==-1)	return false;
	this->GetDlgItemText(IDC_USER_NAME,mValue);
	if(strcmp(mValue,"")==-1)	return false;
	this->GetDlgItemText(IDC_IPADDRESS,mValue);
	if(strcmp(mValue,"")==-1)	return false;
	return true;
}

void CInLineSetDlg::OnOK() 
{
	if(!VerifyData())
	{
		MessageBox("电话号码、用户名称和IP地址不能为空!","Error",MB_OK);
		return;
	}
	if(!SaveLineInfo())	return;
	pResult=true;
	this->CloseWindow();
	CDialog::OnOK();
}

void CInLineSetDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	pResult=false;
	this->CloseWindow();
	CDialog::OnCancel();
}
