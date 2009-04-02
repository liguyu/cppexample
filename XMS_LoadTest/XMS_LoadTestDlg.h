// XMS_LoadTestDlg.h : header file
//

#if !defined(AFX_XMS_LOADTESTDLG_H__4C7720D3_F1B5_45F1_B99A_8A6770DAD689__INCLUDED_)
#define AFX_XMS_LOADTESTDLG_H__4C7720D3_F1B5_45F1_B99A_8A6770DAD689__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXMS_LoadTestDlg dialog

class CXMS_LoadTestDlg : public CDialog
{
// Construction
public:
	CXMS_LoadTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXMS_LoadTestDlg)
	enum { IDD = IDD_XMS_LOADTEST_DIALOG };
	CListBox	m_ListMsg;
	CListCtrl	m_ListCount;
	CListCtrl	m_ListPcm;
	CListCtrl	m_ListMain;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_LoadTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXMS_LoadTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_LOADTESTDLG_H__4C7720D3_F1B5_45F1_B99A_8A6770DAD689__INCLUDED_)
