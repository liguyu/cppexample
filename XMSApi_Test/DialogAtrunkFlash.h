#if !defined(AFX_DIALOGATRUNKFLASH_H__F3C12D52_EDF1_4DBA_8CC7_93186CB9E572__INCLUDED_)
#define AFX_DIALOGATRUNKFLASH_H__F3C12D52_EDF1_4DBA_8CC7_93186CB9E572__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogAtrunkFlash.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DialogAtrunkFlash dialog

class DialogAtrunkFlash : public CDialog
{
// Construction
public:
	DialogAtrunkFlash(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(DialogAtrunkFlash)
	enum { IDD = IDD_DIALOG_PARAM_ATRUNKFLASH };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DialogAtrunkFlash)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DialogAtrunkFlash)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGATRUNKFLASH_H__F3C12D52_EDF1_4DBA_8CC7_93186CB9E572__INCLUDED_)
