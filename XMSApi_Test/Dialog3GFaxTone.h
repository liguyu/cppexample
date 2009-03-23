#if !defined(AFX_DIALOG3GFAXTONE_H__CA4D93D0_204B_4BD9_857A_1F2311C8831D__INCLUDED_)
#define AFX_DIALOG3GFAXTONE_H__CA4D93D0_204B_4BD9_857A_1F2311C8831D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog3GFaxTone.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog3GFaxTone dialog

class CDialog3GFaxTone : public CDialog
{
// Construction
public:
	CDialog3GFaxTone(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog3GFaxTone)
	enum { IDD = IDD_DIALOG_PARAM_3GFaxTone };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog3GFaxTone)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog3GFaxTone)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG3GFAXTONE_H__CA4D93D0_204B_4BD9_857A_1F2311C8831D__INCLUDED_)
