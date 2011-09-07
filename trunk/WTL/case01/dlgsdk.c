#include <windows.h>
#include "resource.h"

BOOL CALLBACK DlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_COMMAND:
		EndDialog(hDlg, LOWORD(wParam));
		break;
	}
	return FALSE;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow)
{
	return DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_DLG_MAIN), NULL, DlgProc,0);
}

