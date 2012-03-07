// Test_MultimediaTimerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Test_MultimediaTimer.h"
#include "Test_MultimediaTimerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void PASCAL TimerProc(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2);
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
// CTest_MultimediaTimerDlg dialog

CTest_MultimediaTimerDlg::CTest_MultimediaTimerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTest_MultimediaTimerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTest_MultimediaTimerDlg)
	m_ilow = 0;
	m_iup = 0;
	m_icur = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTest_MultimediaTimerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTest_MultimediaTimerDlg)
	DDX_Text(pDX, IDC_EDIT_LOW, m_ilow);
	DDX_Text(pDX, IDC_EDIT_UP, m_iup);
	DDX_Text(pDX, IDC_EDIT_CUR, m_icur);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTest_MultimediaTimerDlg, CDialog)
	//{{AFX_MSG_MAP(CTest_MultimediaTimerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_IM, OnButtonIm)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_LOW, OnButtonLow)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_SETRANGE, OnButtonSetrange)
	ON_BN_CLICKED(IDC_BUTTON_SETCURRES, OnButtonSetcurres)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTest_MultimediaTimerDlg message handlers

BOOL CTest_MultimediaTimerDlg::OnInitDialog()
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
	m_Timer.SetProcParameter(TimerProc,DWORD(this));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTest_MultimediaTimerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CTest_MultimediaTimerDlg::OnPaint() 
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
HCURSOR CTest_MultimediaTimerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTest_MultimediaTimerDlg::OnButtonStart() 
{
	m_Timer.Start();
	ShowIm();
}

void CTest_MultimediaTimerDlg::OnButtonStop() 
{
	m_Timer.Stop();
	ShowIm();
}

void CTest_MultimediaTimerDlg::OnButtonUp() 
{
	m_Timer.SpeedUp();
	ShowIm();
}

void CTest_MultimediaTimerDlg::OnButtonLow() 
{
	m_Timer.SlowDown();
	ShowIm();
}

void CTest_MultimediaTimerDlg::OnButtonPause() 
{
	m_Timer.Pause();
	ShowIm();
}

void CTest_MultimediaTimerDlg::OnButtonSetrange() 
{
	UpdateData();
	m_Timer.SetRange(m_ilow,m_iup);
	ShowIm();
}

void CTest_MultimediaTimerDlg::OnButtonSetcurres() 
{
	UpdateData();
	m_Timer.SetCurRes(m_icur);
	ShowIm();
}

int counter(0);
void PASCAL TimerProc(UINT wTimerID, UINT msg,DWORD dwUser,DWORD dwl,DWORD dw2)
{
	CTest_MultimediaTimerDlg* pDlg = (CTest_MultimediaTimerDlg*)dwUser;
	
	CString str;
	str.Format("%d", counter);
	if(pDlg->GetDlgItem(IDC_STATIC_SHOW)->GetSafeHwnd())
		pDlg->GetDlgItem(IDC_STATIC_SHOW)->SetWindowText(str);
	counter++;
}

void CTest_MultimediaTimerDlg::OnButtonIm() 
{
	CString str;
	str.Format("Resolution: Min:%d Max:%d",m_Timer.timeGetDevCaps().wPeriodMin,m_Timer.timeGetDevCaps().wPeriodMax);
	GetDlgItem(IDC_STATIC_IM)->SetWindowText(str);
}

void CTest_MultimediaTimerDlg::ShowIm()
{
	CString str,str1,str2,str3,str4;
	m_Timer.IsStart() ? str1 = "Start:TRUE" : str1 = "Start:FALSE";
	m_Timer.IsPause() ? str2 = "Pause:TRUE" : str2 = "Pause:FALSE";
	m_Timer.IsStop()  ? str3 = "Stop:TRUE"  : str3 = "Stop:FALSE";
	
	UINT Lower,Upper;
	m_Timer.GetRange(Lower,Upper);
	str4.Format("Timer Range (%d,%d)",Lower,Upper);

	str.Format("CurRes:%d  %s  %s  %s\n%s",m_Timer.GetCurRes(),str1,str2,str3,str4);
	GetDlgItem(IDC_STATIC_STATE)->SetWindowText(str);
}
