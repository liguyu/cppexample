// XMS_DialDlg.h : header file
//

#if !defined(AFX_XMS_DIALDLG_H__7E92C509_3412_40D5_97FC_88524CFAAD9F__INCLUDED_)
#define AFX_XMS_DIALDLG_H__7E92C509_3412_40D5_97FC_88524CFAAD9F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXMS_DialDlg dialog

class CXMS_DialDlg : public CDialog
{
// Construction
public:
	CXMS_DialDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXMS_DialDlg)
	enum { IDD = IDD_XMS_DIAL_DIALOG };
	CListCtrl	m_ListCount;
	CListCtrl	m_ListUser;
	CListBox	m_ListMsg;
	CListCtrl	m_ListPcm;
	CListCtrl	m_ListMain;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_DialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXMS_DialDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnBUTTONSimulate();
	afx_msg void OnBUTTONReOpen();
	afx_msg void OnBUTTONRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_DIALDLG_H__7E92C509_3412_40D5_97FC_88524CFAAD9F__INCLUDED_)
