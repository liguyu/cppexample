#include <windows.h>
#include "resource.h"

INT_PTR CALLBACK DefDialogProc(HWND, UINT, WPARAM, LPARAM);
///////////////////////////////////////////////////////////////////////
class CDialog
{
public:
	CDialog(){ m_hWnd=NULL;	}
	~CDialog(){}
	enum { IDD = IDD_DLG_MAIN };
public:
	INT_PTR HandleMessage(UINT, WPARAM, LPARAM);
	INT_PTR DoDialogBox(HINSTANCE, UINT, HWND);
	INT_PTR OnCommand(WPARAM wParam, LPARAM lParam);

	void Attach(HWND hWnd){ m_hWnd = hWnd; }

private:
	HWND m_hWnd;//窗口句柄，对象通过m_hWnd找到窗口
};

INT_PTR CDialog::DoDialogBox(HINSTANCE hInst, UINT nIDTemplate, HWND hWndParent)
{
	return DialogBoxParam(hInst, MAKEINTRESOURCE(nIDTemplate), hWndParent, DefDialogProc,(LPARAM)this);
}
//消息处理函数
INT_PTR CDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:	return FALSE;
	case WM_COMMAND:	return OnCommand(wParam, lParam);
	default:
		break;
	}
	return FALSE;
}
INT_PTR CDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch(LOWORD(wParam))
	{
	
	}
}

INT_PTR CALLBACK DefDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CDialog* pThis = NULL;
	CDialog* pThat = NULL
	switch(uMsg)
	{
	case WM_INITDIALOG:
		pThis=(CDialog*)lParam;
		if()
		{
		}
		break;
	default:
		break;
	}

}