#if !defined(AFX_RECCSPDLG_H__17C24D14_3A43_4555_98B5_63AF35381DCF__INCLUDED_)
#define AFX_RECCSPDLG_H__17C24D14_3A43_4555_98B5_63AF35381DCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RecCSPDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRecCSPDlg dialog

class CRecCSPDlg : public CDialog
{
// Construction
public:
	CRecCSPDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRecCSPDlg)
	enum { IDD = IDD_DIALOG_RECCSP };
	CEdit	m_PackLen;
	CComboBox	m_SrcMode;
	CComboBox	m_EncodeType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRecCSPDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRecCSPDlg)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnButton2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECCSPDLG_H__17C24D14_3A43_4555_98B5_63AF35381DCF__INCLUDED_)
