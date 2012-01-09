// LedByStaticDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LedByStatic.h"
#include "LedByStaticDlg.h"

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
// CLedByStaticDlg dialog

CLedByStaticDlg::CLedByStaticDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLedByStaticDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLedByStaticDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLedByStaticDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLedByStaticDlg)
	DDX_Control(pDX, IDC_STATIC_2, m_Static2);
	DDX_Control(pDX, IDC_STATIC_PIC, m_MyLed);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLedByStaticDlg, CDialog)
	//{{AFX_MSG_MAP(CLedByStaticDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnOff)
	ON_BN_CLICKED(IDC_BUTTON_ON, OnButtonOn)
	ON_BN_CLICKED(IDC_STATIC_PIC, OnStaticPic)
	ON_BN_CLICKED(IDC_STATIC_2, OnStatic2)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLedByStaticDlg message handlers

BOOL CLedByStaticDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	char tmpStr[256]={0};
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
	
	m_brshBack.CreateSolidBrush(RGB(192, 192, 192));

	m_MyLed.SetOnText("1");
	
	// CG: The following block was added by the ToolTips component.
	{
		// Create the ToolTip control.
		m_tooltip.Create(this);
		m_tooltip.Activate(TRUE);

		// TODO: Use one of the following forms to add controls:
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), <string-table-id>);
		// m_tooltip.AddTool(GetDlgItem(IDC_<name>), "<text>");
	}

	//提示框，自己添加代码
	CWnd*   pW;
	pW = GetDlgItem(IDC_STATIC_PIC);  
	sprintf(tmpStr,"hello,my friend\nnihao, world\nLabel%d",2);
	m_tooltip.AddTool(pW,tmpStr); 
 	m_tooltip.SetMaxTipWidth(100);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLedByStaticDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLedByStaticDlg::OnPaint() 
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
HCURSOR CLedByStaticDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLedByStaticDlg::OnOff() 
{
	m_MyLed.SetOnOff(false);
}

void CLedByStaticDlg::OnButtonOn() 
{
	m_MyLed.SetOnOff(true);	
}

void CLedByStaticDlg::OnStaticPic() 
{
	m_MyLed.SetOnFgColor(RGB(128, 128, 128));

}

void CLedByStaticDlg::OnStatic2() 
{
	AfxMessageBox("222");
	
}

BOOL CLedByStaticDlg::PreTranslateMessage(MSG* pMsg)
{
	// CG: The following block was added by the ToolTips component.
	{
		// Let the ToolTip process this message.
		m_tooltip.RelayEvent(pMsg);
	}
	return CDialog::PreTranslateMessage(pMsg);	// CG: This was added by the ToolTips component.
}

void CLedByStaticDlg::OnOK() 
{
	CWnd*   pW;
	pW = GetDlgItem(IDC_STATIC_PIC);  
	m_tooltip.AddTool(pW,"OK\nOK1\nhaha"); 
	
//	CDialog::OnOK();
}

HBRUSH CLedByStaticDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
    if ( nCtlColor == CTLCOLOR_DLG )
    { 
        return (HBRUSH)m_brshBack; 
    }
	return hbr;
}
