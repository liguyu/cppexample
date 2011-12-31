// LoadAging.cpp : main source file for LoadAging.exe
//

#include "stdafx.h"

#include "resource.h"

#include "MainDlg.h"

CAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
	HRESULT hRes = ::CoInitialize(NULL);		//��ʼ��COM
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);		// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);		//ȫ�ֶ���_Module����ʼ��
	ATLASSERT(SUCCEEDED(hRes));

	BkString::Load(IDR_BK_STRING_DEF);			//����ָ����ԴID��string����xml
	BkFontPool::SetDefaultFont(BkString::Get(IDS_APP_FONT), -12);

	BkSkin::LoadSkins(IDR_BK_SKIN_DEF);			//����ָ����ԴID��skin����xml
	BkStyle::LoadStyles(IDR_BK_STYLE_DEF);		//����ָ����ԴID��Style����xml

	int nRet = 0;
	// BLOCK: Run application, ��CMainDlg��������һ���������Ǻ���Ҫ��
	{
		CMainDlg dlgMain;
		nRet = dlgMain.DoModal();
	}

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
