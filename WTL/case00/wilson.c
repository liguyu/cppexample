#include <windows.h>
#include "resource.h"
#include "wilson.h"

HINSTANCE g_hInst;
char szWindowClass[] = "MyWindows";
char szTitle[] = "My First Windows Program";


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow	)
{
	MSG msg;
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MyRegisterClass(hInstance);

	if(!InitInstance(hInstance,nCmdShow))
	{
		return FALSE;
	}

	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

BOOL MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MAIN_ICON));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDM_MAIN_MENU);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL_ICON));

	return RegisterClassEx(&wcex);

}

BOOL	InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;
	g_hInst = hInstance;

	hWnd = CreateWindow(szWindowClass,szTitle,WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,0,CW_USEDEFAULT,0,NULL,NULL,hInstance,NULL);
	if(!hWnd)
	{
		return FALSE;
	
	}
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);
	return TRUE;

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId,wmEvent;
	PAINTSTRUCT ps;

	HDC hdc;
	switch(message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch(wmId)
		{
		case IDM_ABOUT:
			DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd,&ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd,message, wParam, lParam);
	
	}
	return 0;

}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch(message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if(LOWORD(wParam)==IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

