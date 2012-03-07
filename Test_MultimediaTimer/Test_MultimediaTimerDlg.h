// Test_MultimediaTimerDlg.h : header file
//

#if !defined(AFX_TEST_MULTIMEDIATIMERDLG_H__D71AB084_F2F6_4A87_85FB_B36B9A9609F4__INCLUDED_)
#define AFX_TEST_MULTIMEDIATIMERDLG_H__D71AB084_F2F6_4A87_85FB_B36B9A9609F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MultimediaTimer.h"

/////////////////////////////////////////////////////////////////////////////
// CTest_MultimediaTimerDlg dialog

class CTest_MultimediaTimerDlg : public CDialog
{
// Construction
public:
	CTest_MultimediaTimerDlg(CWnd* pParent = NULL);	// standard constructor

private:
	CMultimediaTimer m_Timer;
	void ShowIm();
// Dialog Data
	//{{AFX_DATA(CTest_MultimediaTimerDlg)
	enum { IDD = IDD_TEST_MULTIMEDIATIMER_DIALOG };
	UINT	m_ilow;
	UINT	m_iup;
	UINT	m_icur;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest_MultimediaTimerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTest_MultimediaTimerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStart();
	afx_msg void OnButtonStop();
	afx_msg void OnButtonIm();
	afx_msg void OnButtonUp();
	afx_msg void OnButtonLow();
	afx_msg void OnButtonPause();
	afx_msg void OnButtonSetrange();
	afx_msg void OnButtonSetcurres();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_MULTIMEDIATIMERDLG_H__D71AB084_F2F6_4A87_85FB_B36B9A9609F4__INCLUDED_)
