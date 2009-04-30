// XMS_VOIPTestDialDlg.h : header file
//

#if !defined(AFX_XMS_VOIPTESTDIALDLG_H__F2D34EF1_0A25_43EE_A881_A6BC68E211BC__INCLUDED_)
#define AFX_XMS_VOIPTESTDIALDLG_H__F2D34EF1_0A25_43EE_A881_A6BC68E211BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXMS_VOIPTestDialDlg dialog

class CXMS_VOIPTestDialDlg : public CDialog
{
// Construction
public:
	CXMS_VOIPTestDialDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXMS_VOIPTestDialDlg)
	enum { IDD = IDD_XMS_VOIPTESTDIAL_DIALOG };
	CListBox	m_ListMsg;
	CListCtrl	m_ListCount;
	CListCtrl	m_ListMain;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_VOIPTestDialDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXMS_VOIPTestDialDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnBUTTONStart();
	afx_msg void OnBUTTONStop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_VOIPTESTDIALDLG_H__F2D34EF1_0A25_43EE_A881_A6BC68E211BC__INCLUDED_)
