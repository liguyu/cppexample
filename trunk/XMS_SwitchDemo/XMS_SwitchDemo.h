// XMS_SwitchDemo.h : main header file for the XMS_SWITCHDEMO application
//

#if !defined(AFX_XMS_SWITCHDEMO_H__6F37C108_9D2C_473D_A489_C42AABCAD403__INCLUDED_)
#define AFX_XMS_SWITCHDEMO_H__6F37C108_9D2C_473D_A489_C42AABCAD403__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMS_SwitchDemoApp:
// See XMS_SwitchDemo.cpp for the implementation of this class
//

class CXMS_SwitchDemoApp : public CWinApp
{
public:
	CXMS_SwitchDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_SwitchDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXMS_SwitchDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_SWITCHDEMO_H__6F37C108_9D2C_473D_A489_C42AABCAD403__INCLUDED_)
