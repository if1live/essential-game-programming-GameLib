#ifndef INCLUDED_GAMELIB_HASH_MULTIMAPIMPL_H
#define INCLUDED_GAMELIB_HASH_MULTIMAPIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/Base/HashMultiMap.h"
#include "GameLib/Base/PrimeNumber.h"

/*
<����>
�Y������mCapacity + i (0<=i<mTableSize)���g���Ă�����A����̓_�~�[�m�[�h�ł���B
mNodes[ mCapacity + i ].mNext��mNodes[ mCapacity + i + 1 ]�������Ă���B
*/

namespace GameLib{

//�擪�Ɩ��������ʈ�������Ȃ��悤�ɂ�����ƍH�v�����Ă���B
template< class K, class V, class H > inline HashMultiMap< K, V, H >::HashMultiMap() : 
mNexts( 0 ),
mKeys( 0 ),
mValues( 0 ),
mCapacity( 0 ),
mSize( 0 ),
mTableSize( 0 ){
	ASSERT( sizeof( K ) != 0 ); //���肦�Ȃ���ˁH
}

//�擪�Ɩ��������ʈ�������Ȃ��悤�ɂ�����ƍH�v�����Ă���B
template< class K, class V, class H > inline HashMultiMap< K, V, H >::HashMultiMap( int capacity, int tableSize ) : 
mNexts( 0 ),
mKeys( 0 ),
mValues( 0 ),
mCapacity( 0 ),
mSize( 0 ),
mTableSize( 0 ){
	ASSERT( sizeof( K ) != 0 ); //���肦�Ȃ���ˁH
	setCapacity( capacity, tableSize );
}

template< class K, class V, class H > inline HashMultiMap< K, V, H >::~HashMultiMap(){
	if ( mNexts ){
		int pos = mNexts[ mCapacity ];
		while ( pos < mCapacity + mTableSize ){ //�m�[�h�������
			if ( pos < mCapacity ){
				mKeys[ pos ].~K();
				mValues[ pos ].~V();
			}
			pos = mNexts[ pos ];
		}
		OPERATOR_DELETE( mKeys ); //��delete
		OPERATOR_DELETE( mValues ); //��delete
		SAFE_DELETE_ARRAY( mNexts );
	}
}

template< class K, class V, class H > inline void HashMultiMap< K, V, H >::setCapacity( int capacity, int tableSize ){
	ASSERT( mSize == 0 && "NOT EMPTY! call clear()." );
	//�܂��N���A
	if ( mCapacity > 0 ){
		mEmptyStack.clear();
		clear();
		OPERATOR_DELETE( mValues ); //��delete
		OPERATOR_DELETE( mKeys ); //��delete
		SAFE_DELETE_ARRAY( mNexts );
	}
	//�Ċm��
	mCapacity = capacity;
	if ( capacity <= 0 ){ //0�T�C�Y����
		return;
	}	
	if ( tableSize == 0 ){ //������capacity�ȏ�̑f����ݒ�
		tableSize = capacity;
	}
	tableSize = PrimeNumber::next( tableSize );
	if ( tableSize < 3 ){
		tableSize = 3; //�Œ��3
	}
	mTableSize = tableSize;
	//���z�� �e�[�u���T�C�Y�������_�~�[�𑫂����Ƃɒ���
	mNexts = NEW int[ mCapacity + mTableSize ];
	//�l�z��m�ہB�R���X�g���N�^���s�v�Ȃ̂Ő�new
	mKeys = static_cast< K* >( OPERATOR_NEW( sizeof( K ) * mCapacity ) );
	mValues = static_cast< V* >( OPERATOR_NEW( sizeof( V ) * mCapacity ) );
	//�󂫔ԍ��X�^�b�N
	mEmptyStack.setCapacity( mCapacity );
	for ( int i = 0; i < mCapacity; ++i ){
		mEmptyStack.push( i ); //�󂫔ԍ����X�g�ɋl�߂Ă���
	}
	//mCapacity+0����mCapacity+mTableSize-1�Ԃ̓_�~�[�v�f�B�e��̐擪�̑O�Ɉʒu����B
	for ( int i = 0; i < mTableSize; ++i ){
		mNexts[ mCapacity + i ] = mCapacity + i + 1;
	}
}

template< class K, class V, class H > inline int HashMultiMap< K, V, H >::capacity() const {
	return mCapacity;
}

template< class K, class V, class H > inline int HashMultiMap< K, V, H >::add( const K& k, const V& v ){
	ASSERT( mSize < mCapacity );
	//�n�b�V���֐��Z�o
	int h = H().value( k, mTableSize );
	//����
	int p = mNexts[ mCapacity + h ];
	while ( p < mCapacity ){
		if ( H().isEqual( mKeys[ p ], k ) ){
			break;
		}
		p = mNexts[ p ];
	}
	int newPos;
	mEmptyStack.pop( &newPos );
	new ( &mKeys[ newPos ] ) K( k ); //�R�s�[�R���X�g���N�^�Ăяo��
	new ( &mValues[ newPos ] ) V( v ); //�R�s�[�R���X�g���N�^�Ăяo��
	if ( p >= mCapacity ){ //�Ȃ����͐擪�ɑ���
		//�e�[�u���擪�ɑ}��
		int head = mCapacity + h;
		mNexts[ newPos ] = mNexts[ head ]; //���_�~�[�̎����Z�b�g
		mNexts[ head ] = newPos; //�O�̃����N��V�m�[�h�ɂȂ�
	}else{ //���鎞�͒���ɑ���
		mNexts[ newPos ] = mNexts[ p ];
		mNexts[ p ] = newPos; //�O�̃����N��V�m�[�h�ɂȂ�
	}
	++mSize;
	return newPos;
}

template< class K, class V, class H > inline int HashMultiMap< K, V, H >::add( const K& k ){
	ASSERT( mSize < mCapacity );
	//�n�b�V���֐��Z�o
	int h = H().value( k, mTableSize );
	//����
	int p = mNexts[ mCapacity + h ];
	while ( p < mCapacity ){
		if ( H().isEqual( mKeys[ p ], k ) ){
			break;
		}
		p = mNexts[ p ];
	}
	int newPos;
	mEmptyStack.pop( &newPos );
	new ( &mKeys[ newPos ] ) K( k ); //�R�s�[�R���X�g���N�^�Ăяo��
	new ( &mValues[ newPos ] ) V;
	if ( p >= mCapacity ){ //�Ȃ����͐擪�ɑ���
		//�e�[�u���擪�ɑ}��
		int head = mCapacity + h;
		mNexts[ newPos ] = mNexts[ head ]; //���_�~�[�̎����Z�b�g
		mNexts[ head ] = newPos; //�O�̃����N��V�m�[�h�ɂȂ�
	}else{ //���鎞�͒���ɑ���
		mNexts[ newPos ] = mNexts[ p ];
		mNexts[ p ] = newPos; //�O�̃����N��V�m�[�h�ɂȂ�
	}
	++mSize;
	return newPos;
}

template< class K, class V, class H > inline int HashMultiMap< K, V, H >::find( const K& k ) const{
	int h = H().value( k, mTableSize );

	int p = mNexts[ mCapacity + h ];
	while ( p < mCapacity ){
		if ( H().isEqual( mKeys[ p ], k ) ){
			return p;
		}
		p = mNexts[ p ];
	}
	return mCapacity + mTableSize;
}

template< class K, class V, class H > inline bool HashMultiMap< K, V, H >::remove( int position ){
	//p�̏ꏊ�ɂ���v�f�̃n�b�V���l���n�b�V���l���v�Z���Ȃ��Œm�肽���B
	//�����ŁA��U�w�b�h�e�[�u���ɏo��܂ŃC�e���[�^��i�߂Č���B
	int h = position;
	while ( h < mCapacity ){
		h = mNexts[ h ];
	}
	//��U�͈͊O�ɏo���B��������mCapacity�������Ǝ��̃n�b�V���l���킩��B
	h -= mCapacity;
	//-1����΂����̃n�b�V���l��
	h -= 1;
	//�n�b�V���l���킩�����̂ŁA�e�[�u���̐擪����H���Ă�����p��������B
	int prev = mCapacity + h; //�w�b�h
	int p = mNexts[ prev ];
	while ( p < mCapacity ){
		if ( p == position ){ //�����B�����������B
			mNexts[ prev ] = mNexts[ p ]; //�Ȃ��ς���
			mKeys[ p ].~K(); //�f�X�g���N�g
			mValues[ p ].~V(); //�f�X�g���N�g
			mEmptyStack.push( p ); //�󂫃X�^�b�N�Ƀv�b�V��
			--mSize;
			return true;
		}
		prev = p;
		p = mNexts[ p ];
	}
	return false;
}

template< class K, class V, class H > inline const V* HashMultiMap< K, V, H >::value( int p ) const {
	return &mValues[ p ];
}

template< class K, class V, class H > inline V* HashMultiMap< K, V, H >::value( int p ){
	return &mValues[ p ];
}

template< class K, class V, class H > inline const K* HashMultiMap< K, V, H >::key( int p ) const {
	return &mKeys[ p ];
}

template< class K, class V, class H > inline void HashMultiMap< K, V, H >::clear(){
	//�f�X�g���N�g���ĉ��
	int prev = mCapacity; //�w�b�h
	int p = mNexts[ mCapacity ]; //�_�~�[�w�b�h
	while ( p < mCapacity + mTableSize ){  //mCapacity + mTableSize�͑��݂��Ȃ��B�����܂ł̓_�~�[�Ƃ��đ��݂���B
		if ( p < mCapacity ){
			mNexts[ prev ] = mNexts[ p ]; //�Ȃ��ς���
			mKeys[ p ].~K();
			mValues[ p ].~V();
			mEmptyStack.push( p ); //�󂫃X�^�b�N�Ƀv�b�V��
		}
		prev = p;
		p = mNexts[ p ];
	}
	mSize = 0;
}

template< class K, class V, class H > inline int HashMultiMap< K, V, H >::next( int p ) const {
	ASSERT( p >= 0 );
	while ( p < mCapacity + mTableSize ){
		p = mNexts[ p ];
		if ( p < mCapacity ){ //�_�~�[�m�[�h�ȊO��Ԃ�
			return p;
		}
	}
	return p;
}

template< class K, class V, class H > inline int HashMultiMap< K, V, H >::first() const {
	int p = mCapacity; //�ŏ��̃_�~�[
	while ( p < mCapacity + mTableSize ){
		p = mNexts[ p ];
		if ( p < mCapacity ){ //�_�~�[�m�[�h�ȊO��Ԃ�
			return p;
		}
	}
	return mCapacity + mTableSize;
}

template< class K, class V, class H > inline bool HashMultiMap< K, V, H >::isEnd( int position ) const {
	return ( position == mCapacity + mTableSize );
}

template< class K, class V, class H > inline void HashMultiMap< K, V, H >::copyTo( V* a ) const {
	int p = mNexts[ mCapacity ];
	int idx = 0;
	while ( p != mCapacity + mTableSize ){
		if ( p < mCapacity ){
			a[ idx ] = mValues[ p ];
			++idx;
		}
		p = mNexts[ p ];
	}
}


template< class K, class V, class H > inline void HashMultiMap< K, V, H >::copyTo( Array< V >* a ) const {
	a->setSize( mSize );
	if ( mSize > 0 ){
		copyTo( &( ( *a )[ 0 ] ) );
	}
}
		


template< class K, class V, class H > inline int HashMultiMap< K, V, H >::size() const {
	return mSize;
}

} //namespace GameLib

#endif
