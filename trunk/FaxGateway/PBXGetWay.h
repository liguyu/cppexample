// PBXGetWay.h : main header file for the PBXGETWAY application
//

#if !defined(AFX_PBXGETWAY_H__3F9FA975_B77C_4224_948F_A81019F7FFD6__INCLUDED_)
#define AFX_PBXGETWAY_H__3F9FA975_B77C_4224_948F_A81019F7FFD6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPBXGetWayApp:
// See PBXGetWay.cpp for the implementation of this class
//

class CPBXGetWayApp : public CWinApp
{
public:
	CPBXGetWayApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPBXGetWayApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CPBXGetWayApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PBXGETWAY_H__3F9FA975_B77C_4224_948F_A81019F7FFD6__INCLUDED_)
