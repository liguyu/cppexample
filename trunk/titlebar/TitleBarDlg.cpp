// TitleBarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TitleBar.h"
#include "TitleBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTitleBarDlg dialog

CTitleBarDlg::CTitleBarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTitleBarDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTitleBarDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTitleBarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTitleBarDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTitleBarDlg, CDialog)
	//{{AFX_MSG_MAP(CTitleBarDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCMOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTitleBarDlg message handlers

BOOL CTitleBarDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CTitleBarDlg::OnPaint() 
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
HCURSOR CTitleBarDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

LRESULT CTitleBarDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	LRESULT lrst=CDialog::DefWindowProc(message, wParam, lParam);
	
	if (!::IsWindow(m_hWnd))
		return lrst;
	
	if (message==WM_MOVE||message==WM_PAINT||message==WM_NCPAINT||message==WM_NCACTIVATE ||message == WM_NOTIFY)
	{
		CDC* pWinDC = GetWindowDC();
		if (pWinDC)
			DrawTitleBar(pWinDC);
		ReleaseDC(pWinDC);
	}
	return lrst;
}

void CTitleBarDlg::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	//检测各按钮是否按到
	if (m_rtIcon.PtInRect(point))
		AfxMessageBox("欢迎访问FlySnowStdio主页");
	else if (m_rtButtHelp.PtInRect(point))
		SendMessage(WM_HELP);
	else if (m_rtButtExit.PtInRect(point))
		SendMessage(WM_CLOSE);
	else if (m_rtButtMin.PtInRect(point))
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, MAKELPARAM(point.x, point.y));
	else if (m_rtButtMax.PtInRect(point))
	{
		if (IsZoomed())
			SendMessage(WM_SYSCOMMAND, SC_RESTORE, MAKELPARAM(point.x, point.y));
		else
		{
			SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE, MAKELPARAM(point.x, point.y));
			Invalidate();
		}
	}
	else if (!IsZoomed())
		Default();
}

void CTitleBarDlg::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	
	CDC* pDC = GetWindowDC();
	CDC* pDisplayMemDC=new CDC;
	pDisplayMemDC->CreateCompatibleDC(pDC);
	CBitmap* pBitmap = new CBitmap;
	CBitmap* pOldBitmap;
	CRect rtWnd, rtButton;

	if (pDC)
	{
		GetWindowRect(&rtWnd);

		//关闭button
		if (m_rtButtExit.PtInRect(point))
			pBitmap->LoadBitmap(IDB_EXIT_FOCUS);
		else
			pBitmap->LoadBitmap(IDB_EXIT_NORMAL);
		rtButton = m_rtButtExit;
		rtButton.OffsetRect(-rtWnd.left, -rtWnd.top);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButton.left, rtButton.top, rtButton.Width(), rtButton.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		pBitmap->DeleteObject();

		//最大化/恢复button
		if (m_rtButtMax.PtInRect(point))
		{
			if (IsZoomed())
				pBitmap->LoadBitmap(IDB_RESTORE_FOCUS);
			else
				pBitmap->LoadBitmap(IDB_MAX_FOCUS);
		}
		else
		{
			if (IsZoomed())
				pBitmap->LoadBitmap(IDB_RESTORE_NORMAL);
			else
				pBitmap->LoadBitmap(IDB_MAX_NORMAL);
		}
		rtButton = m_rtButtMax;
		rtButton.OffsetRect(-rtWnd.left, -rtWnd.top);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButton.left, rtButton.top, rtButton.Width(), rtButton.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		pBitmap->DeleteObject();

		//最小化button
		if (m_rtButtMin.PtInRect(point))
			pBitmap->LoadBitmap(IDB_MIN_FOCUS);
		else
			pBitmap->LoadBitmap(IDB_MIN_NORMAL);
		rtButton = m_rtButtMin;
		rtButton.OffsetRect(-rtWnd.left, -rtWnd.top);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButton.left, rtButton.top, rtButton.Width(), rtButton.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		pBitmap->DeleteObject();

		//帮助button
		if (m_rtButtHelp.PtInRect(point))
			pBitmap->LoadBitmap(IDB_HELP_FOCUS);
		else
			pBitmap->LoadBitmap(IDB_HELP_NORMAL);
		rtButton = m_rtButtHelp;
		rtButton.OffsetRect(-rtWnd.left, -rtWnd.top);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButton.left, rtButton.top, rtButton.Width(), rtButton.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		pBitmap->DeleteObject();
	}
	
	ReleaseDC(pDisplayMemDC);
	ReleaseDC(pDC);
	delete pDisplayMemDC;
	delete pBitmap;

	CDialog::OnNcMouseMove(nHitTest, point);
}

void CTitleBarDlg::DrawTitleBar(CDC *pDC)
{
	if (m_hWnd)
	{
		CBrush Brush(RGB(0,100,255));
		CBrush* pOldBrush = pDC->SelectObject(&Brush);

		CRect rtWnd, rtTitle, rtButtons;
		GetWindowRect(&rtWnd); 
		//取得标题栏的位置
		rtTitle.left = GetSystemMetrics(SM_CXFRAME);
		rtTitle.top = GetSystemMetrics(SM_CYFRAME);
		rtTitle.right = rtWnd.right - rtWnd.left - GetSystemMetrics(SM_CXFRAME);
		rtTitle.bottom = rtTitle.top + GetSystemMetrics(SM_CYSIZE);

		CPoint point;
		//填充顶部框架
		point.x = rtWnd.Width();
		point.y = GetSystemMetrics(SM_CYSIZE) + GetSystemMetrics(SM_CYFRAME)+20;
		pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);
		//填充左侧框架
		point.x = GetSystemMetrics(SM_CXFRAME) + 1;
		point.y = rtWnd.Height();
		pDC->PatBlt(0, 0, point.x, point.y, PATCOPY);
		//填充底部框架
		point.x = rtWnd.Width(); 
		point.y = GetSystemMetrics(SM_CYFRAME) + 1;
		pDC->PatBlt(0, rtWnd.Height()-point.y, point.x, point.y, PATCOPY);
		//填充右侧框架
		point.x = GetSystemMetrics(SM_CXFRAME) + 1;
		point.y = rtWnd.Height();
		pDC->PatBlt(rtWnd.Width()-point.x, 0, point.x, point.y, PATCOPY);

		//绘制美化线条
		CBrush BrushLine(RGB(255, 255, 255));
		pDC->SelectObject(&BrushLine);
		point.x = rtWnd.Width() - 150;
		point.y = GetSystemMetrics(SM_CYFRAME) + 2;
		pDC->PatBlt(GetSystemMetrics(SM_CXFRAME), point.y, point.x, 1, PATCOPY);
		point.y += 5;
		pDC->PatBlt(GetSystemMetrics(SM_CXFRAME), point.y, point.x, 1, PATCOPY);
		point.y += 5;
		pDC->PatBlt(GetSystemMetrics(SM_CXFRAME), point.y, point.x, 1, PATCOPY);
		point.x += 60;
		point.y += 12;
		pDC->PatBlt(point.x, point.y, 85, 1, PATCOPY);
		point.y += 5;
		pDC->PatBlt(point.x, point.y, 85, 1, PATCOPY);
		point.x -= 60 - GetSystemMetrics(SM_CXFRAME);
		point.y += 5;
		pDC->PatBlt(point.x, point.y, 145-GetSystemMetrics(SM_CXFRAME) , 1, PATCOPY);
		point.y -= 32;
		pDC->PatBlt(point.x, point.y, 1 , 32, PATCOPY);
		
		pDC->SelectObject(pOldBrush);

		//重画图标
		m_rtIcon.left = rtWnd.Width() - 135;
		m_rtIcon.top = GetSystemMetrics(SM_CYFRAME);
		m_rtIcon.right = m_rtIcon.left + 32;
		m_rtIcon.bottom = m_rtIcon.top + 32;
		::DrawIconEx(pDC->m_hDC, m_rtIcon.left, m_rtIcon.top, m_hIcon, 
			m_rtIcon.Width(), m_rtIcon.Height(), 0, NULL, DI_NORMAL);
		m_rtIcon.OffsetRect(rtWnd.TopLeft()); 


		//准备画XP风格按钮
		CBitmap* pBitmap = new CBitmap;
		CBitmap* pOldBitmap;
		CDC* pDisplayMemDC=new CDC;
		pDisplayMemDC->CreateCompatibleDC(pDC);

		//重画关闭button
		rtButtons.left = rtTitle.right - 19;
		rtButtons.top = rtTitle.top;
		rtButtons.right = rtButtons.left + 19;
		rtButtons.bottom = rtButtons.top + 19;
		pBitmap->LoadBitmap(IDB_EXIT_NORMAL);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButtons.left, rtButtons.top, rtButtons.Width(), rtButtons.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		m_rtButtExit = rtButtons;
		m_rtButtExit.OffsetRect(rtWnd.TopLeft()); 
		pBitmap->DeleteObject();

		//重画最大化/恢复button
		rtButtons.right = rtButtons.left - 3;
		rtButtons.left = rtButtons.right - 19;
		if (IsZoomed())
			pBitmap->LoadBitmap(IDB_RESTORE_NORMAL);
		else
			pBitmap->LoadBitmap(IDB_MAX_NORMAL);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButtons.left, rtButtons.top, rtButtons.Width(), rtButtons.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		m_rtButtMax = rtButtons;
		m_rtButtMax.OffsetRect(rtWnd.TopLeft());
		pBitmap->DeleteObject();

		//重画最小化button
		rtButtons.right = rtButtons.left - 3;
		rtButtons.left = rtButtons.right - 19;
		pBitmap->LoadBitmap(IDB_MIN_NORMAL);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButtons.left, rtButtons.top, rtButtons.Width(), rtButtons.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		m_rtButtMin = rtButtons;
		m_rtButtMin.OffsetRect(rtWnd.TopLeft());
		pBitmap->DeleteObject();
	
		//重画帮助button
		rtButtons.right = rtButtons.left - 3;
		rtButtons.left = rtButtons.right - 19;
		pBitmap->LoadBitmap(IDB_HELP_NORMAL);
		pOldBitmap=(CBitmap*)pDisplayMemDC->SelectObject(pBitmap);
		pDC->BitBlt(rtButtons.left, rtButtons.top, rtButtons.Width(), rtButtons.Height(), pDisplayMemDC, 0, 0, SRCCOPY);
		pDisplayMemDC->SelectObject(pOldBitmap);
		m_rtButtHelp = rtButtons;
		m_rtButtHelp.OffsetRect(rtWnd.TopLeft());
		pBitmap->DeleteObject();
		
		//重画caption
		int nOldMode = pDC->SetBkMode(TRANSPARENT);
		COLORREF clOldText=pDC->SetTextColor(RGB(0xff, 0xff, 0xff));
		pDC->SelectStockObject(SYSTEM_FIXED_FONT);
		rtTitle.left += 2;
		rtTitle.top += GetSystemMetrics(SM_CYSIZE);
		rtTitle.bottom = rtTitle.top + 30;
		CString m_strTitle;
		GetWindowText(m_strTitle);
		CFont   font;
		font.CreatePointFont(140, "华文行楷");
		pDC->SelectObject(&font); 
		pDC->DrawText(m_strTitle, &rtTitle, DT_LEFT);
		pDC->SetBkMode(nOldMode);
		pDC->SetTextColor(clOldText);


		ReleaseDC(pDisplayMemDC);
		delete pDisplayMemDC;
		delete pBitmap;
	}
}





















