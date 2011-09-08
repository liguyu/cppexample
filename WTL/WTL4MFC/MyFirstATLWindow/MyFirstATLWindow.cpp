// MyFirstATLWindow.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "MyWindow.h"

CComModule _Module;	//这个变量必须命名为_Module

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev,
                   LPSTR szCmdLine, int nCmdShow)
{
    _Module.Init(NULL, hInst);
 
CMyWindow wndMain;
MSG msg;
 
    // Create our main window
    if ( NULL == wndMain.Create ( NULL, CWindow::rcDefault, _T("My First ATL Window") ))
        return 1;   // window creation failed

    wndMain.ShowWindow(nCmdShow);
    wndMain.UpdateWindow();

    // Your standard Win32 message loop:
    while ( GetMessage ( &msg, NULL, 0, 0 ) > 0 )
        {
        TranslateMessage ( &msg );
        DispatchMessage ( &msg );
        }
 
    _Module.Term();
    return msg.wParam;
}
