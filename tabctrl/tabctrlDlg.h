// tabctrlDlg.h : header file
//
#include "TabSheet.h"
#if !defined(AFX_TABCTRLDLG_H__BC0E2C6F_F1F1_4D39_90C3_CD47F9906103__INCLUDED_)
#define AFX_TABCTRLDLG_H__BC0E2C6F_F1F1_4D39_90C3_CD47F9906103__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Page1Dlg.h"
#include "Page2Dlg.h"
/////////////////////////////////////////////////////////////////////////////
// CTabctrlDlg dialog

class CTabctrlDlg : public CDialog
{
// Construction
public:
	CTabctrlDlg(CWnd* pParent = NULL);	// standard constructor
	Page1Dlg m_page1;
	Page2Dlg m_page2;
// Dialog Data
	//{{AFX_DATA(CTabctrlDlg)
	enum { IDD = IDD_TABCTRL_DIALOG };
	CTabCtrl	m_TabCtrlAll;
	CTabSheet	m_sheet;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabctrlDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTabctrlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCTRLDLG_H__BC0E2C6F_F1F1_4D39_90C3_CD47F9906103__INCLUDED_)
