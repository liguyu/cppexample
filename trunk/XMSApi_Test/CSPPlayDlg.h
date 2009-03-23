#if !defined(AFX_CSPPLAYDLG_H__87430523_6D79_4A1F_91A9_2497A1AFFAA3__INCLUDED_)
#define AFX_CSPPLAYDLG_H__87430523_6D79_4A1F_91A9_2497A1AFFAA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSPPlayDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCSPPlayDlg dialog

class CCSPPlayDlg : public CDialog
{
// Construction
public:
	CCSPPlayDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCSPPlayDlg)
	enum { IDD = IDD_DIALOG_PLAYCSP };
	CEdit	m_DataLen;
	CComboBox	m_DataType;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCSPPlayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCSPPlayDlg)
	afx_msg void OnButton1();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSPPLAYDLG_H__87430523_6D79_4A1F_91A9_2497A1AFFAA3__INCLUDED_)
