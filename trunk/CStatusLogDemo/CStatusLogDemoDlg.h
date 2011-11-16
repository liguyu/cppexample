// CStatusLogDemoDlg.h : header file
//

#if !defined(AFX_CSTATUSLOGDEMODLG_H__3C809BF6_9DFD_11D3_A10D_00500402F30B__INCLUDED_)
#define AFX_CSTATUSLOGDEMODLG_H__3C809BF6_9DFD_11D3_A10D_00500402F30B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CCStatusLogDemoDlg dialog

class CCStatusLogDemoDlg : public CDialog
{
// Construction
public:
	CCStatusLogDemoDlg(CWnd* pParent = NULL);	// standard constructor

   ~CCStatusLogDemoDlg();
// Dialog Data
	//{{AFX_DATA(CCStatusLogDemoDlg)
	enum { IDD = IDD_CSTATUSLOGDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCStatusLogDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CCStatusLogDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSTATUSLOGDEMODLG_H__3C809BF6_9DFD_11D3_A10D_00500402F30B__INCLUDED_)
