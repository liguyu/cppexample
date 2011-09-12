// program67.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include <windows.h>

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
	//�ص�����������Ϊstatic����ͨ�������ᴫһ��this
    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
		HDC hDC;
		PAINTSTRUCT ps;
		RECT rect;
        switch (uMsg)
        {
		case WM_PAINT:
			hDC = ::BeginPaint(hWnd, &ps);
			::GetClientRect(hWnd, &rect);
			::DrawText(hDC, "Window��API���м򵥷�װ����װΪZWindow", -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
			::EndPaint(hWnd, &ps);
			break;
        case WM_DESTROY:
            ::PostQuitMessage(0);
            break;
        }
		
        return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,  
                   int nCmdShow)
{
    char szAppName[] = "Hello world";
    HWND hWnd;
    MSG msg;
    WNDCLASS wnd;
    ZWindow zwnd;
    
    wnd.cbClsExtra    = NULL;
    wnd.cbWndExtra    = NULL;
    wnd.hbrBackground    = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wnd.hCursor        = LoadCursor(NULL, IDC_ARROW);
    wnd.hIcon        = LoadIcon(NULL, IDI_APPLICATION);
    wnd.hInstance        = hInstance;
    wnd.lpfnWndProc    = ZWindow::WndProc;
    wnd.lpszClassName    = szAppName;
    wnd.lpszMenuName    = NULL;
    wnd.style        = CS_HREDRAW | CS_VREDRAW;
    //ע�ᴰ����
    if (!RegisterClass(&wnd))
    {
        MessageBox(NULL, "Can not register window class", "Error", 
			MB_OK | MB_ICONINFORMATION);
        return -1;
    }
    //��������
    hWnd = CreateWindow(szAppName, "Hello world", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
	//���ھ���󶨵� ZWindow���Ա����m_hWnd
    zwnd.Attach(hWnd);
    // ���Ա����,��ʾ����
    zwnd.ShowWindow(nCmdShow);
    //���´���
	zwnd.UpdateWindow();
	//��ȡ������Ϣ
    while (GetMessage(&msg, NULL, 0, 0))
    {
		//�ַ���Ϣ
        DispatchMessage(&msg);
    }
    
    return msg.wParam;
}



