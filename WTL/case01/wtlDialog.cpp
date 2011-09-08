#include <atlbase.h>
#include <atlwin.h>
#include <atlapp.h>

#include "resource.h"

class CHelloDlg: public CDialogImpl<CHelloDlg>
{
public:
	enum{IDD = IDD_DLG_MAIN};
public:
	BEGIN_MSG_MAP(CHelloDlg)
		COMMAND_ID_HANDLER(IDC_BTN_CLOSE, OnClose);
	END_MSG_MAP();

	LRESULT CHelloDlg::OnClose(WORD, WORD wID, HWND, BOOL&)
	{
		EndDialog(IDOK);
		return 0;
	}
};

CAppModule _Module;

int APIENTRY WinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPSTR lpCmdLine,
					 int nCmdShow)
{
	_Module.Init(NULL, hInstance);
	{
		CHelloDlg dlg;
		dlg.DoModal();
	}
	_Module.Term();
	return 0;
}

