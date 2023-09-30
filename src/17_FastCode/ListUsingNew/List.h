#ifndef INCLUDED_LIST_H
#define INCLUDED_LIST_H

//�킩��₷���悤�ɒl��double�ɂ��Ă���B
class List{
public:
	struct Node{
		Node* mNext;
		Node* mPrev;
		double mValue;
	};
	List();
	~List();
	//�ǉ� �S�ďꏊ��Ԃ�
	Node* addBefore( Node* , double value ); //�w��ꏊ�̑O�ɒǉ�
	Node* addAfter( Node*, double value ); //�w��ꏊ�̌�ɒǉ�
	Node* addHead( double value ); //�擪�ǉ�
	Node* addTail( double value ); //�����ǉ�
	//�폜
	void remove( Node* ); //�w��ꏊ���폜
	void removeHead(); //�擪���폜
	void removeTail(); //�������폜
	//�擾�n
	double value( Node* ) const;
	Node* next( Node* ) const;
	Node* previous( Node* ) const;
	Node* first() const;
	Node* last() const;
private:
	Node mDummyHead;
	Node mDummyTail;
};

#endif

