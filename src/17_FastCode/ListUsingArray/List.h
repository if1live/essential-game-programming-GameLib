#ifndef INCLUDED_LIST_H
#define INCLUDED_LIST_H

//�킩��₷���悤�ɒl��double�ɂ��Ă���Bint�͑S���ꏊ���w���Y�����B
class List{
public:
	List( int capacity );
	~List();
	//�ǉ� �S�ďꏊ��Ԃ�
	int addBefore( int position, double value ); //�w��ꏊ�̑O�ɒǉ�
	int addAfter( int position, double value ); //�w��ꏊ�̌�ɒǉ�
	int addHead( double value ); //�擪�ǉ�
	int addTail( double value ); //�����ǉ�
	//�폜
	void remove( int position ); //�w��ꏊ���폜
	void removeHead(); //�擪���폜
	void removeTail(); //�������폜
	//�擾�n
	double value( int position ) const;
	int next( int position ) const;
	int previous( int position ) const;
	int first() const;
	int last() const;
private:
	//3�z������̂��ʓ|�Ȃ̂ō\���̂�
	struct Node{
		int mNext;
		int mPrev;
		double mValue;
	};
	Node* mNodes;
	int mAddPosition;
	int mCapacity;
};

#endif

