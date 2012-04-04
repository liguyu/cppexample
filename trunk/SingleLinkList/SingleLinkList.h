 /*
 * SiLinkList.h
 * Copyright (C) lmy User 2008 <lmy@>
 * 单链表
 */
#ifndef SLinkList_H
#define SLinkList_H
#include <iostream>

using  std::cout;
using  std::cin;
using  std::endl;

typedef int T;//单链表数据类型

//定义双向链表节点 类模板
struct SLNode{
        T data;							//定义一个类型为T的变量data
        SLNode *next;					//节点的前驱指针，和后继指针
};

//定义链表结构
class SLinkList{
        SLNode *head;				//头指针，指向头节点
        SLNode *tail;				//尾指针，指向最后一个节点     
public:
        int length;					//指示有序链表的长度
        SLinkList();				//声明构造函数
        void add(T c);				//向表尾添加节点
		void clear();				//清空链表
        //void InsAfter(T &c);
        //void remove(T &c);//删除节点
        //DuLNode<T> *find(T &c);//查找节点
        int GetPos(T c);				//返回第一个值为c的节点位置。没有则返回-1。
        T GetElem(int pos);				//返回第pos个位置的值，非法则返回-1
        SLNode *GetElemP(int pos);	//返回指向第pos个位置的指针，非法则返回-1
        T DelElem(int pos);				//删除第pos个位置的节点，非法则返回-1
		T DelElemEx(int pos);			//删除第pos个位置以及其之前的节点，非法则返回-1

        void print();					//打印节点
        ~SLinkList();					//析够函数
};


#endif