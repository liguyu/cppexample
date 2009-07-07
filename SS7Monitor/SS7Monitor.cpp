//**************************************
//SS7Monitor.cpp : Defines the entry point for the console application.
//VC console application graphic
//**************************************

#include "stdafx.h"
#include "stdlib.h"
#include <windows.h>
#include <iostream.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <conio.h>   


int main(int argc, char* argv[])

{
	/*
	char arg[200]={0};
	arg[0]='\"';
	strcpy(arg+1, argv[0]);
	int len=int(strlen(arg));
	arg[len]='\"';
	
	HWND hWnd=FindWindow(NULL, arg); //找到程序运行窗口的句柄
	HDC hDC=GetDC(hWnd);//通过窗口句柄得到该窗口的设备场境句柄
	HPEN hPen, hOldPen; //画笔
	int j=0;
	
	for(; j<500; ++j)
		SetPixel(hDC, 10+j, 10+j, 0x0000ff);//用画点的办法画一根线,最后一个参数是颜色（32位）
	hPen=CreatePen(PS_SOLID, 2, 0x00ff00);//生成绿色画笔
	hOldPen=(HPEN)SelectObject(hDC, hPen);//把画笔引入设备场境
	
	MoveToEx(hDC, 20, 50, NULL); //设置画线起点
	LineTo(hDC, 520, 550);      //画到终点
	
	Arc(hDC, 100, 100, 300, 300, 350, 500, 350, 500);//画圆
	
	SelectObject(hDC, hOldPen);
	
	//下面是对比，表明它确实是控制台程序
	
	printf("hello console");
	system("pause");
*/
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

	
	
}