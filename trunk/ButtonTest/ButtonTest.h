// ButtonTest.h : main header file for the BUTTONTEST application
//

#if !defined(AFX_BUTTONTEST_H__D381AAC1_46FC_45B3_8276_9100951B2123__INCLUDED_)
#define AFX_BUTTONTEST_H__D381AAC1_46FC_45B3_8276_9100951B2123__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CButtonTestApp:
// See ButtonTest.cpp for the implementation of this class
//

class CButtonTestApp : public CWinApp
{
public:
	CButtonTestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonTestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CButtonTestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONTEST_H__D381AAC1_46FC_45B3_8276_9100951B2123__INCLUDED_)
