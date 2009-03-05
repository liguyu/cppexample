// KGEventView.h : main header file for the KGEVENTVIEW application
//

#if !defined(AFX_KGEVENTVIEW_H__95CFB4F8_C170_4C95_99F1_4883B1D17B0D__INCLUDED_)
#define AFX_KGEVENTVIEW_H__95CFB4F8_C170_4C95_99F1_4883B1D17B0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CKGEventViewApp:
// See KGEventView.cpp for the implementation of this class
//

class CKGEventViewApp : public CWinApp
{
public:
	CKGEventViewApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKGEventViewApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CKGEventViewApp)
	afx_msg void OnAppAbout();

		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KGEVENTVIEW_H__95CFB4F8_C170_4C95_99F1_4883B1D17B0D__INCLUDED_)
