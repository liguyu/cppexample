// ButtonTestDlg.h : header file
//
#include "BtnST.h"

#if !defined(AFX_BUTTONTESTDLG_H__EB6581C5_40E4_4DBA_A7D2_23AC761F3D1D__INCLUDED_)
#define AFX_BUTTONTESTDLG_H__EB6581C5_40E4_4DBA_A7D2_23AC761F3D1D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CButtonTestDlg dialog

class CButtonTestDlg : public CDialog
{
// Construction
public:
	CButtonTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CButtonTestDlg)
	enum { IDD = IDD_BUTTONTEST_DIALOG };
	CButtonST	m_BtnSysSet;
	CButtonST	m_BtnStartTest;
	CButtonST	m_BtnPassword;
	CButtonST	m_BtnParamImport;
	CButtonST	m_BtnParamEdit;
	CButtonST	m_BtnHelp;
	CButtonST	m_BtnExit;
	CButtonST	m_btn;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CButtonTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButtonStarttest();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONTESTDLG_H__EB6581C5_40E4_4DBA_A7D2_23AC761F3D1D__INCLUDED_)
