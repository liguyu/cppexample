// XMSTest_FaxDial.h : main header file for the XMSTEST_FAXDIAL application
//

#if !defined(AFX_XMSTEST_FAXDIAL_H__D0587131_478D_4378_87D1_6D5A0C1C84D8__INCLUDED_)
#define AFX_XMSTEST_FAXDIAL_H__D0587131_478D_4378_87D1_6D5A0C1C84D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMSTest_FaxDialApp:
// See XMSTest_FaxDial.cpp for the implementation of this class
//

class CXMSTest_FaxDialApp : public CWinApp
{
public:
	CXMSTest_FaxDialApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMSTest_FaxDialApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXMSTest_FaxDialApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMSTEST_FAXDIAL_H__D0587131_478D_4378_87D1_6D5A0C1C84D8__INCLUDED_)
