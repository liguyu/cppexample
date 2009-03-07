// DJIPPBXGateway.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DJIPPBXGateway.h"
#include "DJIPPBXGatewayDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDJIPPBXGatewayApp

BEGIN_MESSAGE_MAP(CDJIPPBXGatewayApp, CWinApp)
	//{{AFX_MSG_MAP(CDJIPPBXGatewayApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDJIPPBXGatewayApp construction

CDJIPPBXGatewayApp::CDJIPPBXGatewayApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDJIPPBXGatewayApp object

CDJIPPBXGatewayApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDJIPPBXGatewayApp initialization

BOOL CDJIPPBXGatewayApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	char chMdlFlName[MAX_PATH];
	GetModuleFileName(NULL, chMdlFlName, MAX_PATH);
	CString strMdlFlName = chMdlFlName;
	CString strLIni = strMdlFlName.Left(strMdlFlName.ReverseFind('\\')+1);
	strLIni += "IPGLanguage.ini";
	m_clsLIni.SetLIniPath(strLIni);

	HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, "CDJIPPBXGatewayApp");
	if( GetLastError() == ERROR_ALREADY_EXISTS )
	{
		AfxMessageBox(m_clsLIni.GLoadString("AppAlreadyExists"));
		return FALSE;
	}

	//OSVERSIONINFO OsVerInfo;
	//GetVersionEx(&OsVerInfo);

	CDJIPPBXGatewayDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	CloseHandle(hEvent);

	return FALSE;
}
