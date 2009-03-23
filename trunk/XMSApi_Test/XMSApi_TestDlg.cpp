// XMSApi_TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "XMSApi_TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"

#include "XMSApi_Test_Sub.h"
#include "XMSApi_Test_Func.h"

int g_iProtocol = -1;
int g_Select = -1;
int g_iSendRecv = -1;
/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_TestDlg dialog

CXMSApi_TestDlg::CXMSApi_TestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXMSApi_TestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMSApi_TestDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXMSApi_TestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMSApi_TestDlg)
	DDX_Control(pDX, IDC_COMBO_FAXTYPE, m_FaxType);
	DDX_Control(pDX, IDC_EDIT_DSPPORT, m_DspPort);
	DDX_Control(pDX, IDC_EDIT_DSPIP, m_DspIP);
	DDX_Control(pDX, IDC_COMBO_NowIndexList, m_ComboIndexList);
	DDX_Control(pDX, IDC_COMBO_ParamType, m_ComboParam);
	DDX_Control(pDX, IDC_LIST_MSG_HIDDEN, m_ListMsgHidden);
	DDX_Control(pDX, IDC_LIST_MSG, m_ListMsg);
	DDX_Control(pDX, IDC_LIST_DEVICE_INFO, m_ListDevInfo);
	DDX_Control(pDX, IDC_LIST_DEVICE_OPENED, m_ListDevOpened);
	DDX_Control(pDX, IDC_COMBO_MSG, m_ComboMsg);
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_ComboDevice);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXMSApi_TestDlg, CDialog)
	//{{AFX_MSG_MAP(CXMSApi_TestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_GetDeviceList, OnBUTTONGetDeviceList)
	ON_LBN_SELCHANGE(IDC_LIST_DEVICE_INFO, OnSelchangeListDeviceInfo)
	ON_BN_CLICKED(IDC_BUTTON_OpenDevice, OnBUTTONOpenDevice)
	ON_LBN_SELCHANGE(IDC_LIST_DEVICE_OPENED, OnSelchangeListDeviceOpened)
	ON_LBN_DBLCLK(IDC_LIST_DEVICE_OPENED, OnDblclkListDeviceOpened)
	ON_BN_CLICKED(IDC_BUTTON_Exchange, OnBUTTONExchange)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE, OnSelchangeComboDevice)
	ON_CBN_SELCHANGE(IDC_COMBO_MSG, OnSelchangeComboMsg)
	ON_BN_CLICKED(IDC_BUTTON_CloseDevice, OnBUTTONCloseDevice)
	ON_BN_CLICKED(IDC_BUTTON_ResetDevice, OnBUTTONResetDevice)
	ON_BN_CLICKED(IDC_BUTTON_GetDeviceState, OnBUTTONGetDeviceState)
	ON_BN_CLICKED(IDC_BUTTON_AlertCall, OnBUTTONAlertCall)
	ON_BN_CLICKED(IDC_BUTTON_AnswerCallIn, OnBUTTONAnswerCallIn)
	ON_BN_CLICKED(IDC_BUTTON_ClearCall, OnBUTTONClearCall)
	ON_BN_CLICKED(IDC_BUTTON_MakeCallOut, OnBUTTONMakeCallOut)
	ON_BN_CLICKED(IDC_CHECK_AutoScroll, OnCHECKAutoScroll)
	ON_BN_CLICKED(IDC_BUTTON_LinkDevice, OnBUTTONLinkDevice)
	ON_BN_CLICKED(IDC_BUTTON_UnlinkDevice, OnBUTTONUnlinkDevice)
	ON_BN_CLICKED(IDC_BUTTON_Play, OnBUTTONPlay)
	ON_BN_CLICKED(IDC_BUTTON_SendFax, OnBUTTONSendFax)
	ON_BN_CLICKED(IDC_BUTTON_StopSendFax, OnBUTTONStopSendFax)
	ON_BN_CLICKED(IDC_BUTTON_ReceiveFax, OnBUTTONReceiveFax)
	ON_BN_CLICKED(IDC_BUTTON_StopReceiveFax, OnBUTTONStopReceiveFax)
	ON_LBN_SELCHANGE(IDC_LIST_MSG, OnSelchangeListMsg)
	ON_BN_CLICKED(IDC_BUTTON_SelectSendFaxFile, OnBUTTONSelectSendFaxFile)
	ON_BN_CLICKED(IDC_BUTTON_SelectReceiveFaxFile, OnBUTTONSelectReceiveFaxFile)
	ON_BN_CLICKED(IDC_BUTTON_SelectPlayFile, OnBUTTONSelectPlayFile)
	ON_BN_CLICKED(IDC_BUTTON_SelectRecordFile, OnBUTTONSelectRecordFile)
	ON_LBN_DBLCLK(IDC_LIST_DEVICE_INFO, OnDblclkListDeviceInfo)
	ON_BN_CLICKED(IDC_BUTTON_My_StopPlay, OnBUTTONMyStopPlay)
	ON_BN_CLICKED(IDC_BUTTON_Record, OnBUTTONRecord)
	ON_BN_CLICKED(IDC_BUTTON_My_StopRecord, OnBUTTONMyStopRecord)
	ON_BN_CLICKED(IDC_BUTTON_JoinConf, OnBUTTONJoinConf)
	ON_BN_CLICKED(IDC_BUTTON_LeaveFromConf, OnBUTTONLeaveFromConf)
	ON_BN_CLICKED(IDC_BUTTON_ClearConf, OnBUTTONClearConf)
	ON_BN_CLICKED(IDC_BUTTON_ControlPlay, OnBUTTONControlPlay)
	ON_BN_CLICKED(IDC_BUTTON_PlayDetail, OnBUTTONPlayDetail)
	ON_BN_CLICKED(IDC_BUTTON_RecordDetail, OnBUTTONRecordDetail)
	ON_BN_CLICKED(IDC_BUTTON_ControlRecord, OnBUTTONControlRecord)
	ON_BN_CLICKED(IDC_BUTTON_SetParam, OnBUTTONSetParam)
	ON_CBN_SELCHANGE(IDC_COMBO_ParamType, OnSelchangeCOMBOParamType)
	ON_BN_CLICKED(IDC_BUTTON_InitPlayIndex, OnBUTTONInitPlayIndex)
	ON_BN_CLICKED(IDC_BUTTON_BuildPlayIndex, OnBUTTONBuildPlayIndex)
	ON_BN_CLICKED(IDC_BUTTON_SendIOData, OnBUTTONSendIOData)
	ON_BN_CLICKED(IDC_BUTTON_CONN, OnButtonConn)
	ON_BN_CLICKED(IDC_BUTTON_RECCSP, OnButtonReccsp)
	ON_BN_CLICKED(IDC_BUTTON_CTRLRECCSP, OnButtonCtrlreccsp)
	ON_BN_CLICKED(IDC_BUTTON_SelectRecordFile2, OnBUTTONSelectRecordFile2)
	ON_BN_CLICKED(IDC_BUTTON_CTXLINK, OnButtonCtxlink)
	ON_BN_CLICKED(IDC_BUTTON_CTXUNLINK, OnButtonCtxunlink)
	ON_BN_CLICKED(IDC_BUTTON_DISCONN, OnButtonDisconn)
	ON_BN_CLICKED(IDC_BUTTON_SendFaxT38, OnBUTTONSendFaxT38)
	ON_BN_CLICKED(IDC_BUTTON_StopSendFaxT38, OnBUTTONStopSendFaxT38)
	ON_BN_CLICKED(IDC_BUTTON_ReceiveFaxT38, OnBUTTONReceiveFaxT38)
	ON_BN_CLICKED(IDC_BUTTON_StopReceiveFaxT38, OnBUTTONStopReceiveFaxT38)
	ON_BN_CLICKED(IDC_BUTTON_PLAYCSP, OnButtonPlaycsp)
	ON_BN_CLICKED(IDC_BUTTON_GetBoardInfo, OnBUTTONGetBoardInfo)
	ON_BN_CLICKED(IDC_BUTTON_CTRLPLAYCSP, OnButtonCtrlplaycsp)
	ON_BN_CLICKED(IDC_RADIO_H323, OnRadioH323)
	ON_BN_CLICKED(IDC_RADIO_SIP, OnRadioSip)
	ON_BN_CLICKED(IDC_RADIO_OTHER, OnRadioOther)
	ON_BN_CLICKED(IDC_RADIO_VOIP, OnRadioVoip)
	ON_BN_CLICKED(IDC_RADIO_SEND, OnRadioSend)
	ON_BN_CLICKED(IDC_RADIO_RECV, OnRadioRecv)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_TestDlg message handlers

BOOL CXMSApi_TestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	char szBuf[100]={0};
	char szProName[100]={0};
	int nRet = 0;
	unsigned long u32Size = 0;
	HKEY hKey;

	nRet = RegOpenKey(HKEY_LOCAL_MACHINE,"Software\\DJXMS",&hKey);
	if(hKey != NULL)
	{
		u32Size = sizeof(szBuf);
		nRet = RegQueryValueEx(hKey,"Product",0,NULL,(unsigned char*)szBuf,&u32Size);
		if(nRet)
		{
			strcpy(szBuf,"DJKeygoe");
		}
	}
	else
	{
		strcpy(szBuf,"DJKeygoe");
	}
	RegCloseKey(hKey);

	sprintf(szProName,"Donjin %s APITest",szBuf);
	this->SetWindowText(szProName);

	InitSystem();

	//SetDlgItemText(IDC_EDIT_CALLEE_ADDR,"192.168.10.58");
	//SetDlgItemText(IDC_EDIT_CALLEE_PORT,"1730");

	((CButton *)GetDlgItem ( IDC_RADIO_H323 ))->SetCheck ( 1 );
	g_iProtocol = 0 ;

	((CButton *)GetDlgItem ( IDC_RADIO_OTHER ))->SetCheck ( 1 );
	g_Select = 0 ;

	((CButton *)GetDlgItem ( IDC_RADIO_SEND ))->SetCheck ( 1 );
	g_iSendRecv = 0 ;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXMSApi_TestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CXMSApi_TestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXMSApi_TestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CXMSApi_TestDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ExitSystem();	
}

void CXMSApi_TestDlg::OnBUTTONGetDeviceList() 
{
	// TODO: Add your control notification handler code here

	DoFunc_GetDeviceList();
	
}

void CXMSApi_TestDlg::OnSelchangeListDeviceInfo() 
{
	// TODO: Add your control notification handler code here

	SelectOpenDevice();
}

void CXMSApi_TestDlg::OnBUTTONOpenDevice() 
{
	// TODO: Add your control notification handler code here
	DoFunc_OpenDevice();
}

void CXMSApi_TestDlg::OnSelchangeListDeviceOpened() 
{
	// TODO: Add your control notification handler code here
	
	SelectOpenedDevice();
}

void CXMSApi_TestDlg::OnDblclkListDeviceOpened() 
{
	// TODO: Add your control notification handler code here

}

void CXMSApi_TestDlg::OnBUTTONExchange() 
{
	// TODO: Add your control notification handler code here

	ExchangeDeviceIndex();
	
}

void CXMSApi_TestDlg::OnSelchangeComboDevice() 
{
	// TODO: Add your control notification handler code here
	SelectComboDevice();

}

void CXMSApi_TestDlg::OnSelchangeComboMsg() 
{
	// TODO: Add your control notification handler code here
	
	SelectComboMsg();
}

void CXMSApi_TestDlg::OnBUTTONCloseDevice() 
{
	// TODO: Add your control notification handler code here
	DoFunc_CloseDevice();
	
}

void CXMSApi_TestDlg::OnBUTTONResetDevice() 
{
	// TODO: Add your control notification handler code here

	DoFunc_ResetDevice();
	
}

void CXMSApi_TestDlg::OnBUTTONGetDeviceState() 
{
	// TODO: Add your control notification handler code here
	
	if (g_Select == 0)
		DoFunc_GetDeviceState();
	else
		DoFunc_GetDeviceStateVoIP();
}

void CXMSApi_TestDlg::OnBUTTONAlertCall() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_AlertCall();
}

void CXMSApi_TestDlg::OnBUTTONAnswerCallIn() 
{
	// TODO: Add your control notification handler code here

	DoFunc_AnswerCallIn();
	
}

void CXMSApi_TestDlg::OnBUTTONClearCall() 
{
	// TODO: Add your control notification handler code here

	DoFunc_ClearCall();
	
}

void CXMSApi_TestDlg::OnBUTTONMakeCallOut() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_MakeCallOut();
}

extern int	cfg_iAutoScroll;
void CXMSApi_TestDlg::OnCHECKAutoScroll() 
{
	// TODO: Add your control notification handler code here
	
	cfg_iAutoScroll = ((CButton *)GetDlgItem ( IDC_CHECK_AutoScroll ))->GetCheck();
}

void CXMSApi_TestDlg::OnBUTTONLinkDevice() 
{
	// TODO: Add your control notification handler code here

	DoFunc_LinkDevice();
}

void CXMSApi_TestDlg::OnBUTTONUnlinkDevice() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_UnlinkDevice();
}

void CXMSApi_TestDlg::OnBUTTONPlay() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_Play();
}


void CXMSApi_TestDlg::OnBUTTONPlayDetail() 
{
	// TODO: Add your control notification handler code here
	DoFunc_Play(true);
	
}

void CXMSApi_TestDlg::OnBUTTONSendFax() 
{
	// TODO: Add your control notification handler code here

	DoFunc_SendFax();
}

void CXMSApi_TestDlg::OnBUTTONStopSendFax() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_StopSendFax();
}

void CXMSApi_TestDlg::OnBUTTONReceiveFax() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_ReceiveFax();
}

void CXMSApi_TestDlg::OnBUTTONStopReceiveFax() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_StopReceiveFax();
}

void CXMSApi_TestDlg::OnSelchangeListMsg() 
{
	// TODO: Add your control notification handler code here

	int		iRow = 0;
	char	TmpStr[256];
	
	iRow = m_ListMsg.GetCurSel ();

	m_ListMsg.GetText ( iRow, TmpStr );

	GetDlgItem ( IDC_EDIT_MsgDetail )->SetWindowText ( TmpStr );

}	


void CXMSApi_TestDlg::OnBUTTONSelectSendFaxFile() 
{
	// TODO: Add your control notification handler code here

	SelectSendFaxFile ();

}

void CXMSApi_TestDlg::OnBUTTONSelectReceiveFaxFile() 
{
	// TODO: Add your control notification handler code here
	
	SelectReceiveFaxFile ();
}

void CXMSApi_TestDlg::OnBUTTONSelectPlayFile() 
{
	// TODO: Add your control notification handler code here
	
	SelectPlayFile ();
}

void CXMSApi_TestDlg::OnBUTTONSelectRecordFile() 
{
	// TODO: Add your control notification handler code here
	
	SelectRecordFile(IDC_EDIT1_RECORD);
}

void CXMSApi_TestDlg::OnDblclkListDeviceInfo() 
{
	// TODO: Add your control notification handler code here
	
	SelectOpenDevice();

	DoFunc_OpenDevice();
}

void CXMSApi_TestDlg::OnBUTTONMyStopPlay() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_MyStopPlay();
}

void CXMSApi_TestDlg::OnBUTTONRecord() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_Record();
}

void CXMSApi_TestDlg::OnBUTTONRecordDetail() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_Record(true);
}

void CXMSApi_TestDlg::OnBUTTONMyStopRecord() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_MyStopRecord();
}

void CXMSApi_TestDlg::OnBUTTONJoinConf() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_JoinToConf();
}

void CXMSApi_TestDlg::OnBUTTONLeaveFromConf() 
{
	// TODO: Add your control notification handler code here

	DoFunc_LeaveFromConf();
}

void CXMSApi_TestDlg::OnBUTTONClearConf() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_ClearConf();
}

void CXMSApi_TestDlg::OnBUTTONControlPlay() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_ControlPlay();
}

void CXMSApi_TestDlg::OnBUTTONControlRecord() 
{
	// TODO: Add your control notification handler code here
		
	DoFunc_ControlRecord();
}

void CXMSApi_TestDlg::OnBUTTONSetParam() 
{
	// TODO: Add your control notification handler code here

	DoFunc_SetParam();
}

extern int		iComboParam;
void CXMSApi_TestDlg::OnSelchangeCOMBOParamType() 
{
	// TODO: Add your control notification handler code here
	
	iComboParam = m_ComboParam.GetCurSel ( ); 
}

void CXMSApi_TestDlg::OnBUTTONInitPlayIndex() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_InitPlayIndex();
}

void CXMSApi_TestDlg::OnBUTTONBuildPlayIndex() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_BuildPlayIndex();
}

void CXMSApi_TestDlg::OnBUTTONSendIOData() 
{
	// TODO: Add your control notification handler code here
	
	DoFunc_SendIOData();
}

void CXMSApi_TestDlg::OnButtonConn() 
{
	DoFunc_ConnDsp();
}

void CXMSApi_TestDlg::OnButtonDisconn() 
{
	DoFunc_DisConnDsp();
}

void CXMSApi_TestDlg::OnButtonReccsp() 
{
	DoFunc_RecCSP();	
}

void CXMSApi_TestDlg::OnButtonCtrlreccsp() 
{
	DoFunc_CtrlRecCSP();	
}

void CXMSApi_TestDlg::OnBUTTONSelectRecordFile2() 
{
	SelectRecordFile(IDC_EDIT_RECCSP_FILE);		
}

void CXMSApi_TestDlg::OnButtonCtxlink() 
{
	DoFunc_CTXLink();	
}

void CXMSApi_TestDlg::OnButtonCtxunlink() 
{
	DoFunc_CTXUnLink();
}

void CXMSApi_TestDlg::OnBUTTONSendFaxT38() 
{
	DoFunc_StartFaxT38();
	//DoFunc_SendFaxT38();		
}

void CXMSApi_TestDlg::OnBUTTONStopSendFaxT38() 
{
	DoFunc_StopSendFaxT38();		
}

void CXMSApi_TestDlg::OnBUTTONReceiveFaxT38() 
{
	DoFunc_ReceiveFaxT38();	
}

void CXMSApi_TestDlg::OnBUTTONStopReceiveFaxT38() 
{
	DoFunc_StopReceiveFaxT38();		
}

void CXMSApi_TestDlg::OnButtonPlaycsp() 
{
	DoFunc_PlayCSP();		
}

void CXMSApi_TestDlg::OnBUTTONGetBoardInfo() 
{
	DoFunc_GetBoardInfo();	
}

void CXMSApi_TestDlg::OnButtonCtrlplaycsp() 
{
	DoFunc_ControlPlayCSP();	
}

void CXMSApi_TestDlg::OnBUTTONGetDeviceStateVoIP() 
{
	// TODO: Add your control notification handler code here
	DoFunc_GetDeviceStateVoIP();
}



void CXMSApi_TestDlg::OnRadioH323() 
{
	// TODO: Add your control notification handler code here
	g_iProtocol = 0;
}

void CXMSApi_TestDlg::OnRadioSip() 
{
	// TODO: Add your control notification handler code here
	g_iProtocol = 1;
}

void CXMSApi_TestDlg::OnRadioOther() 
{
	// TODO: Add your control notification handler code here
	//g_Select = 0;
}

void CXMSApi_TestDlg::OnRadioVoip() 
{
	// TODO: Add your control notification handler code here
	//g_Select = 1;
	
}

void CXMSApi_TestDlg::OnRadioSend() 
{
	// TODO: Add your control notification handler code here
	g_iSendRecv = 0;
}

void CXMSApi_TestDlg::OnRadioRecv() 
{
	// TODO: Add your control notification handler code here
	g_iSendRecv = 1;
	
}
