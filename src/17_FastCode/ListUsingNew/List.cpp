#include "List.h"

//�擪�Ɩ��������ʈ�������Ȃ��悤�ɂ�����ƍH�v�����Ă���B
List::List(){
	//�ŏ���肳��ɑO�ɂ���_�~�[�v�f
	mDummyHead.mNext = &mDummyTail;
	mDummyHead.mPrev = 0;
	mDummyHead.mValue = 0.0;
	//�Ō�̗v�f�̂���Ɍ�ɂ���_�~�[�v�f�B
	mDummyTail.mNext = 0;
	mDummyTail.mPrev = &mDummyHead;
	mDummyTail.mValue = 0.0;
}

List::~List(){
	//�S�������ĉ��Ȃ��Ƃ����Ȃ�
	Node* p = first();
	while ( p ){
		Node* nxt = next( p );
		delete p;
		p = nxt;
	}
}

List::Node* List::addAfter( Node* cur, double v ){
	Node* e = new Node();
	//���̓z�����o��
	Node* nxt = cur->mNext;
	//�l�i�[
	e->mValue = v;
	e->mPrev = cur; //�w��ꏊ���O��
	e->mNext = nxt; //�w��̎��̂�����
	//�w��̓z�͑O�ɂȂ�̂ŁA���͑������z�ɂȂ�
	cur->mNext = e;
	//�w��̎��̓z�͌�ɂȂ�̂ŁA�O�͑������z�ɂȂ�
	nxt->mPrev = e;

	return e;
}

List::Node* List::addBefore( Node* cur, double v ){
	Node* e = new Node();
	//�O�̓z�����o��
	Node* prev = cur->mPrev;
	//�l�i�[
	e->mValue = v;
	e->mPrev = prev; //�w��̑O�̓z���O
	e->mNext = cur; //�w��̓z����
	//�w��̓z�͌�ɂȂ�̂ŁA�O�͑������z�ɂȂ�
	cur->mPrev = e;
	//�w��̑O�̓z�͑O�ɂȂ�̂ŁA��͑������z�ɂȂ�
	prev->mNext = e;

	return e;
}

List::Node* List::addHead( double v ){
	return addBefore( first(), v ); //�_�~�[�v�f�̂������ł���������B
}

List::Node* List::addTail( double v ){
	return addAfter( last(), v ); //�_�~�[�v�f�̂������ł���������
}

void List::remove( Node* cur ){
	//��
	Node* nxt = cur->mNext;
	//�O
	Node* prev = cur->mPrev;
	//�O�̓z�̎����A���ɂ���B
	prev->mNext = nxt;
	//���̓z�̑O���A�O�ɂ���B
	nxt->mPrev = prev;
	//����
	delete cur;
	cur = 0;
}

void List::removeHead(){ //����͂قƂ�Ǖʖ��B
	remove( first() );
}

void List::removeTail(){ //����͂قƂ�Ǖʖ��B
	remove( last() );
}

double List::value( Node* p ) const {
	return p->mValue;
}

List::Node* List::next( Node* p ) const {
	Node* r = p->mNext;
	if ( r == &mDummyHead || r == &mDummyTail ){
		return 0;
	}else{
		return r;
	}
}

List::Node* List::previous( Node* p ) const {
	Node* r = p->mPrev;
	if ( r == &mDummyHead || r == &mDummyTail ){
		return 0;
	}else{
		return r;
	}
}

List::Node* List::first() const {
	return mDummyHead.mNext; //�_�~�[�擪�̎����{���̐擪
}

List::Node* List::last() const {
	return mDummyTail.mPrev; //�_�~�[�����̑O���{���̖���
}
