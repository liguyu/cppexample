// XMS_SMonitor.h : main header file for the XMS_SMONITOR application
//

#if !defined(AFX_XMS_SMONITOR_H__4A791E42_87B0_4693_BC33_A7DD00F0980E__INCLUDED_)
#define AFX_XMS_SMONITOR_H__4A791E42_87B0_4693_BC33_A7DD00F0980E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMS_SMonitorApp:
// See XMS_SMonitor.cpp for the implementation of this class
//

class CXMS_SMonitorApp : public CWinApp
{
public:
	CXMS_SMonitorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_SMonitorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXMS_SMonitorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_SMONITOR_H__4A791E42_87B0_4693_BC33_A7DD00F0980E__INCLUDED_)
