#include "List.h"

//�_�~�[�v�f���g�����ƂŃR�[�h���Ƃ��Ă��ȒP�ɁI
static const int DUMMY_HEAD = 0;
static const int DUMMY_TAIL = 1;

//�擪�Ɩ��������ʈ�������Ȃ��悤�ɂ�����ƍH�v�����Ă���B
List::List( int capacity ) : 
mNodes( 0 ),
mAddPosition( 0 ),
mCapacity( capacity + 2 ){
	mNodes = new Node[ capacity + 2 ];
	//0�Ԃ͓��ʁB�ŏ��̗v�f�̑O�ɂ���_�~�[�v�f�B
	Node* e;
	e = &mNodes[ DUMMY_HEAD ];
	e->mNext = 1;
	e->mPrev = -1;
	e->mValue = 0.0;
	//1�Ԃ����ʁB�Ō�̗v�f�̂���Ɍ�ɂ���_�~�[�v�f�B
	e = &mNodes[ DUMMY_TAIL ];
	e->mNext = -1;
	e->mPrev = 0;
	e->mValue = 0.0;

	mAddPosition = 2; //2�Ԗڂ���ǉ��J�n
}

List::~List(){
	delete[] mNodes;
	mNodes = 0;
}

int List::addAfter( int position, double v ){
	//�����ꏊ��addPosition
	Node* e = &mNodes[ mAddPosition ];
	//�w��̓z�����o��
	Node* cur = &mNodes[ position ];
	//���̓z�����o��
	Node* next = &mNodes[ cur->mNext ];
	//�l�i�[
	e->mValue = v;
	e->mPrev = position; //�w��ꏊ���O��
	e->mNext = cur->mNext; //�w��̎��̂�����
	//�w��̓z�͑O�ɂȂ�̂ŁA���͑������z�ɂȂ�
	cur->mNext = mAddPosition;
	//�w��̎��̓z�͌�ɂȂ�̂ŁA�O�͑������z�ɂȂ�
	next->mPrev = mAddPosition;
	++mAddPosition;

	return mAddPosition - 1;
}

int List::addBefore( int position, double v ){
	//�����ꏊ��addPosition
	Node* e = &mNodes[ mAddPosition ];
	//�w��̓z�����o��
	Node* cur = &mNodes[ position ];
	//�O�̓z�����o��
	Node* prev = &mNodes[ cur->mPrev ];
	//�l�i�[
	e->mValue = v;
	e->mPrev = cur->mPrev; //�w��̑O�̓z���O
	e->mNext = position; //�w��̓z����
	//�w��̓z�͌�ɂȂ�̂ŁA�O�͑������z�ɂȂ�
	cur->mPrev = mAddPosition;
	//�w��̑O�̓z�͑O�ɂȂ�̂ŁA��͑������z�ɂȂ�
	prev->mNext = mAddPosition;
	++mAddPosition;

	return mAddPosition - 1;
}

int List::addHead( double v ){
	return addBefore( first(), v ); //�_�~�[�v�f�̂������ł���������B
}

int List::addTail( double v ){
	return addAfter( last(), v ); //�_�~�[�v�f�̂������ł���������
}

void List::remove( int position ){
	//�w��̓z
	Node* cur = &mNodes[ position ];
	//��
	Node* next = &mNodes[ cur->mNext ];
	//�O
	Node* prev = &mNodes[ cur->mPrev ];
	//�O�̓z�̎����A���ɂ���B
	prev->mNext = cur->mNext;
	//���̓z�̑O���A�O�ɂ���B
	next->mPrev = cur->mPrev;

	//���܂��B�Ō���������悤�Ȃ�addPosition�������߂��Ă��B
	//�����͑���������悤�ɂȂ邩���ȁB
	if ( position == mAddPosition - 1 ){
		--mAddPosition;
	}
}

void List::removeHead(){ //����͂قƂ�Ǖʖ��B
	remove( first() );
}

void List::removeTail(){ //����͂قƂ�Ǖʖ��B
	remove( last() );
}

double List::value( int position ) const {
	return mNodes[ position ].mValue;
}

int List::next( int position ) const {
	int r = mNodes[ position ].mNext;
	return ( r >= 2 ) ? r : -1; //�_�~�[���w���Ă��-1�ɒu��
}

int List::previous( int position ) const {
	int r = mNodes[ position ].mPrev;
	return ( r >= 2 ) ? r : -1; //�_�~�[���w���Ă��-1�ɒu��
}

int List::first() const {
	return mNodes[ DUMMY_HEAD ].mNext; //0�Ԗڂ̓_�~�[�擪�Ȃ̂ŁA���̎����{���̐擪
}

int List::last() const {
	return mNodes[ DUMMY_TAIL ].mPrev; //1�Ԗڂ̓_�~�[�����Ȃ̂ŁA���̑O���{���̖���
}
