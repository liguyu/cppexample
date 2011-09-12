// program69.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include <windows.h>


class ZWindow;

ZWindow* g_pWnd = NULL;//全局窗口类指针

class ZWindow
{
public:
    HWND m_hWnd;
	
    ZWindow(HWND hWnd = 0) : m_hWnd(hWnd) { }
	
    inline void Attach(HWND hWnd)
    { m_hWnd = hWnd; }
	
    inline BOOL ShowWindow(int nCmdShow)
    { return ::ShowWindow(m_hWnd, nCmdShow); }
	
    inline BOOL UpdateWindow()
    {  return ::UpdateWindow(m_hWnd); }
	
    inline HDC BeginPaint(LPPAINTSTRUCT ps)
    {  return ::BeginPaint(m_hWnd, ps); }
	
    inline BOOL EndPaint(LPPAINTSTRUCT ps)
    {  return ::EndPaint(m_hWnd, ps); }
	
    inline BOOL GetClientRect(LPRECT rect)
    {  return ::GetClientRect(m_hWnd, rect); }
	
    BOOL Create(LPCTSTR szClassName, LPCTSTR szTitle, HINSTANCE hInstance, 
		HWND hWndParent = 0,    DWORD dwStyle = WS_OVERLAPPEDWINDOW, 
		DWORD dwExStyle = 0, HMENU hMenu = 0)
    {
        m_hWnd = ::CreateWindowEx(dwExStyle, szClassName, szTitle, dwStyle, 
			CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
			CW_USEDEFAULT, hWndParent, hMenu, hInstance, NULL);
		
        return m_hWnd != NULL;
    }
	
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        ZWindow* pThis = g_pWnd;
        HDC hDC;
        PAINTSTRUCT ps;
        RECT rect;
		
        switch (uMsg)
        {
        case WM_PAINT:
            hDC = pThis->BeginPaint(&ps);//在静态成员函数中通过pThis调用非静态成员函数
            pThis->GetClientRect(&rect);
            ::DrawText(hDC, "在静态成员函数中通过pThis调用非静态成员函数", -1, &rect, 
				DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            pThis->EndPaint(&ps);
            break;
			
        case WM_DESTROY:
            ::PostQuitMessage(0);
            break;
        }
		
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine,   int nCmdShow)
{
    char szAppName[] = "Hello world";
    MSG msg;
    WNDCLASS wnd;
    ZWindow zwnd;
    
    wnd.cbClsExtra    = NULL;
    wnd.cbWndExtra    = NULL;
    wnd.hbrBackground    = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wnd.hCursor    = LoadCursor(NULL, IDC_ARROW);
    wnd.hIcon        = LoadIcon(NULL, IDI_APPLICATION);
    wnd.hInstance    = hInstance;
    wnd.lpfnWndProc    = zwnd.WndProc;
    wnd.lpszClassName    = szAppName;
    wnd.lpszMenuName    = NULL;
    wnd.style        = CS_HREDRAW | CS_VREDRAW;
    
    if (!RegisterClass(&wnd))
    {
        MessageBox(NULL, "Can not register window class", "Error", 
			MB_OK | MB_ICONINFORMATION);
        return -1;
    }
	
    g_pWnd = &zwnd;
    zwnd.Create(szAppName, "Hell world", hInstance);
    zwnd.ShowWindow(nCmdShow);
    zwnd.UpdateWindow();
	
    while (GetMessage(&msg, NULL, 0, 0))
    {
        DispatchMessage(&msg);
    }
    
    return msg.wParam;
}



