// XMS_RecDemo.h : main header file for the XMS_RECDEMO application
//

#if !defined(AFX_XMS_RECDEMO_H__7ADFAC4B_2E71_4B8D_9D0E_3E4385F410D6__INCLUDED_)
#define AFX_XMS_RECDEMO_H__7ADFAC4B_2E71_4B8D_9D0E_3E4385F410D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMS_RecDemoApp:
// See XMS_RecDemo.cpp for the implementation of this class
//

class CXMS_RecDemoApp : public CWinApp
{
public:
	CXMS_RecDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_RecDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXMS_RecDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_RECDEMO_H__7ADFAC4B_2E71_4B8D_9D0E_3E4385F410D6__INCLUDED_)
