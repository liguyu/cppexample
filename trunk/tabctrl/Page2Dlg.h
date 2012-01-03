#if !defined(AFX_PAGE2DLG_H__84B73CDF_28E6_4D74_BD76_08615ECF283A__INCLUDED_)
#define AFX_PAGE2DLG_H__84B73CDF_28E6_4D74_BD76_08615ECF283A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Page2Dlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Page2Dlg dialog

class Page2Dlg : public CDialog
{
// Construction
public:
	Page2Dlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(Page2Dlg)
	enum { IDD = IDD_DIALOG2 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Page2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Page2Dlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGE2DLG_H__84B73CDF_28E6_4D74_BD76_08615ECF283A__INCLUDED_)
