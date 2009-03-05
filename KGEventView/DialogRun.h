#if !defined(AFX_DIALOGRUN_H__2C599BB0_1324_4D7F_8733_54B7760510E2__INCLUDED_)
#define AFX_DIALOGRUN_H__2C599BB0_1324_4D7F_8733_54B7760510E2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogRun.h : header file
//
#include "MainFrm.h"
/////////////////////////////////////////////////////////////////////////////
// DialogRun dialog

class DialogRun : public CDialog
{
// Construction
public:
	DialogRun(CWnd* pParent = NULL);   // standard constructor
    CMainFrame *pFrm;
	CString FilePath;
	CString m_IP;
	CString m_strPort;
// Dialog Data
	//{{AFX_DATA(DialogRun)
	enum { IDD = IDD_DLGRUN };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DialogRun)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(DialogRun)
	afx_msg void OnButtonFpath();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(DialogRun)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGRUN_H__2C599BB0_1324_4D7F_8733_54B7760510E2__INCLUDED_)
