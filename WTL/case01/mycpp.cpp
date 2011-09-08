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
	case IDC_BTN_CLOSE:
		EndDialog(m_hWnd,IDOK);
		return TRUE;
	default:
		break;	
	}
	return FALSE;
}

INT_PTR CALLBACK DefDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CDialog* pThis = NULL;
	CDialog* pThat = NULL;
	switch(uMsg)
	{
	case WM_INITDIALOG:
		pThis=(CDialog*)lParam;
		if( NULL != pThis)
		{
			pThis->Attach(hWnd);//设置HWND和C++ Object之间的关系
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);
		}
		break;
	default:
		break;
	}
	pThat = (CDialog*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if(NULL != pThat)
	{
		return ( pThat->HandleMessage(uMsg,wParam, lParam) );
	}

	return (INT_PTR)FALSE;
}

int APIENTRY WinMain( HINSTANCE hInstance, 
					 HINSTANCE hPrevInstance, 
					 LPSTR lpCmdLine, 
					 int nCmdShow)
{
	CDialog dlg;
	dlg.DoDialogBox(hInstance, CDialog::IDD, GetDesktopWindow());
	return 0;
}
