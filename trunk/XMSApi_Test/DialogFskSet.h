#if !defined(AFX_DIALOGFSKSET_H__B3E8BAB3_1851_4624_93FC_95D5922D57DD__INCLUDED_)
#define AFX_DIALOGFSKSET_H__B3E8BAB3_1851_4624_93FC_95D5922D57DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogFskSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogFskSet dialog

class CDialogFskSet : public CDialog
{
// Construction
public:
	CDialogFskSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogFskSet)
	enum { IDD = IDD_DIALOG_PARAM_FSK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogFskSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogFskSet)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGFSKSET_H__B3E8BAB3_1851_4624_93FC_95D5922D57DD__INCLUDED_)
