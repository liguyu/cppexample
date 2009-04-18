// XMS_ConfDemoDlg.h : header file
//

#if !defined(AFX_XMS_CONFDEMODLG_H__763FACD7_7DF2_46B7_991D_9DF2558215A3__INCLUDED_)
#define AFX_XMS_CONFDEMODLG_H__763FACD7_7DF2_46B7_991D_9DF2558215A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXMS_ConfDemoDlg dialog

class CXMS_ConfDemoDlg : public CDialog
{
// Construction
public:
	CXMS_ConfDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXMS_ConfDemoDlg)
	enum { IDD = IDD_XMS_CONFDEMO_DIALOG };
	CListCtrl	m_ListConf;
	CListCtrl	m_ListPcm;
	CListBox	m_ListMsg;
	CListCtrl	m_ListMain;
	CListCtrl	m_ListCount;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_ConfDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXMS_ConfDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnBUTTONReOpen();
	afx_msg void OnBUTTONRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_CONFDEMODLG_H__763FACD7_7DF2_46B7_991D_9DF2558215A3__INCLUDED_)
