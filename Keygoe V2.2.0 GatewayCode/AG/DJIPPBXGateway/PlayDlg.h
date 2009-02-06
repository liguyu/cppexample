#if !defined(AFX_PLAYDLG_H__68C33B5C_4D58_4904_8A3E_DD5E048DBBC7__INCLUDED_)
#define AFX_PLAYDLG_H__68C33B5C_4D58_4904_8A3E_DD5E048DBBC7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PlayDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPlayDlg dialog

class CPlayDlg : public CDialog
{
// Construction
public:
	CPlayDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPlayDlg)
	enum { IDD = IDD_DIALOG_PLAY };
	int		m_iDspID;
	int		m_iChID;
	CString	m_strFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPlayDlg)
	afx_msg void OnButtonFile();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYDLG_H__68C33B5C_4D58_4904_8A3E_DD5E048DBBC7__INCLUDED_)
