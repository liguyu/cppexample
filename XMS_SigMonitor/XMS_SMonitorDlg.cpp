// XMS_SMonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XMS_SMonitor.h"
#include "XMS_SMonitorDlg.h"

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"
#include "ITPDataDefine.h"
#include "ITPDataDefine.h"
#include "XMS_SMonitorFunc.h"

//
extern CXMS_SMonitorApp theApp;
CXMS_SMonitorDlg* pdlg;
extern int g_StartWorkFlag;
extern int g_Pause;
//

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CXMS_SMonitorDlg dialog

CXMS_SMonitorDlg::CXMS_SMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXMS_SMonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMS_SMonitorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXMS_SMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMS_SMonitorDlg)
	DDX_Control(pDX, IDC_COMBO_LINE, m_RawLine);
	DDX_Control(pDX, IDC_LIST_RAWDATA_INFO, m_RawInfo);
	DDX_Control(pDX, IDC_BUTTON_START, m_BtnStart);
	DDX_Control(pDX, IDC_EDIT_PORT, m_Port);
	DDX_Control(pDX, IDC_EDIT_IPADDRESS, m_IPAddress);
	DDX_Control(pDX, IDC_LIST_DEVICE, m_DeviceInfo);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXMS_SMonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CXMS_SMonitorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	ON_CBN_EDITCHANGE(IDC_COMBO_LINE, OnEditchangeComboLine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMS_SMonitorDlg message handlers

BOOL CXMS_SMonitorDlg::OnInitDialog()
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

	pdlg = (CXMS_SMonitorDlg*)theApp.m_pMainWnd;
	
	InitSystem();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXMS_SMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CXMS_SMonitorDlg::OnPaint() 
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
HCURSOR CXMS_SMonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CXMS_SMonitorDlg::OnButtonStart() 
{
	// TODO: Add your control notification handler code here
	if (g_StartWorkFlag==0)
	{	
		m_BtnStart.SetWindowText("&Stop");
		g_StartWorkFlag = 1;
		
		if (g_Pause==0)
		{
		OpenDevice();
		}
	}
	else
	{
		m_BtnStart.SetWindowText("&Start");
		g_StartWorkFlag = 0;
		g_Pause = 1;
	}
}

void CXMS_SMonitorDlg::OnButtonExit() 
{
	// TODO: Add your control notification handler code here
	
	Set_ConfigInformation();
	g_StartWorkFlag = 0;

	ExitSystem();
	
	OnOK();
}

BOOL CXMS_SMonitorDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam==0x1b)) // ESC
	{
		return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CXMS_SMonitorDlg::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
	m_RawInfo.DeleteAllItems();
}

void CXMS_SMonitorDlg::OnEditchangeComboLine() 
{
	// TODO: Add your control notification handler code here	
}
