// XMS_DemoDlg.h : header file
//

#if !defined(AFX_XMS_DEMODLG_H__F4740706_1E3B_4921_8560_1D1C8104097F__INCLUDED_)
#define AFX_XMS_DEMODLG_H__F4740706_1E3B_4921_8560_1D1C8104097F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXMS_DemoDlg dialog

class CXMS_DemoDlg : public CDialog
{
// Construction
public:
	CXMS_DemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXMS_DemoDlg)
	enum { IDD = IDD_XMS_DEMO_DIALOG };
	CListCtrl	m_ListCount;
	CListBox	m_ListMsg;
	CListCtrl	m_ListPcm;
	CListCtrl	m_ListMain;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_DemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXMS_DemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	afx_msg void OnBUTTONReOpen();
	afx_msg void OnBUTTONRemove();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_DEMODLG_H__F4740706_1E3B_4921_8560_1D1C8104097F__INCLUDED_)
