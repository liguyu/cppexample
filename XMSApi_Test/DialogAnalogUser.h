#if !defined(AFX_DIALOGANALOGUSER_H__9B2F1240_EFEF_4EBE_8C69_A4FD803B795C__INCLUDED_)
#define AFX_DIALOGANALOGUSER_H__9B2F1240_EFEF_4EBE_8C69_A4FD803B795C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogAnalogUser.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogAnalogUser dialog

class CDialogAnalogUser : public CDialog
{
// Construction
public:
	CDialogAnalogUser(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogAnalogUser)
	enum { IDD = IDD_DIALOG_ANAUSER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogAnalogUser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogAnalogUser)
	virtual BOOL OnInitDialog();
	afx_msg void OnButton1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGANALOGUSER_H__9B2F1240_EFEF_4EBE_8C69_A4FD803B795C__INCLUDED_)
