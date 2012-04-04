 /*
 * SiLinkList.h
 * Copyright (C) lmy User 2008 <lmy@>
 * ������
 */
#ifndef SLinkList_H
#define SLinkList_H
#include <iostream>

using  std::cout;
using  std::cin;
using  std::endl;

typedef int T;//��������������

//����˫������ڵ� ��ģ��
struct SLNode{
        T data;							//����һ������ΪT�ı���data
        SLNode *next;					//�ڵ��ǰ��ָ�룬�ͺ��ָ��
};

//��������ṹ
class SLinkList{
        SLNode *head;				//ͷָ�룬ָ��ͷ�ڵ�
        SLNode *tail;				//βָ�룬ָ�����һ���ڵ�     
public:
        int length;					//ָʾ��������ĳ���
        SLinkList();				//�������캯��
        void add(T c);				//���β��ӽڵ�
		void clear();				//�������
        //void InsAfter(T &c);
        //void remove(T &c);//ɾ���ڵ�
        //DuLNode<T> *find(T &c);//���ҽڵ�
        int GetPos(T c);				//���ص�һ��ֵΪc�Ľڵ�λ�á�û���򷵻�-1��
        T GetElem(int pos);				//���ص�pos��λ�õ�ֵ���Ƿ��򷵻�-1
        SLNode *GetElemP(int pos);	//����ָ���pos��λ�õ�ָ�룬�Ƿ��򷵻�-1
        T DelElem(int pos);				//ɾ����pos��λ�õĽڵ㣬�Ƿ��򷵻�-1
		T DelElemEx(int pos);			//ɾ����pos��λ���Լ���֮ǰ�Ľڵ㣬�Ƿ��򷵻�-1

        void print();					//��ӡ�ڵ�
        ~SLinkList();					//��������
};


#endif