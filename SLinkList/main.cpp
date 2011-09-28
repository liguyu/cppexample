/*
* main.cpp
* Copyright (C) lmy User 2008 <lmy@>
* 类模板的实现
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
	cout << "双链表的长度: " << sLinkList.length << endl;;
    return 0;
}