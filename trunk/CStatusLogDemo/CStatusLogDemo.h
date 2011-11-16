// CStatusLogDemo.h : main header file for the CSTATUSLOGDEMO application
//

#if !defined(AFX_CSTATUSLOGDEMO_H__3C809BF4_9DFD_11D3_A10D_00500402F30B__INCLUDED_)
#define AFX_CSTATUSLOGDEMO_H__3C809BF4_9DFD_11D3_A10D_00500402F30B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "SAStatusLog.h"

/////////////////////////////////////////////////////////////////////////////
// CCStatusLogDemoApp:
// See CStatusLogDemo.cpp for the implementation of this class
//

class CCStatusLogDemoApp : public CWinApp
{
public:
	CCStatusLogDemoApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCStatusLogDemoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CCStatusLogDemoApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

// one and only
extern CSAStatusLog g_statusLog;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSTATUSLOGDEMO_H__3C809BF4_9DFD_11D3_A10D_00500402F30B__INCLUDED_)
