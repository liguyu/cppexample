// XMS_Dial.h : main header file for the XMS_DIAL application
//

#if !defined(AFX_XMS_DIAL_H__7268F56E_BF1A_416E_8516_2A962E4AFEEF__INCLUDED_)
#define AFX_XMS_DIAL_H__7268F56E_BF1A_416E_8516_2A962E4AFEEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMS_DialApp:
// See XMS_Dial.cpp for the implementation of this class
//

class CXMS_DialApp : public CWinApp
{
public:
	CXMS_DialApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_DialApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXMS_DialApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_DIAL_H__7268F56E_BF1A_416E_8516_2A962E4AFEEF__INCLUDED_)
