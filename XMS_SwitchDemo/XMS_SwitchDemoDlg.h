// XMS_SwitchDemoDlg.h : header file
//

#if !defined(AFX_XMS_SWITCHDEMODLG_H__398199E9_5843_4C42_B9E1_D720B2A6125F__INCLUDED_)
#define AFX_XMS_SWITCHDEMODLG_H__398199E9_5843_4C42_B9E1_D720B2A6125F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXMS_SwitchDemoDlg dialog

class CXMS_SwitchDemoDlg : public CDialog
{
// Construction
public:
	CXMS_SwitchDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXMS_SwitchDemoDlg)
	enum { IDD = IDD_XMS_SWITCHDEMO_DIALOG };
	CListCtrl	m_List_AnalogUser;
	CListBox	m_List_Msg;
	CListCtrl	m_List_ChnRes;
	CListCtrl	m_List_DSPRes;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_SwitchDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXMS_SwitchDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_SWITCHDEMODLG_H__398199E9_5843_4C42_B9E1_D720B2A6125F__INCLUDED_)
