// HelloKeygoe.h : main header file for the HELLOKEYGOE application
//

#if !defined(AFX_HELLOKEYGOE_H__09CB7353_B2BE_46CB_B1B4_AD49F59AE59C__INCLUDED_)
#define AFX_HELLOKEYGOE_H__09CB7353_B2BE_46CB_B1B4_AD49F59AE59C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CHelloKeygoeApp:
// See HelloKeygoe.cpp for the implementation of this class
//

class CHelloKeygoeApp : public CWinApp
{
public:
	CHelloKeygoeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHelloKeygoeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHelloKeygoeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HELLOKEYGOE_H__09CB7353_B2BE_46CB_B1B4_AD49F59AE59C__INCLUDED_)
