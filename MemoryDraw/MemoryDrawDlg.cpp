// MemoryDrawDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MemoryDraw.h"
#include "MemoryDrawDlg.h"

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
// CMemoryDrawDlg dialog

CMemoryDrawDlg::CMemoryDrawDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMemoryDrawDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMemoryDrawDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMemoryDrawDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMemoryDrawDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMemoryDrawDlg, CDialog)
	//{{AFX_MSG_MAP(CMemoryDrawDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoryDrawDlg message handlers

BOOL CMemoryDrawDlg::OnInitDialog()
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
	
	pWnd = GetDlgItem(IDC_STATIC_1); // 获得对话框上的picture的窗口句柄
	pWnd->GetClientRect(&rect);         // 获取绘制坐标的文本框
	
	pDC = pWnd->GetDC();                 // 获得对话框上的picture的设备指针
	pOldBmp = NULL;                           // 将旧的位图指针置空
	// 创建内存绘图设备，使内存位图的DC与控件的DC关联
	memDC.CreateCompatibleDC(pDC); 
	memBitmap.CreateCompatibleBitmap(pDC,rect.right,rect.bottom);
	pOldBmp = memDC.SelectObject(&memBitmap);
	 SetTimer(1,10,NULL);          // 启动定时器
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CMemoryDrawDlg::DrawWave(CDC *pDC)
{
 CPen pen,*oldpen;
 
 memDC.Rectangle(rect.left,rect.top,rect.right,rect.bottom);
 for(int i =0; i<1500; i++)
 {
  pen.CreatePen(PS_SOLID,1, RGB(rand()%255,rand()%255,rand()%255));   
  oldpen = memDC.SelectObject(&pen);
  
  int x0,y0,x1,y1;
  
  x0 = rand()%rect.Width();
  x1 = rand()%rect.Width();
  y0 = rand()%rect.Height();
  y1 = rand()%rect.Height();
  
  memDC.MoveTo(x0,y0);
  memDC.LineTo(x1,y1);
  
  memDC.SelectObject(oldpen);
  pen.DeleteObject();
 }
}
void CMemoryDrawDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMemoryDrawDlg::OnPaint() 
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
		pDC->BitBlt(rect.left,rect.top,rect.right,rect.bottom,&memDC,0,0,SRCCOPY); 
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMemoryDrawDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMemoryDrawDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	DrawWave(&memDC);         // 在位图中画图片 
       OnPaint();                            // 使屏幕刷新      
	CDialog::OnTimer(nIDEvent);
}

void CMemoryDrawDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	 memDC.SelectObject(pOldBmp);
        memDC.DeleteDC();
        memBitmap.DeleteObject();
	CDialog::OnClose();
}

void CMemoryDrawDlg::OnSize(UINT nType, int cx, int cy) 
{
	if (	nType != SIZE_MINIMIZED)
	{
		CWnd * pWnd;
		CWnd *buttonWnd[10];
		buttonWnd[0]=GetDlgItem(IDC_STATIC_1);
		int i=0;
		{
			CRect rect;
			if(buttonWnd[i])
			{
				buttonWnd[i]->GetWindowRect(&rect);
				ScreenToClient(&rect);
				rect.left=rect.left*cx/m_rect.Width();
				rect.right=rect.right*cx/m_rect.Width();
				rect.top=rect.top*cy/m_rect.Height();
				rect.bottom=rect.bottom*cy/m_rect.Height();
				buttonWnd[i]->MoveWindow(rect);
			}
		}
        
		GetClientRect(&m_rect);
	}

	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}
