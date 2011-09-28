 /*
 * SiLinkList.h
 * Copyright (C) lmy User 2008 <lmy@>
 * 模板的定义
 */
#ifndef SLinkList_H
#define SLinkList_H
#include <iostream>

using  std::cout;
using  std::cin;
using  std::endl;
//定义双向链表节点 类模板
template<typename T>
struct SLNode{
        SLNode(T &d):data(d),next(0){}	//结构体的构造函数，对节点进行初始化
        T data;							//定义一个类型为T的变量data
        SLNode *next;					//节点的前驱指针，和后继指针
};

//定义链表结构
template<typename T>
class SLinkList{
        SLNode<T> *head;				//头指针，指向头节点
        SLNode<T> *tail;				//尾指针，指向最后一个节点     
public:
        int length;						//指示有序链表的长度
        SLinkList(T &n);				//声明构造函数
        void add(T &c);					//向表尾添加节点
		void clear();					//清空链表
        //void InsAfter(T &c);
        //void remove(T &c);//删除节点
        //DuLNode<T> *find(T &c);//查找节点
        int GetPos(T &c);				//返回第一个值为c的节点位置。没有则返回-1。
        T GetElem(int pos);				//返回第pos个位置的值，非法则返回-1
        SLNode<T> *GetElemP(int pos);	//返回指向第pos个位置的指针，非法则返回-1
        T DelElem(int pos);				//删除第pos个位置的节点，非法则返回-1
		T DelElemEx(int pos);			//删除第pos个位置以及其之前的节点，非法则返回-1

        void print();					//打印节点
        ~SLinkList();					//析够函数
};

//实现构造函数
template<typename T>
SLinkList<T>::SLinkList(T& n):length(0){
        SLNode<T> *p = new SLNode<T>(n);//头节点，data为0
        if(!p)exit(1);					//存储空间分配失败
        head = tail = p;
        p->next = NULL;					//节点的后继指针       
}
//实现添加节点,不断的向链表尾插入元素
template<typename T>
void SLinkList<T>::add(T& n){
        SLNode<T> *p = new SLNode<T>(n);
        tail->next = p;
        tail = p;
        p->next =NULL;
        length++;
}
//清空
template<typename T>
void SLinkList<T>::clear(){
	tail = head;
	length=0;
	head->next=NULL;
}
//查找节点 返回第一个值为n的节点的指针
template<typename T>
int SLinkList<T>::GetPos(T &n){
    int j=1;
    for(SLNode<T> *p=head;p!=tail;p=p->next,j++)
    {
        if(p->next->data == n) return j;
    }   
    return -1;//没找到
}

// 返回第pos个位置的值
template<typename T>
T SLinkList<T>::GetElem(int pos){
    if( pos<0 || pos>length ) return -1;//pos位置不合法
    SLNode<T> *p = head->next;
    int j=1;
    while( p!=tail && j<pos ){
        p = p->next;
        j++;
    }
    return p->data;
}
// 返回第pos个位置的指针
template<typename T>
SLNode<T> *SLinkList<T>::GetElemP(int pos){
	if(0 == pos)return head;
    if( pos<0 || pos>length ) return 0;//pos位置不合法
    SLNode<T> *p = head->next;
    int j=1;
    while( p!=tail && j<pos ){
        p = p->next;
        j++;
    }
    return p;
}

// 删除第pos个位置的元素
template<typename T>
T SLinkList<T>::DelElem(int pos){
    SLNode<T> *p;
    SLNode<T> *pPrior;
    if( !(p=GetElemP(pos)) ) return -1;				//pos位置不合法,第pos个元素不存在
    if( !(pPrior=GetElemP(pos-1)) ) return -1;		//pos-1位置的指针
    if( p == tail ){//删除最后一个元素
        tail = pPrior;
        tail->next = NULL;
    }else{
        pPrior->next = p->next;
    }
	delete p;
    p = NULL;
    length--;
    return head->data;
}

// 删除第pos个以及它之前的元素
template<typename T>
T SLinkList<T>::DelElemEx(int pos){
	if( pos<0 || pos>length ) return 0;//pos位置不合法
    for (int i=pos; i>=1; i--)
    {
		DelElem(i);
    }
    return head->data;
}
//实现打印节点
template<typename T>
void SLinkList<T>::print(){
	int i=1;
	cout << "Head:" << head->data << "\nLength:" << length << "\n";
    for(SLNode<T> *p = head; p!=tail; p=p->next)
    {
        cout << i++ << ":" << p->next->data << " \n";
    }        
}
//析构函数
template<typename T>
SLinkList<T>::~SLinkList(){
    if(head == tail){
        delete head;//此双链表为只有头指针的空表
    }else{
        for(SLNode<T> *p = head; head != tail ; )
        {            
			p = head;
            head = head->next;
            delete p;
            p = NULL;
        }   
        delete tail;
        tail = NULL;
    }       
}
#endif