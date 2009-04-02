// XMS_TestDial.h : main header file for the XMS_TESTDIAL application
//

#if !defined(AFX_XMS_TESTDIAL_H__249A6BD4_632D_4D1D_B2F3_4D92F6693911__INCLUDED_)
#define AFX_XMS_TESTDIAL_H__249A6BD4_632D_4D1D_B2F3_4D92F6693911__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMS_TestDialApp:
// See XMS_TestDial.cpp for the implementation of this class
//

class CXMS_TestDialApp : public CWinApp
{
public:
	CXMS_TestDialApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_TestDialApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXMS_TestDialApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_TESTDIAL_H__249A6BD4_632D_4D1D_B2F3_4D92F6693911__INCLUDED_)
