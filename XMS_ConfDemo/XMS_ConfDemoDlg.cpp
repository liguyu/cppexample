// XMS_ConfDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "XMS_ConfDemo.h"
#include "XMS_ConfDemoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "XMS_ConfDemo_Sub.h"

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
// CXMS_ConfDemoDlg dialog

CXMS_ConfDemoDlg::CXMS_ConfDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CXMS_ConfDemoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CXMS_ConfDemoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CXMS_ConfDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CXMS_ConfDemoDlg)
	DDX_Control(pDX, IDC_LIST_IP, m_ListIP);
	DDX_Control(pDX, IDC_LIST_CONF, m_ListConf);
	DDX_Control(pDX, IDC_LIST2, m_ListPcm);
	DDX_Control(pDX, IDC_LIST3, m_ListMsg);
	DDX_Control(pDX, IDC_LIST1, m_ListMain);
	DDX_Control(pDX, IDC_LIST_COUNT, m_ListCount);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CXMS_ConfDemoDlg, CDialog)
	//{{AFX_MSG_MAP(CXMS_ConfDemoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_ReOpen, OnBUTTONReOpen)
	ON_BN_CLICKED(IDC_BUTTON_Remove, OnBUTTONRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXMS_ConfDemoDlg message handlers

BOOL CXMS_ConfDemoDlg::OnInitDialog()
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

	sprintf(szProName,"Donjin %s Conference Demo",szBuf);
	this->SetWindowText(szProName);

	InitSystem();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CXMS_ConfDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CXMS_ConfDemoDlg::OnPaint() 
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
HCURSOR CXMS_ConfDemoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CXMS_ConfDemoDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	ExitSystem();	
}

void CXMS_ConfDemoDlg::OnBUTTONReOpen() 
{
	// TODO: Add your control notification handler code here
	
	ReOpen_AllDevice ();
}

void CXMS_ConfDemoDlg::OnBUTTONRemove() 
{
	// TODO: Add your control notification handler code here
	
	Remove_OneDsp();
}
