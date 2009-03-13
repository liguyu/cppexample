// XMS_HizRecDemoDlg.h : header file
//

#if !defined(AFX_XMS_HIZRECDEMODLG_H__5087E725_CBFD_4F97_8247_F518E3373009__INCLUDED_)
#define AFX_XMS_HIZRECDEMODLG_H__5087E725_CBFD_4F97_8247_F518E3373009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CXMS_HizRecDemoDlg dialog

class CXMS_HizRecDemoDlg : public CDialog
{
// Construction
public:
	CXMS_HizRecDemoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CXMS_HizRecDemoDlg)
	enum { IDD = IDD_XMS_HIZRECDEMO_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_HizRecDemoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CXMS_HizRecDemoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_HIZRECDEMODLG_H__5087E725_CBFD_4F97_8247_F518E3373009__INCLUDED_)
