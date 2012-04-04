#include "SingleLinkList.h"

//实现构造函数
SLinkList::SLinkList(){
        SLNode *p = new SLNode;//头节点，data为0
        if(!p)exit(1);					//存储空间分配失败
		p->data = 0;
		length =0;
        head = tail = p;
        p->next = NULL;					//节点的后继指针       
}
//实现添加节点,不断的向链表尾插入元素
void SLinkList::add(T n){
        SLNode *p = new SLNode;
		if(!p)exit(1);					//存储空间分配失败
		p->data = n;

        tail->next = p;
        tail = p;
        p->next =NULL;
        length++;
}
//清空
void SLinkList::clear(){
	tail = head;
	length=0;
	head->next=NULL;
}
//查找节点 返回第一个值为n的节点的指针
int SLinkList::GetPos(T n){
    int j=1;
    for(SLNode *p=head;p!=tail;p=p->next,j++)
    {
        if(p->next->data == n) return j;
    }   
    return -1;//没找到
}

// 返回第pos个位置的值
T SLinkList::GetElem(int pos){
    if( pos<0 || pos>length ) return -1;//pos位置不合法
    SLNode *p = head->next;
    int j=1;
    while( p!=tail && j<pos ){
        p = p->next;
        j++;
    }
    return p->data;
}
// 返回第pos个位置的指针
SLNode *SLinkList::GetElemP(int pos){
	if(0 == pos)return head;
    if( pos<0 || pos>length ) return 0;//pos位置不合法
    SLNode *p = head->next;
    int j=1;
    while( p!=tail && j<pos ){
        p = p->next;
        j++;
    }
    return p;
}

// 删除第pos个位置的元素
T SLinkList::DelElem(int pos){
    SLNode *p;
    SLNode *pPrior;
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
T SLinkList::DelElemEx(int pos){
	if( pos<0 || pos>length ) return 0;//pos位置不合法
    for (int i=pos; i>=1; i--)
    {
		DelElem(i);
    }
    return head->data;
}
//实现打印节点
void SLinkList::print(){
	int i=1;
	cout << "Head:" << head->data << "\nLength:" << length << "\n";
    for(SLNode *p = head; p!=tail; p=p->next)
    {
        cout << i++ << ":" << p->next->data << " \n";
    }        
}
//析构函数
SLinkList::~SLinkList(){
    if(head == tail){
        delete head;//此双链表为只有头指针的空表
    }else{
        for(SLNode *p = head; head != tail ; )
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