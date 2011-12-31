// LoadAging.cpp : main source file for LoadAging.exe
//

#include "stdafx.h"

#include "resource.h"

#include "MainDlg.h"

CAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
	HRESULT hRes = ::CoInitialize(NULL);		//初始化COM
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);		// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);		//全局对象_Module被初始化
	ATLASSERT(SUCCEEDED(hRes));

	BkString::Load(IDR_BK_STRING_DEF);			//加载指定资源ID的string定义xml
	BkFontPool::SetDefaultFont(BkString::Get(IDS_APP_FONT), -12);

	BkSkin::LoadSkins(IDR_BK_SKIN_DEF);			//加载指定资源ID的skin定义xml
	BkStyle::LoadStyles(IDR_BK_STYLE_DEF);		//加载指定资源ID的Style定义xml

	int nRet = 0;
	// BLOCK: Run application, 将CMainDlg变量放在一个区块中是很重要的
	{
		CMainDlg dlgMain;
		nRet = dlgMain.DoModal();
	}

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
