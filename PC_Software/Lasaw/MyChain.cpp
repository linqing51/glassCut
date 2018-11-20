#include "stdafx.h"
#include "MyChain.h"


//�������������������ɾ�����е������еĽڵ�
template<class T>
CMyChain<T>::~CMyChain(){
	Erase();
}


//����������е�����Ԫ�ز����ͷ��ڴ�
template<class T>
void CMyChain<T>::Erase(){
	ChainNode<T>*next;
	//ָ����һ���ڵ�
	while (first){
		next = first->link;
		delete first;
		first = next;
	}
}

//ȷ������ĳ���
template<class T>
int CMyChain<T>::Length()const{
	ChainNode<T>*current = first;
	int length = 0;
	while (current){
		length++;
		current = current->link;
	}
	return length;
}
//�������в��ҵ�k��Ԫ��
//���ھʹ��浽x��
//�������򷵻�false�����򷵻�true
template<class T>
bool CMyChain<T>::Find(int k, T&x)const{
	if (k < 1 || first==0)
		return false;
	ChainNode<T>*current = first;
	int index = 1;
	while (index < k){
		current = current->link;
		index++;
	}
	if (current){
		x = current->data;
		return true;
	}
	return false;
}
//������������
//����x����������򷵻�x���±�
//���x�������򷵻�0
template<class T>
int CMyChain<T>::Search(const T&x)const{
	ChainNode<T>*current = first;
	int index = 1;
	while (current && current->data != x){
		current = current->link;
		index++;
	}
	if (current){
		return index;
	}
	return 0;
}
//��������ɾ��һ��Ԫ��
//����k��Ԫ��ȡ��x
//Ȼ���������ɾ����k��Ԫ��
//����������������쳣OutOfBounds
template<class T>
CMyChain<T>& CMyChain<T>::Delete(int k, T& x){
	if (k < 1 || !first){
		return NULL;
	}
	ChainNode<T>*p = first;
	if (k == 1){
		first = first->link;
	}
	else{
		ChainNode<T>*q = first;
		for (int index = 1; index < k - 1 && q; index++){
			q = q->link;
			//��ʱqָ��Ҫɾ����ǰһ���ڵ�
		}
		if (!q || !q->link){
			return NULL;
		}
		p = q->link;
		if (p == last)
			last = q;
		q->link = p->link;
		//��������ɾ���ýڵ�
		x = p->data;
		delete p;
		return *this;
	}
}
//�ڵ�k��λ��֮�����Ԫ��
//��������OutOfBounds�쳣
//û���㹻�ڴ���NoMem�쳣
template<class T>
CMyChain<T>& CMyChain<T>::Insert(int k, const T&x){
	if (k < 0){
		return NULL;
	}
	ChainNode<T>*p = first;
	for (int index = 1; index < k && p; index++){
		p = p->link;
	}
	if (k > 0 && !p){
		return NULL;
	}
	ChainNode<T>*y = new ChainNode<T>;
	y->data = x;
	if (k){
		y->link = p->link;
		p->link = y;
	}
	else{
		//��Ϊ��һ��Ԫ�ز���
		y->link = first;
		first = y;
	}
	if (!y->link)
		last = y;
	return *this;
}


//�������Ҷ����һ������
template<class T>
CMyChain<T>& CMyChain<T>::Append(const T&x){
	ChainNode<T>*y;
	y = new ChainNode<T>;
	y->data = x;
	y->link = 0;
	if (first){
		last->link = y;
		last = y;
	}
	else{
		first = last = y;
	}
	return *this;
}