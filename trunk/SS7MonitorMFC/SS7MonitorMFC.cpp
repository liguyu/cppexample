// SS7MonitorMFC.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SS7MonitorMFC.h"
#include <conio.h>   

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
		CString strHello;
		strHello.LoadString(IDS_HELLO);
		cout << (LPCTSTR)strHello << endl;
		
		HANDLE   hOut   =   GetStdHandle(STD_OUTPUT_HANDLE);  
		//   获取标准输出设备句柄  
		CONSOLE_SCREEN_BUFFER_INFO   bInfo;   //   窗口缓冲区信息  
		GetConsoleScreenBufferInfo(hOut,   &bInfo   );  
		//   获取窗口缓冲区信息  
		
		SetConsoleTextAttribute(hOut,   FOREGROUND_GREEN);  
		char   strTitle[255];  
		GetConsoleTitle(strTitle,   255);   //   获取窗口标题  
		printf("当前窗口标题是：%s\n",   strTitle);  
		_getch();  
		SetConsoleTitle("控制台窗口操作");   //   获取窗口标题  
		_getch();  
		COORD   size   =   {80,   25};  
		SetConsoleScreenBufferSize(hOut,size);   //   重新设置缓冲区大小  
		_getch();  
		SMALL_RECT   rc   =   {0,0,   80-1,   25-1};   //   重置窗口位置和大小  
		SetConsoleWindowInfo(hOut,true   ,&rc);  
		CloseHandle(hOut);   //   关闭标准输出设备句柄   
		
		CRect rect(0,0,0,0);
		CListCtrl listctrl;
		listctrl.Create( WS_CHILD|
			WS_VISIBLE|
			WS_VSCROLL|
			LVS_REPORT|
			LVS_SHOWSELALWAYS|
			LVS_SINGLESEL|
			LVS_OWNERDRAWFIXED|
			LVS_NOLABELWRAP|
			WS_EX_CLIENTEDGE,
			rect,
			hOut,
			hOut);
		listctrl.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);	
		listctrl.SetBkColor(RGB(184,189,229));
		//---------Insert Column-------------------------------
		
		LV_COLUMN lvc;
		lvc.mask = LVCF_TEXT | LVCF_SUBITEM | LVCF_WIDTH;
		
		lvc.pszText = _T("姓名");
		lvc.cx = 60;
		listctrl.InsertColumn(0,&lvc);
		
		lvc.pszText = _T("称号");
		lvc.cx= 60;
		listctrl.InsertColumn(1,&lvc);
		
		lvc.pszText = _T("总级别");
		lvc.cx= 60;
		listctrl.InsertColumn(2,&lvc);
		
		lvc.pszText = _T("年级");
		lvc.cx = 60;
		listctrl.InsertColumn(3,&lvc);
		
		lvc.pszText = _T("省份");
		lvc.cx = 60;
		listctrl.InsertColumn(4,&lvc);
		
		lvc.pszText = _T("性别");
		lvc.cx= 60;
		listctrl.InsertColumn(5,&lvc);
		
		lvc.pszText = _T("学号");
		lvc.cx = 100;
		listctrl.InsertColumn(6,&lvc);
		//SetTextBkColor(RGB(88,88,88));
	//	SetTextColor(RGB(00,00,255));
		//InsertItem(0,"xler");
		listctrl.SetItemText(0,1,"魔法师");
		listctrl.SetItemText(0,2,"小屁屁");
		listctrl.SetItemText(0,3,"一年级");
		listctrl.SetItemText(0,4,"花园省");
		listctrl.SetItemText(0,5,"男");
		listctrl.SetItemText(0,6,"vcer0001");
//		SetTextBkColor(RGB(00,66,88));
	//	SetTextColor(RGB(00,255,00));
	//	InsertItem(1,"bluejoe");
		listctrl.SetItemText(1,1,"魔法师");
		listctrl.SetItemText(1,2,"小屁屁");
		listctrl.SetItemText(1,3,"一年级");
		listctrl.SetItemText(1,4,"花园省");
		listctrl.SetItemText(1,5,"男");
		listctrl.SetItemText(1,6,"vcer0002");
///		SetTextBkColor(RGB(66,00,88));
	//	SetTextColor(RGB(255,00,00));
	//	InsertItem(2,"jerry");
		listctrl.SetItemText(2,1,"魔法师");
		listctrl.SetItemText(2,2,"小屁屁");
		listctrl.SetItemText(2,3,"一年级");
		listctrl.SetItemText(2,4,"花园省");
		listctrl.SetItemText(2,5,"男");
	listctrl.SetItemText(2,6,"vcer0003");


	}

	return nRetCode;
}


