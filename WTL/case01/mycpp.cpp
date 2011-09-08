#include <windows.h>
#include "resource.h"

INT_PTR CALLBACK DefDialogProc(HWND, UINT, WPARAM, LPARAM);
//对话框类
class CDialog
{
public:
	CDialog(){ m_hWnd=NULL;	}						//构造函数
	~CDialog(){}									//析构函数
	enum { IDD = IDD_DLG_MAIN };
public:
	INT_PTR HandleMessage(UINT, WPARAM, LPARAM);	//消息处理函数
	INT_PTR DoDialogBox(HINSTANCE, UINT, HWND);
	INT_PTR OnCommand(WPARAM wParam, LPARAM lParam);

	void Attach(HWND hWnd){ m_hWnd = hWnd; }		//将创建窗口时的句柄hWnd，绑定到对象成员m_hWnd

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
		pThis=(CDialog*)lParam;		//获取对象指针
		if( NULL != pThis)
		{
			pThis->Attach(hWnd);	//设置HWND和对象之间的关系
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);	//将对象指针保存到窗口hWnd用户数据空间
		}
		break;
	default:
		break;
	}
	pThat = (CDialog*)GetWindowLongPtr(hWnd, GWLP_USERDATA);	//从窗口句柄hWnd得到对象指针，因为之前有SetWindowLongPtr()
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
