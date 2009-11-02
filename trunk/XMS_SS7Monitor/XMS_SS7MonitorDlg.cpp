// XMS_SS7MonitorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XMS_SS7Monitor.h"
#include "XMS_SS7MonitorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "XMS_SS7Monitor_Sub.H"

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
// CXMS_SS7MonitorDlg dialog

CXMS_SS7MonitorDlg::CXMS_SS7MonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXMS_SS7MonitorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMS_SS7MonitorDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXMS_SS7MonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMS_SS7MonitorDlg)
	DDX_Control(pDX, IDC_COMBO_COMTYPE, m_ComboComType);
	DDX_Control(pDX, IDC_COMBO_TUPMSGTYPE, m_ComboTUPMsgType);
	DDX_Control(pDX, IDC_COMBO_ISUPMSGTYPE, m_ComboISUPMsgType);
	DDX_Control(pDX, IDC_LIST_TRUNK, m_ListTrunk);
	DDX_Control(pDX, IDC_LIST_PCM, m_ListPCM);
	DDX_Control(pDX, IDC_LIST_MSG, m_ListMsg);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXMS_SS7MonitorDlg, CDialog)
	//{{AFX_MSG_MAP(CXMS_SS7MonitorDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEND, OnButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_SENDSIGMSG, OnButtonSendsigmsg)
	ON_BN_CLICKED(IDC_BUTTON_SENDRAWDATA, OnButtonSendrawdata)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMS_SS7MonitorDlg message handlers

BOOL CXMS_SS7MonitorDlg::OnInitDialog()
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
	this->SetWindowText("DONJIN Keygoe SS7 Monitor Tool");
	
	InitSystem();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXMS_SS7MonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CXMS_SS7MonitorDlg::OnPaint() 
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
HCURSOR CXMS_SS7MonitorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CXMS_SS7MonitorDlg::OnButtonSend() 
{
	
	SendRawDataToSS7Link();
}

void CXMS_SS7MonitorDlg::OnButtonSendsigmsg() 
{
	SendSigMsgToTrunk();
}


void CXMS_SS7MonitorDlg::OnButtonSendrawdata() 
{

	SendRawDataToSS7Link2();	
}
