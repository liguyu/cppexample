// XMS_RecDemoDlg.h : header file
//

#if !defined(AFX_XMS_RECDEMODLG_H__8C10A0C3_CF52_4D0A_8244_321F830F7544__INCLUDED_)
#define AFX_XMS_RECDEMODLG_H__8C10A0C3_CF52_4D0A_8244_321F830F7544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXMS_RecDemoDlg dialog

class CXMS_RecDemoDlg : public CDialog
{
// Construction
public:
	CXMS_RecDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXMS_RecDemoDlg)
	enum { IDD = IDD_XMS_RECDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_RecDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXMS_RecDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_RECDEMODLG_H__8C10A0C3_CF52_4D0A_8244_321F830F7544__INCLUDED_)
