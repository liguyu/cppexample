// XMS_VOIPTestDemoDlg.h : header file
//

#if !defined(AFX_XMS_VOIPTESTDEMODLG_H__27C84F9D_EB8F_43E1_BBD8_E91957ADC3A3__INCLUDED_)
#define AFX_XMS_VOIPTESTDEMODLG_H__27C84F9D_EB8F_43E1_BBD8_E91957ADC3A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXMS_VOIPTestDemoDlg dialog

class CXMS_VOIPTestDemoDlg : public CDialog
{
// Construction
public:
	CXMS_VOIPTestDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXMS_VOIPTestDemoDlg)
	enum { IDD = IDD_XMS_VOIPTESTDEMO_DIALOG };
	CListCtrl	m_ListCount;
	CListCtrl	m_ListMain;
	CListBox	m_ListMsg;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_VOIPTestDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXMS_VOIPTestDemoDlg)
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

#endif // !defined(AFX_XMS_VOIPTESTDEMODLG_H__27C84F9D_EB8F_43E1_BBD8_E91957ADC3A3__INCLUDED_)
