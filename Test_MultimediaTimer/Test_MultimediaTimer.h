// Test_MultimediaTimer.h : main header file for the TEST_MULTIMEDIATIMER application
//

#if !defined(AFX_TEST_MULTIMEDIATIMER_H__2CAE728F_9700_4DBA_A67C_3E73E13E8ACF__INCLUDED_)
#define AFX_TEST_MULTIMEDIATIMER_H__2CAE728F_9700_4DBA_A67C_3E73E13E8ACF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTest_MultimediaTimerApp:
// See Test_MultimediaTimer.cpp for the implementation of this class
//

class CTest_MultimediaTimerApp : public CWinApp
{
public:
	CTest_MultimediaTimerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest_MultimediaTimerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTest_MultimediaTimerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_MULTIMEDIATIMER_H__2CAE728F_9700_4DBA_A67C_3E73E13E8ACF__INCLUDED_)
