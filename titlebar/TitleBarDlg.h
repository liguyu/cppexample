// TitleBarDlg.h : header file
//

#if !defined(AFX_TITLEBARDLG_H__3A58CBD1_FFA4_4CA7_8E5C_B774124A9BDE__INCLUDED_)
#define AFX_TITLEBARDLG_H__3A58CBD1_FFA4_4CA7_8E5C_B774124A9BDE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTitleBarDlg dialog

class CTitleBarDlg : public CDialog
{
// Construction
public:
	void DrawTitleBar(CDC *pDC);
	CRect m_rtButtExit;	//关闭按钮位置
	CRect m_rtButtMax;	//最大化按钮位置
	CRect m_rtButtMin;	//最小化按钮位置
	CRect m_rtButtHelp;	//帮助按钮位置
	CRect m_rtIcon;		//图标位置
	CTitleBarDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTitleBarDlg)
	enum { IDD = IDD_TITLEBAR_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTitleBarDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTitleBarDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TITLEBARDLG_H__3A58CBD1_FFA4_4CA7_8E5C_B774124A9BDE__INCLUDED_)
