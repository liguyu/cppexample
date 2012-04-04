#include "SingleLinkList.h"

//ʵ�ֹ��캯��
SLinkList::SLinkList(){
        SLNode *p = new SLNode;//ͷ�ڵ㣬dataΪ0
        if(!p)exit(1);					//�洢�ռ����ʧ��
		p->data = 0;
		length =0;
        head = tail = p;
        p->next = NULL;					//�ڵ�ĺ��ָ��       
}
//ʵ����ӽڵ�,���ϵ�������β����Ԫ��
void SLinkList::add(T n){
        SLNode *p = new SLNode;
		if(!p)exit(1);					//�洢�ռ����ʧ��
		p->data = n;

        tail->next = p;
        tail = p;
        p->next =NULL;
        length++;
}
//���
void SLinkList::clear(){
	tail = head;
	length=0;
	head->next=NULL;
}
//���ҽڵ� ���ص�һ��ֵΪn�Ľڵ��ָ��
int SLinkList::GetPos(T n){
    int j=1;
    for(SLNode *p=head;p!=tail;p=p->next,j++)
    {
        if(p->next->data == n) return j;
    }   
    return -1;//û�ҵ�
}

// ���ص�pos��λ�õ�ֵ
T SLinkList::GetElem(int pos){
    if( pos<0 || pos>length ) return -1;//posλ�ò��Ϸ�
    SLNode *p = head->next;
    int j=1;
    while( p!=tail && j<pos ){
        p = p->next;
        j++;
    }
    return p->data;
}
// ���ص�pos��λ�õ�ָ��
SLNode *SLinkList::GetElemP(int pos){
	if(0 == pos)return head;
    if( pos<0 || pos>length ) return 0;//posλ�ò��Ϸ�
    SLNode *p = head->next;
    int j=1;
    while( p!=tail && j<pos ){
        p = p->next;
        j++;
    }
    return p;
}

// ɾ����pos��λ�õ�Ԫ��
T SLinkList::DelElem(int pos){
    SLNode *p;
    SLNode *pPrior;
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
T SLinkList::DelElemEx(int pos){
	if( pos<0 || pos>length ) return 0;//posλ�ò��Ϸ�
    for (int i=pos; i>=1; i--)
    {
		DelElem(i);
    }
    return head->data;
}
//ʵ�ִ�ӡ�ڵ�
void SLinkList::print(){
	int i=1;
	cout << "Head:" << head->data << "\nLength:" << length << "\n";
    for(SLNode *p = head; p!=tail; p=p->next)
    {
        cout << i++ << ":" << p->next->data << " \n";
    }        
}
//��������
SLinkList::~SLinkList(){
    if(head == tail){
        delete head;//��˫����Ϊֻ��ͷָ��Ŀձ�
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