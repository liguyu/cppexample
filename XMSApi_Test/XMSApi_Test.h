// XMSApi_Test.h : main header file for the XMSAPI_TEST application
//

#if !defined(AFX_XMSAPI_TEST_H__702570CF_6DA8_477C_82E6_A839CD1599B5__INCLUDED_)
#define AFX_XMSAPI_TEST_H__702570CF_6DA8_477C_82E6_A839CD1599B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMSApi_TestApp:
// See XMSApi_Test.cpp for the implementation of this class
//

class CXMSApi_TestApp : public CWinApp
{
public:
	CXMSApi_TestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMSApi_TestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXMSApi_TestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMSAPI_TEST_H__702570CF_6DA8_477C_82E6_A839CD1599B5__INCLUDED_)
