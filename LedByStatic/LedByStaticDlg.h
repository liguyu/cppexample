// LedByStaticDlg.h : header file
//

#if !defined(AFX_LEDBYSTATICDLG_H__FEAED18E_E32F_4361_8100_1F6CBC75DF92__INCLUDED_)
#define AFX_LEDBYSTATICDLG_H__FEAED18E_E32F_4361_8100_1F6CBC75DF92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LED.h"

/////////////////////////////////////////////////////////////////////////////
// CLedByStaticDlg dialog

class CLedByStaticDlg : public CDialog
{
// Construction
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CLedByStaticDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLedByStaticDlg)
	enum { IDD = IDD_LEDBYSTATIC_DIALOG };
	CStatic	m_Static2;
	CLED	m_MyLed;
	CBrush m_brshBack;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLedByStaticDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolTipCtrl m_tooltip;
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLedByStaticDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOff();
	afx_msg void OnButtonOn();
	afx_msg void OnStaticPic();
	afx_msg void OnStatic2();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEDBYSTATICDLG_H__FEAED18E_E32F_4361_8100_1F6CBC75DF92__INCLUDED_)
