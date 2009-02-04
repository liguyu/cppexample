// KeySwitchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KeySwitch.h"
#include "KeySwitchDlg.h"
#include "PublicDef.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

static UINT StatusBarItem[]=
{
		IDS_SYS_STATUS,
		IDS_SYS_STATUS_VALUE
};


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
// CKeySwitchDlg dialog

CKeySwitchDlg::CKeySwitchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeySwitchDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeySwitchDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeySwitchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeySwitchDlg)
	DDX_Control(pDX, IDC_VOIP, m_VoipList);
	DDX_Control(pDX, IDC_VOICE, m_VoiceList);
	DDX_Control(pDX, IDC_TRUNK, m_TrunkList);
	DDX_Control(pDX, IDC_DSP, m_DspList);
	DDX_Control(pDX, IDC_LIST1, m_ListInfo);
	DDX_Control(pDX, IDC_LIST_STATUS, m_ListControl);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKeySwitchDlg, CDialog)
	//{{AFX_MSG_MAP(CKeySwitchDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_COMMAND(IDR_INLINE_SET, OnInlineSet)
	ON_COMMAND(IDR_OUTLINE_SET, OnOutlineSet)
	ON_COMMAND(IDR_EXIT_SYS, OnExitSys)
	ON_COMMAND(IDR_START_SERVICE, OnStartService)
	ON_COMMAND(IDR_DEBUG, OnDebug)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_NOTIFY(HDN_ITEMDBLCLICK, IDC_LIST_STATUS, OnItemdblclickListStatus)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_STATUS, OnDblclkListStatus)
	ON_MESSAGE(WM_REFRESH_STATUS_BAR,OnRefreshStatusBar)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeySwitchDlg message handlers

BOOL CKeySwitchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CString		fTitle;
	fTitle.LoadString(IDS_APP_TITLE);
	SetWindowText((LPCSTR)fTitle);
	mIsRun=false;

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

	m_ListControl.SetExtendedStyle(LVS_EX_FULLROWSELECT);		//设置为行选择模式

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	this->ShowWindow(SW_SHOWMAXIMIZED);
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	if(!m_wndStatusBar.Create(this) || !m_wndStatusBar.SetIndicators(StatusBarItem,sizeof(StatusBarItem)/sizeof(UINT)))	return -1;
	m_wndStatusBar.SetPaneStyle(m_wndStatusBar.GetCount()-1,SBPS_STRETCH);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST,AFX_IDW_CONTROLBAR_LAST,0);
	AfxInitRichEdit( );		//初始化richedit
	//初始化列表控件标题
	InitStatusControl(&m_ListControl);
	//调用初始化函数进行初始化r
	if(InitSystem()==false)
	{
		MessageBox("系统初始化失败!!","Error",MB_OK);
		return false;
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKeySwitchDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CKeySwitchDlg::OnPaint() 
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
HCURSOR CKeySwitchDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CKeySwitchDlg::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::Create(IDD, pParentWnd);
}

void CKeySwitchDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if(MessageBox("您真的要退出该系统吗?","Info",MB_YESNO)==IDNO) 
		return;
	else
		CloseAllDevice();
		CKeySwitchDlg::DestroyWindow();
}

void CKeySwitchDlg::OnInlineSet() 
{
	// 内线设置对话框
	m_InLineDlg.DoModal();
}

void CKeySwitchDlg::OnOutlineSet() 
{
	// 外线设置对话框
	m_OutLineDlg.DoModal();	
}


void CKeySwitchDlg::OnExitSys() 
{
	// TODO: Add your command handler code here
	if(MessageBox("您真的要退出该系统吗?","Info",MB_YESNO)==IDNO) 
		return;
	else
	{
		CloseAllDevice();
		CloseXMSConnect();
		this->CloseWindow();
	}
}

void CKeySwitchDlg::OnStartService() 
{
	// TODO: Add your command handler code here
	/////进行初始化的设备邦定
	if(mIsRun==false)
	{
		if(LinkInitVoice()==false)
		{
			MessageBox("系统启动服务失败!!","Error",MB_OK);
			m_wndStatusBar.SetPaneText(1,"启动服务失败");
		}
		else
			mIsRun=true;
	}
	else
	{
		MessageBox("服务已经启动!!","Error",MB_OK);
	}
}

void CKeySwitchDlg::OnDebug() 
{
	// TODO: Add your command handler code here
	DebugOutput();	
}

void CKeySwitchDlg::OnButton1() 
{
	// TODO: Add your control notification handler code here
	VerifyData();

}

void CKeySwitchDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	m_TrunkList.ResetContent();
	m_VoiceList.ResetContent();
	m_VoipList.ResetContent();
	
}

void CKeySwitchDlg::OnButton3() 
{
	// TODO: Add your control notification handler code here

}

void CKeySwitchDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	CloseAllDevice();
	CloseXMSConnect();
	
}


void CKeySwitchDlg::OnItemdblclickListStatus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	// TODO: Add your control notification handler code here

	*pResult = 0;
}

void CKeySwitchDlg::OnDblclkListStatus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	/*双击某一个项目，进行配置*/
	int		mItemIndex;
	DEVICE_TYPE	pDeviceType;
	LINE_TYPE	pLineType;
	long	pDeviceNo;
	DJ_S32	pDspNo;

	long	mDeviceNo;
	DJ_S32	mDspNo;
	DEVICE_TYPE	mDeviceType;
	LINE_TYPE	mLineType;

	
	mItemIndex=m_ListControl.GetNextItem(-1,LVIS_SELECTED);
	if(mItemIndex<0)	return;
	if(!JudgeCanEdit(mItemIndex,&pDeviceType,&pDeviceNo,&pLineType,&pDspNo))
	{
		MessageBox("对不起!您不能对该设备进行修改!!","Message",MB_OK);
		return;
	}
	mDeviceNo=pDeviceNo;
	mLineType=pLineType;
	mDspNo=pDspNo;
	mDeviceType=pDeviceType;

	if(pDeviceType==DEVICE_TRUNK)	
	{
		if(pLineType==LINE_IN)
		{
			CInLineSetDlg		mDlg;
			mDlg.mDspNo=mDspNo;
			mDlg.mDeviceNo=mDeviceNo;
			mDlg.DoModal();
			if(!mDlg.pResult)	return ;
			if(!RefreshDeviceGUI(pDspNo,pDeviceNo,LINE_IN))
			{
				MessageBox("系统刷新数据失败!!","Error",MB_OK);
				return ;
			}
			
		}
		if(pLineType==LINE_OUT)
		{
			COutLineSetDlg		mDlg;
			mDlg.mDspNo=pDspNo;
			mDlg.mDeviceNo=pDeviceNo;
			mDlg.DoModal();
			if(!mDlg.pResult)	return ;
			if(!RefreshDeviceGUI(pDspNo,pDeviceNo,LINE_OUT))
			{
				MessageBox("系统刷新数据失败!!","Error",MB_OK);
				return ;
			}
		}
	}
	if(pDeviceType==DEVICE_VOIP)
	{
	}

	*pResult = 0;
}

/*刷新状态栏数据*/
LRESULT	CKeySwitchDlg::OnRefreshStatusBar(WPARAM wParam,LPARAM lParam)
{
	if(strlen(mUpdateStatusBar)>0)
		m_wndStatusBar.SetPaneText(1,mUpdateStatusBar);
	return	true;
}

void CKeySwitchDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
}	



void CKeySwitchDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	DisplayVoipStatus();
	
}
