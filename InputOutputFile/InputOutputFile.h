// InputOutputFile.h : main header file for the INPUTOUTPUTFILE application
//

#if !defined(AFX_INPUTOUTPUTFILE_H__B24686CE_D7FF_42E0_BE76_12B94E941757__INCLUDED_)
#define AFX_INPUTOUTPUTFILE_H__B24686CE_D7FF_42E0_BE76_12B94E941757__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CInputOutputFileApp:
// See InputOutputFile.cpp for the implementation of this class
//

class CInputOutputFileApp : public CWinApp
{
public:
	CInputOutputFileApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputOutputFileApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CInputOutputFileApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTOUTPUTFILE_H__B24686CE_D7FF_42E0_BE76_12B94E941757__INCLUDED_)
