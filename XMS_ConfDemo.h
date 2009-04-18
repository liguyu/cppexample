// XMS_ConfDemo.h : main header file for the XMS_CONFDEMO application
//

#if !defined(AFX_XMS_CONFDEMO_H__C7B52CA2_680D_48D1_86A3_FC328EC84990__INCLUDED_)
#define AFX_XMS_CONFDEMO_H__C7B52CA2_680D_48D1_86A3_FC328EC84990__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMS_ConfDemoApp:
// See XMS_ConfDemo.cpp for the implementation of this class
//

class CXMS_ConfDemoApp : public CWinApp
{
public:
	CXMS_ConfDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_ConfDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXMS_ConfDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_CONFDEMO_H__C7B52CA2_680D_48D1_86A3_FC328EC84990__INCLUDED_)
