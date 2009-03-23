#if !defined(AFX_DIALOGANATRUNK_H__CCC1AA56_F408_495F_9314_8CEC41081AD8__INCLUDED_)
#define AFX_DIALOGANATRUNK_H__CCC1AA56_F408_495F_9314_8CEC41081AD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogAnaTrunk.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogAnaTrunk dialog

class CDialogAnaTrunk : public CDialog
{
// Construction
public:
	CDialogAnaTrunk(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogAnaTrunk)
	enum { IDD = IDD_DIALOG_ANATRUNK };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogAnaTrunk)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogAnaTrunk)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGANATRUNK_H__CCC1AA56_F408_495F_9314_8CEC41081AD8__INCLUDED_)
