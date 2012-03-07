// MemoryDrawDlg.h : header file
//

#if !defined(AFX_MEMORYDRAWDLG_H__1E3BFC56_C349_40AE_82B2_98FB4AC8DAE9__INCLUDED_)
#define AFX_MEMORYDRAWDLG_H__1E3BFC56_C349_40AE_82B2_98FB4AC8DAE9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMemoryDrawDlg dialog

class CMemoryDrawDlg : public CDialog
{
// Construction
public:
	void DrawWave(CDC *pDC);
	CMemoryDrawDlg(CWnd* pParent = NULL);	// standard constructor
		CRect rect;                  // 存储绘图控件的绘图区域
        CDC *pDC;                  // 控件的屏幕绘图设备指针
        CDC memDC;              // 内存绘图设备
        CBitmap memBitmap;  // 用于内存绘图的位图  
        CBitmap* pOldBmp;    // 备份旧的位图指针 
        CWnd* pWnd;             // 绘图控件的指针
			CRect  m_rect;

// Dialog Data
	//{{AFX_DATA(CMemoryDrawDlg)
	enum { IDD = IDD_MEMORYDRAW_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemoryDrawDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMemoryDrawDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMORYDRAWDLG_H__1E3BFC56_C349_40AE_82B2_98FB4AC8DAE9__INCLUDED_)
