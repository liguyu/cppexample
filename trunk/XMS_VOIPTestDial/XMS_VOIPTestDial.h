// XMS_VOIPTestDial.h : main header file for the XMS_VOIPTESTDIAL application
//

#if !defined(AFX_XMS_VOIPTESTDIAL_H__07E84025_ACDB_4E16_AB7C_BC0E317B150B__INCLUDED_)
#define AFX_XMS_VOIPTESTDIAL_H__07E84025_ACDB_4E16_AB7C_BC0E317B150B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMS_VOIPTestDialApp:
// See XMS_VOIPTestDial.cpp for the implementation of this class
//

class CXMS_VOIPTestDialApp : public CWinApp
{
public:
	CXMS_VOIPTestDialApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_VOIPTestDialApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXMS_VOIPTestDialApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_VOIPTESTDIAL_H__07E84025_ACDB_4E16_AB7C_BC0E317B150B__INCLUDED_)
