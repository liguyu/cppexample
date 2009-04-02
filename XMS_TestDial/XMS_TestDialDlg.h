// XMS_TestDialDlg.h : header file
//

#if !defined(AFX_XMS_TESTDIALDLG_H__C3D4AC03_9963_4954_A5C6_5750079108F6__INCLUDED_)
#define AFX_XMS_TESTDIALDLG_H__C3D4AC03_9963_4954_A5C6_5750079108F6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXMS_TestDialDlg dialog

class CXMS_TestDialDlg : public CDialog
{
// Construction
public:
	CXMS_TestDialDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXMS_TestDialDlg)
	enum { IDD = IDD_XMS_TESTDIAL_DIALOG };
	CListCtrl	m_ListPcm;
	CListBox	m_ListMsg;
	CListCtrl	m_ListMain;
	CListCtrl	m_ListCount;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_TestDialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXMS_TestDialDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnEDITStart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_TESTDIALDLG_H__C3D4AC03_9963_4954_A5C6_5750079108F6__INCLUDED_)
