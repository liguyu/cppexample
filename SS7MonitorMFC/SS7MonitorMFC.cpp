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
		//   ��ȡ��׼����豸���  
		CONSOLE_SCREEN_BUFFER_INFO   bInfo;   //   ���ڻ�������Ϣ  
		GetConsoleScreenBufferInfo(hOut,   &bInfo   );  
		//   ��ȡ���ڻ�������Ϣ  
		
		SetConsoleTextAttribute(hOut,   FOREGROUND_GREEN);  
		char   strTitle[255];  
		GetConsoleTitle(strTitle,   255);   //   ��ȡ���ڱ���  
		printf("��ǰ���ڱ����ǣ�%s\n",   strTitle);  
		_getch();  
		SetConsoleTitle("����̨���ڲ���");   //   ��ȡ���ڱ���  
		_getch();  
		COORD   size   =   {80,   25};  
		SetConsoleScreenBufferSize(hOut,size);   //   �������û�������С  
		_getch();  
		SMALL_RECT   rc   =   {0,0,   80-1,   25-1};   //   ���ô���λ�úʹ�С  
		SetConsoleWindowInfo(hOut,true   ,&rc);  
		CloseHandle(hOut);   //   �رձ�׼����豸���   
		
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
		
		lvc.pszText = _T("����");
		lvc.cx = 60;
		listctrl.InsertColumn(0,&lvc);
		
		lvc.pszText = _T("�ƺ�");
		lvc.cx= 60;
		listctrl.InsertColumn(1,&lvc);
		
		lvc.pszText = _T("�ܼ���");
		lvc.cx= 60;
		listctrl.InsertColumn(2,&lvc);
		
		lvc.pszText = _T("�꼶");
		lvc.cx = 60;
		listctrl.InsertColumn(3,&lvc);
		
		lvc.pszText = _T("ʡ��");
		lvc.cx = 60;
		listctrl.InsertColumn(4,&lvc);
		
		lvc.pszText = _T("�Ա�");
		lvc.cx= 60;
		listctrl.InsertColumn(5,&lvc);
		
		lvc.pszText = _T("ѧ��");
		lvc.cx = 100;
		listctrl.InsertColumn(6,&lvc);
		//SetTextBkColor(RGB(88,88,88));
	//	SetTextColor(RGB(00,00,255));
		//InsertItem(0,"xler");
		listctrl.SetItemText(0,1,"ħ��ʦ");
		listctrl.SetItemText(0,2,"Сƨƨ");
		listctrl.SetItemText(0,3,"һ�꼶");
		listctrl.SetItemText(0,4,"��԰ʡ");
		listctrl.SetItemText(0,5,"��");
		listctrl.SetItemText(0,6,"vcer0001");
//		SetTextBkColor(RGB(00,66,88));
	//	SetTextColor(RGB(00,255,00));
	//	InsertItem(1,"bluejoe");
		listctrl.SetItemText(1,1,"ħ��ʦ");
		listctrl.SetItemText(1,2,"Сƨƨ");
		listctrl.SetItemText(1,3,"һ�꼶");
		listctrl.SetItemText(1,4,"��԰ʡ");
		listctrl.SetItemText(1,5,"��");
		listctrl.SetItemText(1,6,"vcer0002");
///		SetTextBkColor(RGB(66,00,88));
	//	SetTextColor(RGB(255,00,00));
	//	InsertItem(2,"jerry");
		listctrl.SetItemText(2,1,"ħ��ʦ");
		listctrl.SetItemText(2,2,"Сƨƨ");
		listctrl.SetItemText(2,3,"һ�꼶");
		listctrl.SetItemText(2,4,"��԰ʡ");
		listctrl.SetItemText(2,5,"��");
	listctrl.SetItemText(2,6,"vcer0003");


	}

	return nRetCode;
}


