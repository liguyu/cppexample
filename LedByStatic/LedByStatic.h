// LedByStatic.h : main header file for the LEDBYSTATIC application
//

#if !defined(AFX_LEDBYSTATIC_H__FF776DA4_1610_4231_AAE1_A41DC218BF71__INCLUDED_)
#define AFX_LEDBYSTATIC_H__FF776DA4_1610_4231_AAE1_A41DC218BF71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLedByStaticApp:
// See LedByStatic.cpp for the implementation of this class
//

class CLedByStaticApp : public CWinApp
{
public:
	CLedByStaticApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLedByStaticApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLedByStaticApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEDBYSTATIC_H__FF776DA4_1610_4231_AAE1_A41DC218BF71__INCLUDED_)
