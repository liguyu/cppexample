// XMSTest_FAXDemo.h : main header file for the XMSTEST_FAXDEMO application
//

#if !defined(AFX_XMSTEST_FAXDEMO_H__EEA73164_7285_4784_B644_55E8A5E65AC1__INCLUDED_)
#define AFX_XMSTEST_FAXDEMO_H__EEA73164_7285_4784_B644_55E8A5E65AC1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMSTest_FAXDemoApp:
// See XMSTest_FAXDemo.cpp for the implementation of this class
//

class CXMSTest_FAXDemoApp : public CWinApp
{
public:
	CXMSTest_FAXDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMSTest_FAXDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXMSTest_FAXDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMSTEST_FAXDEMO_H__EEA73164_7285_4784_B644_55E8A5E65AC1__INCLUDED_)
