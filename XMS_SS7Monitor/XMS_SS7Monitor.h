// XMS_SS7Monitor.h : main header file for the XMS_SS7MONITOR application
//

#if !defined(AFX_XMS_SS7MONITOR_H__429DD6A9_C41F_46A2_835D_E6F4DDDCA5A8__INCLUDED_)
#define AFX_XMS_SS7MONITOR_H__429DD6A9_C41F_46A2_835D_E6F4DDDCA5A8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CXMS_SS7MonitorApp:
// See XMS_SS7Monitor.cpp for the implementation of this class
//

class CXMS_SS7MonitorApp : public CWinApp
{
public:
	CXMS_SS7MonitorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXMS_SS7MonitorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CXMS_SS7MonitorApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XMS_SS7MONITOR_H__429DD6A9_C41F_46A2_835D_E6F4DDDCA5A8__INCLUDED_)
