 /*
 * SiLinkList.h
 * Copyright (C) lmy User 2008 <lmy@>
 * ģ��Ķ���
 */
#ifndef SLinkList_H
#define SLinkList_H
#include <iostream>

using  std::cout;
using  std::cin;
using  std::endl;
//����˫������ڵ� ��ģ��
template<typename T>
struct SLNode{
        SLNode(T &d):data(d),next(0){}	//�ṹ��Ĺ��캯�����Խڵ���г�ʼ��
        T data;							//����һ������ΪT�ı���data
        SLNode *next;					//�ڵ��ǰ��ָ�룬�ͺ��ָ��
};

//��������ṹ
template<typename T>
class SLinkList{
        SLNode<T> *head;				//ͷָ�룬ָ��ͷ�ڵ�
        SLNode<T> *tail;				//βָ�룬ָ�����һ���ڵ�     
public:
        int length;						//ָʾ��������ĳ���
        SLinkList(T &n);				//�������캯��
        void add(T &c);					//���β��ӽڵ�
		void clear();					//�������
        //void InsAfter(T &c);
        //void remove(T &c);//ɾ���ڵ�
        //DuLNode<T> *find(T &c);//���ҽڵ�
        int GetPos(T &c);				//���ص�һ��ֵΪc�Ľڵ�λ�á�û���򷵻�-1��
        T GetElem(int pos);				//���ص�pos��λ�õ�ֵ���Ƿ��򷵻�-1
        SLNode<T> *GetElemP(int pos);	//����ָ���pos��λ�õ�ָ�룬�Ƿ��򷵻�-1
        T DelElem(int pos);				//ɾ����pos��λ�õĽڵ㣬�Ƿ��򷵻�-1
		T DelElemEx(int pos);			//ɾ����pos��λ���Լ���֮ǰ�Ľڵ㣬�Ƿ��򷵻�-1

        void print();					//��ӡ�ڵ�
        ~SLinkList();					//��������
};

//ʵ�ֹ��캯��
template<typename T>
SLinkList<T>::SLinkList(T& n):length(0){
        SLNode<T> *p = new SLNode<T>(n);//ͷ�ڵ㣬dataΪ0
        if(!p)exit(1);					//�洢�ռ����ʧ��
        head = tail = p;
        p->next = NULL;					//�ڵ�ĺ��ָ��       
}
//ʵ����ӽڵ�,���ϵ�������β����Ԫ��
template<typename T>
void SLinkList<T>::add(T& n){
        SLNode<T> *p = new SLNode<T>(n);
        tail->next = p;
        tail = p;
        p->next =NULL;
        length++;
}
//���
template<typename T>
void SLinkList<T>::clear(){
	tail = head;
	length=0;
	head->next=NULL;
}
//���ҽڵ� ���ص�һ��ֵΪn�Ľڵ��ָ��
template<typename T>
int SLinkList<T>::GetPos(T &n){
    int j=1;
    for(SLNode<T> *p=head;p!=tail;p=p->next,j++)
    {
        if(p->next->data == n) return j;
    }   
    return -1;//û�ҵ�
}

// ���ص�pos��λ�õ�ֵ
template<typename T>
T SLinkList<T>::GetElem(int pos){
    if( pos<0 || pos>length ) return -1;//posλ�ò��Ϸ�
    SLNode<T> *p = head->next;
    int j=1;
    while( p!=tail && j<pos ){
        p = p->next;
        j++;
    }
    return p->data;
}
// ���ص�pos��λ�õ�ָ��
template<typename T>
SLNode<T> *SLinkList<T>::GetElemP(int pos){
	if(0 == pos)return head;
    if( pos<0 || pos>length ) return 0;//posλ�ò��Ϸ�
    SLNode<T> *p = head->next;
    int j=1;
    while( p!=tail && j<pos ){
        p = p->next;
        j++;
    }
    return p;
}

// ɾ����pos��λ�õ�Ԫ��
template<typename T>
T SLinkList<T>::DelElem(int pos){
    SLNode<T> *p;
    SLNode<T> *pPrior;
    if( !(p=GetElemP(pos)) ) return -1;				//posλ�ò��Ϸ�,��pos��Ԫ�ز�����
    if( !(pPrior=GetElemP(pos-1)) ) return -1;		//pos-1λ�õ�ָ��
    if( p == tail ){//ɾ�����һ��Ԫ��
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

// ɾ����pos���Լ���֮ǰ��Ԫ��
template<typename T>
T SLinkList<T>::DelElemEx(int pos){
	if( pos<0 || pos>length ) return 0;//posλ�ò��Ϸ�
    for (int i=pos; i>=1; i--)
    {
		DelElem(i);
    }
    return head->data;
}
//ʵ�ִ�ӡ�ڵ�
template<typename T>
void SLinkList<T>::print(){
	int i=1;
	cout << "Head:" << head->data << "\nLength:" << length << "\n";
    for(SLNode<T> *p = head; p!=tail; p=p->next)
    {
        cout << i++ << ":" << p->next->data << " \n";
    }        
}
//��������
template<typename T>
SLinkList<T>::~SLinkList(){
    if(head == tail){
        delete head;//��˫����Ϊֻ��ͷָ��Ŀձ�
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