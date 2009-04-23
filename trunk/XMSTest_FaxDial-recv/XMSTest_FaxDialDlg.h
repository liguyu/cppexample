// XMSTest_FaxDialDlg.h : header file
//

#if !defined(AFX_XMSTEST_FAXDIALDLG_H__2DD779D7_EB06_4DB2_AB95_93AE788A9979__INCLUDED_)
#define AFX_XMSTEST_FAXDIALDLG_H__2DD779D7_EB06_4DB2_AB95_93AE788A9979__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXMSTest_FaxDialDlg dialog

class CXMSTest_FaxDialDlg : public CDialog
{
// Construction
public:
	CXMSTest_FaxDialDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXMSTest_FaxDialDlg)
	enum { IDD = IDD_XMSTEST_FAXDIAL_DIALOG };
	CListBox	m_ListMsg;
	CListCtrl	m_ListPcm;
	CListCtrl	m_ListMain;
	CListCtrl	m_ListFax;
	CListCtrl	m_ListCount;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMSTest_FaxDialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXMSTest_FaxDialDlg)
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

#endif // !defined(AFX_XMSTEST_FAXDIALDLG_H__2DD779D7_EB06_4DB2_AB95_93AE788A9979__INCLUDED_)
