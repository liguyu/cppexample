// ButtonTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ButtonTest.h"
#include "ButtonTestDlg.h"

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
// CButtonTestDlg dialog

CButtonTestDlg::CButtonTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CButtonTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CButtonTestDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CButtonTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CButtonTestDlg)
	DDX_Control(pDX, IDC_BUTTON_SYSSET, m_BtnSysSet);
	DDX_Control(pDX, IDC_BUTTON_STARTTEST, m_BtnStartTest);
	DDX_Control(pDX, IDC_BUTTON_PASSWORD, m_BtnPassword);
	DDX_Control(pDX, IDC_BUTTON_PARAMIMPORT, m_BtnParamImport);
	DDX_Control(pDX, IDC_BUTTON_PARAMEDIT, m_BtnParamEdit);
	DDX_Control(pDX, IDC_BUTTON_HELP, m_BtnHelp);
	DDX_Control(pDX, IDC_BUTTON_EXIT, m_BtnExit);
	DDX_Control(pDX, IDC_BUTTON1, m_btn);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CButtonTestDlg, CDialog)
	//{{AFX_MSG_MAP(CButtonTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON_STARTTEST, OnButtonStarttest)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonTestDlg message handlers

BOOL CButtonTestDlg::OnInitDialog()
{
	short	shBtnColor = 30;

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
	
	// Standard button
	m_btn.SetIcon(IDI_ICON1);
	m_btn.SetFlat(FALSE);

	m_BtnParamEdit.SetIcon(IDI_ICON_PARAMEDIT);
	//m_BtnParamEdit.SetFlat(FALSE);

	m_BtnParamImport.SetIcon(IDI_ICON_PARAMIMPORT);

	m_BtnSysSet.SetIcon(IDI_ICON_SYSSET);

	m_BtnExit.SetIcon(IDI_ICON_EXIT);

	m_BtnStartTest.SetIcon(IDI_ICON_STARTTEST);

	m_BtnHelp.SetIcon(IDI_ICON_HELP);

	m_BtnPassword.SetIcon(IDI_ICON_CHANGEPASSWORD);


	//m_btn.SetIcon(IDI_ICON1, (int)BTNST_AUTO_GRAY);
	//m_btn.OffsetColor(CButtonST::BTNST_COLOR_BK_IN, shBtnColor);
	//m_btn.SetTooltipText(_T("Close the application"));

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CButtonTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CButtonTestDlg::OnPaint() 
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
HCURSOR CButtonTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CButtonTestDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	m_btn.SetIcon(IDI_ICON2);
	m_btn.SetAlign(CButtonST::ST_ALIGN_VERT);
//	AfxMessageBox("ddd");	
}

void CButtonTestDlg::OnButtonStarttest() 
{
	// TODO: Add your control notification handler code here
	m_BtnStartTest.SetIcon(IDI_ICON_STOP);
	this->SetDlgItemText(IDC_BUTTON_STARTTEST,"Õ£÷π≤‚ ‘");


}
