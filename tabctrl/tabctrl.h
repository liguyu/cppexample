// tabctrl.h : main header file for the TABCTRL application
//

#if !defined(AFX_TABCTRL_H__351B85E3_D579_4D5B_AC63_3850B38CA89E__INCLUDED_)
#define AFX_TABCTRL_H__351B85E3_D579_4D5B_AC63_3850B38CA89E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTabctrlApp:
// See tabctrl.cpp for the implementation of this class
//

class CTabctrlApp : public CWinApp
{
public:
	CTabctrlApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabctrlApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTabctrlApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCTRL_H__351B85E3_D579_4D5B_AC63_3850B38CA89E__INCLUDED_)
