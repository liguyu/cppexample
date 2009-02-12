// DJIPPBXGateway.h : main header file for the DJIPPBXGATEWAY application
//

#if !defined(AFX_DJIPPBXGATEWAY_H__40C460B1_32C0_4CFE_82A8_59DBFD8681D3__INCLUDED_)
#define AFX_DJIPPBXGATEWAY_H__40C460B1_32C0_4CFE_82A8_59DBFD8681D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDJIPPBXGatewayApp:
// See DJIPPBXGateway.cpp for the implementation of this class
//

class CDJIPPBXGatewayApp : public CWinApp
{
public:
	CLanguageIni m_clsLIni;
	CDJIPPBXGatewayApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDJIPPBXGatewayApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDJIPPBXGatewayApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

extern CDJIPPBXGatewayApp theApp;

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DJIPPBXGATEWAY_H__40C460B1_32C0_4CFE_82A8_59DBFD8681D3__INCLUDED_)
