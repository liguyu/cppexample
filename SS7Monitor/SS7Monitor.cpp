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
	
	HWND hWnd=FindWindow(NULL, arg); //�ҵ��������д��ڵľ��
	HDC hDC=GetDC(hWnd);//ͨ�����ھ���õ��ô��ڵ��豸�������
	HPEN hPen, hOldPen; //����
	int j=0;
	
	for(; j<500; ++j)
		SetPixel(hDC, 10+j, 10+j, 0x0000ff);//�û���İ취��һ����,���һ����������ɫ��32λ��
	hPen=CreatePen(PS_SOLID, 2, 0x00ff00);//������ɫ����
	hOldPen=(HPEN)SelectObject(hDC, hPen);//�ѻ��������豸����
	
	MoveToEx(hDC, 20, 50, NULL); //���û������
	LineTo(hDC, 520, 550);      //�����յ�
	
	Arc(hDC, 100, 100, 300, 300, 350, 500, 350, 500);//��Բ
	
	SelectObject(hDC, hOldPen);
	
	//�����ǶԱȣ�������ȷʵ�ǿ���̨����
	
	printf("hello console");
	system("pause");
*/
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

	
	
}