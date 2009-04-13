// XMS_HizDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XMS_HizDemo.h"
#include "XMS_HizDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "XMS_HizDemo_Sub.H"

DJ_U8                   g_u8IsStartFlag = FALSE;
long		            g_iTestStartTime = 0;

extern int				cfg_iVoiceRule;

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
// CXMS_DemoDlg dialog

CXMS_DemoDlg::CXMS_DemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXMS_DemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMS_DemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXMS_DemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMS_DemoDlg)
	DDX_Control(pDX, IDC_BUTTON_ReOpen, m_BtnStart);
	DDX_Control(pDX, IDC_LIST_COUNT, m_ListCount);
	DDX_Control(pDX, IDC_LIST3, m_ListMsg);
	DDX_Control(pDX, IDC_LIST2, m_ListPcm);
	DDX_Control(pDX, IDC_LIST1, m_ListMain);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXMS_DemoDlg, CDialog)
	//{{AFX_MSG_MAP(CXMS_DemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ReOpen, OnBUTTONReOpen)
	ON_BN_CLICKED(IDC_BUTTON_Remove, OnBUTTONRemove)
	ON_BN_CLICKED(IDC_RADIO_Fix, OnRADIOFix)
	ON_BN_CLICKED(IDC_RADIO_SameModule, OnRADIOSameModule)
	ON_BN_CLICKED(IDC_RADIO_MostFreeFist, OnRADIOMostFreeFist)
	ON_BN_CLICKED(IDC_RADIO_Seq, OnRADIOSeq)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMS_DemoDlg message handlers

BOOL CXMS_DemoDlg::OnInitDialog()
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
	
	//write programme name
	char szProName[100]={0};
	strcpy(szProName,"DJKeygoe Record System Demo");
	this->SetWindowText(szProName);

	InitSystem();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXMS_DemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CXMS_DemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CXMS_DemoDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ExitSystem();
	
}

void CXMS_DemoDlg::OnBUTTONReOpen() 
{

	g_u8IsStartFlag = TRUE;
	g_iTestStartTime = GetTickCount();

	CString str;
	CTime tm;					
	tm=CTime::GetCurrentTime();
	str=tm.Format("%Y%m%d-%H:%M:%S");
	this->GetDlgItem(IDC_BUTTON_Start)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BUTTON_Start)->SetWindowText(str);


}
	

void CXMS_DemoDlg::OnBUTTONRemove() 
{
	// TODO: Add your control notification handler code here
	
	Remove_OneDsp();
}

void CXMS_DemoDlg::OnRADIOFix() 
{
	// TODO: Add your control notification handler code here
	cfg_iVoiceRule = 0;
}

void CXMS_DemoDlg::OnRADIOSameModule() 
{
	// TODO: Add your control notification handler code here
		cfg_iVoiceRule = 1;

}

void CXMS_DemoDlg::OnRADIOMostFreeFist() 
{
	// TODO: Add your control notification handler code here
		cfg_iVoiceRule = 2;

}

void CXMS_DemoDlg::OnRADIOSeq() 
{
	// TODO: Add your control notification handler code here
		cfg_iVoiceRule = 3;
}
