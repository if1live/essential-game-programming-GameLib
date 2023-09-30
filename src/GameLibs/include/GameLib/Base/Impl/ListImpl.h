#ifndef INCLUDED_GAMELIB_LISTIMPL_H
#define INCLUDED_GAMELIB_LISTIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/Base/List.h"

/*
<����>
�Y������mCapacity���g���Ă�����A����̓_�~�[�m�[�h�ł���B
mNodes[ mCapacity ].mNext�͐擪�A
mNodes[ mCapacity ].mPrev�͖����ɂȂ��Ă���B
*/

namespace GameLib{

//�擪�Ɩ��������ʈ�������Ȃ��悤�ɂ�����ƍH�v�����Ă���B
template< class T > inline List< T >::List() : 
mValues( 0 ),
mNodes( 0 ),
mCapacity( 0 ),
mSize( 0 ){
}

//�擪�Ɩ��������ʈ�������Ȃ��悤�ɂ�����ƍH�v�����Ă���B
template< class T > inline List< T >::List( int capacity ) : 
mValues( 0 ),
mNodes( 0 ),
mCapacity( 0 ),
mSize( 0 ){
	setCapacity( capacity );
}

template< class T > inline List< T >::~List(){
	if ( mNodes ){
		//�P�̃f�X�g���N�g
		int pos = mNodes[ mCapacity ].mNext;
		while ( pos < mCapacity ){ //�_�~�[�łȂ����
			mValues[ pos ].~T(); //�f�X�g���N�g
			pos = mNodes[ pos ].mNext;
		}
		OPERATOR_DELETE( mValues ); //��delete
		SAFE_DELETE_ARRAY( mNodes );
	}
}

template< class T > inline void List< T >::setCapacity( int capacity ){
	ASSERT( mSize == 0 && "NOT EMPTY! call clear()." );
	//�܂��N���A
	if ( mCapacity > 0 ){
		mEmptyStack.clear();
		clear();
		OPERATOR_DELETE( mValues ); //��delete
		SAFE_DELETE_ARRAY( mNodes );
	}
	//�Ċm��
	mCapacity = capacity;
	if ( capacity <= 0 ){ //0�T�C�Y����
		return;
	}
	mNodes = NEW Node[ mCapacity + 1 ];
	//�m�[�h�z��m�ہB�R���X�g���N�^���s�v�Ȃ̂�malloc��
	mValues = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
	//�󂫔ԍ��X�^�b�N
	mEmptyStack.setCapacity( mCapacity );
	for ( int i = 0; i < mCapacity; ++i ){
		mEmptyStack.push( i ); //�󂫔ԍ����X�g�ɋl�߂Ă���
	}
	//mCapacity�Ԃ͓��ʁB�ŏ��̗v�f�̑O���Ō�̗v�f�̌�ɂ���_�~�[�v�f
	mNodes[ mCapacity ].mNext = mNodes[ mCapacity ].mPrev = mCapacity;
}

template< class T > inline int List< T >::capacity() const {
	return mCapacity;
}

template< class T > inline int List< T >::addAfter( int position, const T& v ){
	ASSERT( position >= 0 && position <= mCapacity ); //�C�R�[���͂��肤��
	//�󂫏ꏊ���擾
	int newPos;
	mEmptyStack.pop( &newPos );
	//�����ꏊ��addPosition
	Node* e = &mNodes[ newPos ];
	//�w��̓z�����o��
	Node* cur = &mNodes[ position ];
	//���̓z�����o��
	Node* next = &mNodes[ cur->mNext ];
	//�l�i�[(�R�s�[�R���X�g���N�^�Ăяo��)
	new ( &mValues[ newPos ] ) T( v );
	e->mPrev = position; //�w��ꏊ���O��
	e->mNext = cur->mNext; //�w��̎��̂�����
	//�w��̓z�͑O�ɂȂ�̂ŁA���͑������z�ɂȂ�
	cur->mNext = newPos;
	//�w��̎��̓z�͌�ɂȂ�̂ŁA�O�͑������z�ɂȂ�
	next->mPrev = newPos;
	++mSize;

	return newPos;
}

template< class T > inline int List< T >::addBefore( int position, const T& v ){
	ASSERT( position >= 0 && position <= mCapacity ); //�C�R�[���͂��肤��
	//�󂫏ꏊ���擾
	int newPos;
	mEmptyStack.pop( &newPos );
	//�����ꏊ��addPosition
	Node* e = &mNodes[ newPos ];
	//�w��̓z�����o��
	Node* cur = &mNodes[ position ];
	//�O�̓z�����o��
	Node* prev = &mNodes[ cur->mPrev ];
	//�l�i�[(�R���X�g���N�^�Ăяo��)
	new ( &mValues[ newPos ] ) T( v );
	e->mPrev = cur->mPrev; //�w��̑O�̓z���O
	e->mNext = position; //�w��̓z����
	//�w��̓z�͌�ɂȂ�̂ŁA�O�͑������z�ɂȂ�
	cur->mPrev = newPos;
	//�w��̑O�̓z�͑O�ɂȂ�̂ŁA��͑������z�ɂȂ�
	prev->mNext = newPos;
	++mSize;

	return newPos;
}

template< class T > inline int List< T >::addHead( const T& v ){
	return addAfter( mCapacity, v ); //�_�~�[�v�f�̂������ł���������B
}

template< class T > inline int List< T >::addTail( const T& v ){
	return addBefore( mCapacity, v ); //�_�~�[�v�f�̂������ł���������
}

template< class T > inline void List< T >::remove( int position ){
	ASSERT( mSize > 0 );
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
	//�f�X�g���N�^�𖾎��I�ɌĂ�
	mValues[ position ].~T();
	//�T�C�Y����
	--mSize;
	//�󂫃X�^�b�N�Ƀv�b�V��
	mEmptyStack.push( position );
}

template< class T > inline void List< T >::removeHead(){ //����͂قƂ�Ǖʖ��B
	int pos = mNodes[ mCapacity ].mNext;
	if ( pos < mCapacity ){ //�_�~�[�͏����Ȃ������
		remove( pos  );
	}
}

template< class T > inline void List< T >::removeTail(){ //����͂قƂ�Ǖʖ��B
	int pos = mNodes[ mCapacity ].mPrev;
	if ( pos < mCapacity ){ //�_�~�[�͏����Ȃ������
		remove( pos  );
	}
}

template< class T> inline void List< T >::clear(){
	int pos = mNodes[ mCapacity ].mNext;
	while ( pos < mCapacity ){ //�_�~�[�łȂ����
		mValues[ pos ].~T(); //�f�X�g���N�g
		pos = mNodes[ pos ].mNext;
	}
	mNodes[ mCapacity ].mNext = mCapacity;
	mNodes[ mCapacity ].mPrev = mCapacity;
	mSize = 0;
	//��X�^�b�N�l�߂Ȃ���
	mEmptyStack.clear();
	for ( int i = 0; i < mCapacity; ++i ){
		mEmptyStack.push( i ); //�󂫔ԍ����X�g�ɋl�߂Ă���
	}
}

template< class T > inline const T* List< T >::value( int position ) const {
	ASSERT( position >= 0 && position < mCapacity );
	return &mValues[ position ];
}

template< class T > inline T* List< T >::value( int position ){
	ASSERT( position >= 0 && position < mCapacity );
	return &mValues[ position ];
}

template< class T > inline int List< T >::next( int position ) const {
	ASSERT( position >= 0 && position <= mCapacity ); //�_�~�[��������̂�<=
	return mNodes[ position ].mNext;
}

template< class T > inline int List< T >::previous( int position ) const {
	ASSERT( position >= 0 && position <= mCapacity );//�_�~�[��������̂�<=
	return mNodes[ position ].mPrev;
}

template< class T > inline int List< T >::first() const {
	return next( mCapacity );
}

template< class T > inline int List< T >::last() const {
	return getPrev( mCapacity );
}

template< class T > inline bool List< T >::isEnd( int position ) const {
	return ( position == mCapacity );
}

template< class T > inline void List< T >::copyTo( T* a ) const {
	int p = mNodes[ mCapacity ].mNext;
	int idx = 0;
	while ( p != mCapacity ){
		a[ idx ] = mValues[ p ];;
		++idx;
		p = mNodes[ p ].mNext;
	}
}

template< class T > inline void List< T >::copyTo( Array< T >* a ) const {
	a->setSize( mSize );
	if ( mSize > 0 ){
		copyTo( &( ( *a )[ 0 ] ) );
	}
}
		




} //namespace GameLib

#endif
