#if !defined(AFX_DIALOGFLASHSET_H__4FF2D5C8_E0AE_4E4E_8E27_D31976E5DC9A__INCLUDED_)
#define AFX_DIALOGFLASHSET_H__4FF2D5C8_E0AE_4E4E_8E27_D31976E5DC9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogFlashSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogFlashSet dialog

class CDialogFlashSet : public CDialog
{
// Construction
public:
	CDialogFlashSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogFlashSet)
	enum { IDD = IDD_DIALOG_PARAM_Flash };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogFlashSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogFlashSet)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGFLASHSET_H__4FF2D5C8_E0AE_4E4E_8E27_D31976E5DC9A__INCLUDED_)
