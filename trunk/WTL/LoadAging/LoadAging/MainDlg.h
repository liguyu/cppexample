// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainDlg : public CBkDialogImpl<CMainDlg>
{
public:
	CMainDlg() : CBkDialogImpl<CMainDlg>(IDR_BK_MAIN_DIALOG)
	{
	}

public:
	BK_NOTIFY_MAP(IDC_RICHVIEW_WIN)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_SYS_CLOSE, OnBkBtnClose)
		BK_NOTIFY_ID_COMMAND(IDC_CHK_CHK1, OnCheck1)
		BK_NOTIFY_ID_COMMAND(IDC_CHK_MYCHK, OnMyCheck)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_MENU, OnMainMenu)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_MAX, OnMaxWindow)
		BK_NOTIFY_ID_COMMAND(IDC_BTN_MIN, OnMinWindow)
	BK_NOTIFY_MAP_END()

	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_BK_NOTIFY(IDC_RICHVIEW_WIN)
		MSG_WM_SYSCOMMAND(OnSysCommand)
		CHAIN_MSG_MAP(CBkDialogImpl<CMainDlg>)
		MSG_WM_INITDIALOG(OnInitDialog)
	END_MSG_MAP()

public:
	void OnBkBtnClose()
	{
		EndDialog(IDCANCEL);
	}
	void OnMainMenu()
	{
		::MessageBox(this->m_hWnd, _T("Main Menu!"), _T("Menu!"), MB_OK);
	}
	void OnCheck1()
	{
		//EndDialog(IDCANCEL);
		::MessageBox(this->m_hWnd, _T("Hello!"), NULL, MB_OK);
		return;
	}
	void OnMyCheck()
	{
		::MessageBox(this->m_hWnd, _T("My CheckBox!"), NULL, MB_OK);
	}
	LRESULT OnMaxWindow();
	LRESULT OnMinWindow();
	void	OnSysCommand(UINT nID, CPoint point);
	LRESULT OnInitDialog(HWND, LPARAM);
};
