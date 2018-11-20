#pragma once
//�ڵ��࣬������ÿ���ڵ�Ĵ洢���ͺ�ָ������
template<class T>
class ChainNode{
public:
	T data;
	ChainNode<T> *link;
};

//�����࣬��װ�������������Ӧ����
template<class T>
class CMyChain
{
public:
	CMyChain(){ first = 0; };
	~CMyChain();
	bool IsEmpty()const{ return first == 0; }	//�ж��Ƿ�Ϊ��
	int Length()const;						//���ظ�����ĳ���
	bool Find(int k, T&x)const;				//���ص�k��Ԫ�ص�x��
	int Search(const T&x)const;				//����x���ڵ�λ��  
	CMyChain<T>& Delete(int k, T& x);			//ɾ����k��Ԫ�ز��������ص�x��
	CMyChain<T>& Insert(int k, const T&x);		//�ڵ�k��Ԫ��֮�����x
	ChainNode<T> *first;					//ָ���һ���ڵ��ָ��
	ChainNode<T> *last;						//ָ�����һ���ڵ��ָ��

	void Erase();
	CMyChain<T>& Append(const T&x);

};

//�����������ʵ�ֶ�����ı���
template<class T>
class ChainIterator{
public:
	T* Initialize(const CMyChain<T>&c){
		location = c.first;
		if (location){
			return &location->data;//Ϊ���е�ַ����
		}
		return 0;
	}

	T* Next(){
		if (!location)
			return 0;
		location = location->link;
		if (location)
			return &location->data;
		return 0;
	}
private:
	ChainNode<T>*location;
};
