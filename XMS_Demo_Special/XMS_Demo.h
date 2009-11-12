// XMS_Demo.h : main header file for the XMS_DEMO application
//

#if !defined(AFX_XMS_DEMO_H__5CEC0A23_DE01_4045_9AB5_B2928B657364__INCLUDED_)
#define AFX_XMS_DEMO_H__5CEC0A23_DE01_4045_9AB5_B2928B657364__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMS_DemoApp:
// See XMS_Demo.cpp for the implementation of this class
//

class CXMS_DemoApp : public CWinApp
{
public:
	CXMS_DemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_DemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXMS_DemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_DEMO_H__5CEC0A23_DE01_4045_9AB5_B2928B657364__INCLUDED_)
