#if !defined(AFX_PAGE1DLG_H__F3E22EF6_D9D7_42EA_8B6E_679043E17C33__INCLUDED_)
#define AFX_PAGE1DLG_H__F3E22EF6_D9D7_42EA_8B6E_679043E17C33__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page1Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Page1Dlg dialog

class Page1Dlg : public CDialog
{
// Construction
public:
	Page1Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Page1Dlg)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Page1Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Page1Dlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE1DLG_H__F3E22EF6_D9D7_42EA_8B6E_679043E17C33__INCLUDED_)
