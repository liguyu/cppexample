// MemoryDraw.h : main header file for the MEMORYDRAW application
//

#if !defined(AFX_MEMORYDRAW_H__A246CF17_7EBE_4B46_8D07_AA00C440DD67__INCLUDED_)
#define AFX_MEMORYDRAW_H__A246CF17_7EBE_4B46_8D07_AA00C440DD67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMemoryDrawApp:
// See MemoryDraw.cpp for the implementation of this class
//

class CMemoryDrawApp : public CWinApp
{
public:
	CMemoryDrawApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemoryDrawApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMemoryDrawApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMORYDRAW_H__A246CF17_7EBE_4B46_8D07_AA00C440DD67__INCLUDED_)
