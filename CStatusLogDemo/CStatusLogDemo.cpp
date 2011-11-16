// CStatusLogDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CStatusLogDemo.h"
#include "CStatusLogDemoDlg.h"
#include "SAStatusLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCStatusLogDemoApp

BEGIN_MESSAGE_MAP(CCStatusLogDemoApp, CWinApp)
	//{{AFX_MSG_MAP(CCStatusLogDemoApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCStatusLogDemoApp construction

CCStatusLogDemoApp::CCStatusLogDemoApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCStatusLogDemoApp object

CCStatusLogDemoApp theApp;

// the one and only CStatusLog object
CSAStatusLog g_statusLog;


/////////////////////////////////////////////////////////////////////////////
// CCStatusLogDemoApp initialization

BOOL CCStatusLogDemoApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

   g_statusLog.Init("StatusLog.txt");
   
   g_statusLog.PrintTime(TRUE);
   g_statusLog.PrintAppName(TRUE);

   g_statusLog.StatusOut("%s", "Howdy, stranger!");
   g_statusLog.StatusOut("Did you know that %d * %4.2f = %4.2f ?", 15, 32.434, (15 * 32.434));
   g_statusLog.StatusOut("Ain't that great?");
   g_statusLog.StatusOut("I'm at : 0x%x", this);

   g_statusLog.StatusOut("Is anyone else here?");

	CCStatusLogDemoDlg dlg;
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

   g_statusLog.StatusOut("Bye bye"); 

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
