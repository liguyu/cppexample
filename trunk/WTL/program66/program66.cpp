// program66.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <windows.h>


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine,  int nCmdShow)
{
    char szAppName[] = "Hello world";
    HWND hWnd;
    MSG msg;
    WNDCLASS wnd;
    
    wnd.cbClsExtra    = NULL;
    wnd.cbWndExtra    = NULL;
    wnd.hbrBackground    = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wnd.hCursor    = LoadCursor(NULL, IDC_ARROW);
    wnd.hIcon        = LoadIcon(NULL, IDI_APPLICATION);
    wnd.hInstance    = hInstance;
    wnd.lpfnWndProc    = WndProc;	//回调函数,
    wnd.lpszClassName    = szAppName;
    wnd.lpszMenuName    = NULL;
    wnd.style        = CS_HREDRAW | CS_VREDRAW;
    //注册窗口类
    if (!RegisterClass(&wnd))
    {
        MessageBox(NULL, "Can not register window class", "Error", MB_OK | MB_ICONINFORMATION);
        return -1;
    }
    //创建窗口
    hWnd = CreateWindow(szAppName, "Hello world", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
    //显示窗口
    ShowWindow(hWnd, nCmdShow);
	//更新窗口
    UpdateWindow(hWnd);
    //获取窗口消息
    while (GetMessage(&msg, NULL, 0, 0))
    {
        DispatchMessage(&msg);	//分发窗口消息
    }
    
    return msg.wParam;
}
/************************************************************************/
/* 窗口消息处理函数，回调函数                                           */
/************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hDC;
    PAINTSTRUCT ps;
    RECT rect;
	
    switch (uMsg)
    {
    case WM_PAINT:
        hDC = BeginPaint(hWnd, &ps);
        GetClientRect(hWnd, &rect);
        DrawText(hDC, "Hello world, my first Win32 program.", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hWnd, &ps);
        break;
		
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    }
    
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}




