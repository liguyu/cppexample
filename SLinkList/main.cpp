/*
* main.cpp
* Copyright (C) lmy User 2008 <lmy@>
* ��ģ���ʵ��
*/
#include "SLinkList.h"

int main()
{
	int c=0;
	int d1=1;
	int d2=2;
	int d3=3;
	int d4=4;
	SLinkList<int> sLinkList(c);
	sLinkList.add(d1);
	sLinkList.add(d2);
	sLinkList.add(d3);
	sLinkList.add(d4);

	sLinkList.print();
	cout << "˫����ĳ���: " << sLinkList.length << endl;;
    return 0;
}