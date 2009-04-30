// XMS_VOIPTestDemo.h : main header file for the XMS_VOIPTESTDEMO application
//

#if !defined(AFX_XMS_VOIPTESTDEMO_H__6C1BD531_BE20_4F70_841D_BA0EAEE36654__INCLUDED_)
#define AFX_XMS_VOIPTESTDEMO_H__6C1BD531_BE20_4F70_841D_BA0EAEE36654__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMS_VOIPTestDemoApp:
// See XMS_VOIPTestDemo.cpp for the implementation of this class
//

class CXMS_VOIPTestDemoApp : public CWinApp
{
public:
	CXMS_VOIPTestDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_VOIPTestDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXMS_VOIPTestDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_VOIPTESTDEMO_H__6C1BD531_BE20_4F70_841D_BA0EAEE36654__INCLUDED_)
