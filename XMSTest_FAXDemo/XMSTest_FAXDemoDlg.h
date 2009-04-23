// XMSTest_FAXDemoDlg.h : header file
//

#if !defined(AFX_XMSTEST_FAXDEMODLG_H__247C1ED0_8E5F_4155_BFEB_FB825AA027DA__INCLUDED_)
#define AFX_XMSTEST_FAXDEMODLG_H__247C1ED0_8E5F_4155_BFEB_FB825AA027DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXMSTest_FAXDemoDlg dialog

class CXMSTest_FAXDemoDlg : public CDialog
{
// Construction
public:
	CXMSTest_FAXDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXMSTest_FAXDemoDlg)
	enum { IDD = IDD_XMSTEST_FAXDEMO_DIALOG };
	CListCtrl	m_ListFax;
	CListCtrl	m_ListCount;
	CListCtrl	m_ListMain;
	CListBox	m_ListMsg;
	CListCtrl	m_ListPcm;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMSTest_FAXDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXMSTest_FAXDemoDlg)
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

#endif // !defined(AFX_XMSTEST_FAXDEMODLG_H__247C1ED0_8E5F_4155_BFEB_FB825AA027DA__INCLUDED_)
