#ifndef INCLUDED_GAMELIB_HASTSETIMPL_H
#define INCLUDED_GAMELIB_HASHSETIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/Base/HashSet.h"
#include "GameLib/Base/PrimeNumber.h"

/*
<����>
�Y������mCapacity + i (0<=i<mTableSize)���g���Ă�����A����̓_�~�[�m�[�h�ł���B
mNodes[ mCapacity + i ].mNext��mNodes[ mCapacity + i + 1 ]�������Ă���B
*/

namespace GameLib{

//�擪�Ɩ��������ʈ�������Ȃ��悤�ɂ�����ƍH�v�����Ă���B
template< class T, class H > inline HashSet< T, H >::HashSet() : 
mNexts( 0 ),
mValues( 0 ),
mCapacity( 0 ),
mSize( 0 ),
mTableSize( 0 ){
	ASSERT( sizeof( T ) != 0 ); //���肦�Ȃ���ˁH
}

//�擪�Ɩ��������ʈ�������Ȃ��悤�ɂ�����ƍH�v�����Ă���B
template< class T, class H > inline HashSet< T ,H >::HashSet( int capacity, int tableSize ) : 
mNexts( 0 ),
mValues( 0 ),
mCapacity( 0 ),
mSize( 0 ),
mTableSize( 0 ){
	ASSERT( sizeof( T ) != 0 ); //���肦�Ȃ���ˁH
	setCapacity( capacity, tableSize );
}

template< class T, class H > inline HashSet< T, H >::~HashSet(){
	if ( mNexts ){
		int pos = mNexts[ mCapacity ];
		while ( pos < mCapacity + mTableSize ){ //�m�[�h�������
			if ( pos < mCapacity ){
				mValues[ pos ].~T();
			}
			pos = mNexts[ pos ];
		}
		OPERATOR_DELETE( mValues ); //��delete
		SAFE_DELETE_ARRAY( mNexts );
	}
}

template< class T, class H > inline void HashSet< T, H >::setCapacity( int capacity, int tableSize ){
	ASSERT( mSize == 0 && "NOT EMPTY! call clear()." );
	//�܂��N���A
	if ( mCapacity > 0 ){
		mEmptyStack.clear();
		clear();
		OPERATOR_DELETE( mValues ); //��delete
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
	}else if ( tableSize > 0x10000 ){ //65536�ȏ�͋����Ȃ�
		tableSize = 65521; //65536�ȉ��ő�̑f��
	}
	mTableSize = tableSize;
	//���z�� �e�[�u���T�C�Y�������_�~�[�𑫂����Ƃɒ���
	mNexts = NEW int[ mCapacity + mTableSize ];
	//�l�z��m�ہB�R���X�g���N�^���s�v�Ȃ̂Ő�new
	mValues = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
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

template< class T, class H > inline int HashSet< T, H >::capacity() const {
	return mCapacity;
}

template< class T, class H > inline int HashSet< T, H >::add( const T& v ){
	ASSERT( mSize < mCapacity );
	//�n�b�V���֐��Z�o
	int h = H().value( v, mTableSize );
	//����
	int p = mNexts[ mCapacity + h ];
	while ( p < mCapacity ){
		if ( H().isEqual( mValues[ p ], v ) ){
			break;
		}
		p = mNexts[ p ];
	}
	if ( p >= mCapacity ){ //�Ȃ����瑫��
		//�󂫏ꏊ���擾
		int newPos;
		mEmptyStack.pop( &newPos );
		//�e�[�u���擪�ɑ}��
		int head = mCapacity + h;
		mNexts[ newPos ] = mNexts[ head ]; //���_�~�[�̎����Z�b�g
		new ( &mValues[ newPos ] ) T( v ); //�R�s�[�R���X�g���N�^�Ăяo��
		mNexts[ head ] = newPos; //�O�̃����N��V�m�[�h�ɂȂ�
		++mSize;
		return newPos;
	}else{
		return mCapacity + mTableSize;
	}
}

template< class T, class H > inline int HashSet< T, H >::find( const T& v ){
	int h = H().value( v, mTableSize );

	int p = mNexts[ mCapacity + h ];
	while ( p < mCapacity ){
		if ( H().isEqual( mValues[ p ], v ) ){
			return p;
		}
		p = mNexts[ p ];
	}
	return mCapacity + mTableSize;
}

template< class T, class H > inline bool HashSet< T, H >::isIn( const T& v ) const {
	int h = H().value( v, mTableSize );

	int p = mNexts[ mCapacity + h ];
	while ( p < mCapacity ){
		if ( H().isEqual( mValues[ p ], v ) ){
			return true;
		}
		p = mNexts[ p ];
	}
	return false;
}

template< class T, class H > inline bool HashSet< T, H >::remove( int position ){
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
			mValues[ p ].~T(); //�f�X�g���N�g
			mEmptyStack.push( p ); //�󂫃X�^�b�N�Ƀv�b�V��
			--mSize;
			return true;
		}
		prev = p;
		p = mNexts[ p ];
	}
	return false;
}

template< class T, class H > inline const T* HashSet< T, H >::value( int p ) const {
	return &mValues[ p ];
}

template< class T, class H > inline void HashSet< T, H >::clear(){
	//�f�X�g���N�g���ĉ��
	int prev = mCapacity; //�w�b�h
	int p = mNexts[ mCapacity ]; //�_�~�[�w�b�h
	while ( p < mCapacity + mTableSize ){  //mCapacity + mTableSize�͑��݂��Ȃ��B�����܂ł̓_�~�[�Ƃ��đ��݂���B
		if ( p < mCapacity ){
			mNexts[ prev ] = mNexts[ p ]; //�Ȃ��ς���
			mValues[ p ].~T();
			mEmptyStack.push( p ); //�󂫃X�^�b�N�Ƀv�b�V��
		}
		prev = p;
		p = mNexts[ p ];
	}
	mSize = 0;
}

template< class T, class H > inline int HashSet< T, H >::next( int p ) const {
	ASSERT( p >= 0 );
	while ( p < mCapacity + mTableSize ){
		p = mNexts[ p ];
		if ( p < mCapacity ){ //�_�~�[�m�[�h�ȊO��Ԃ�
			return p;
		}
	}
	return p;
}

template< class T, class H > inline int HashSet< T, H >::first() const {
	int p = mCapacity; //�ŏ��̃_�~�[
	while ( p < mCapacity + mTableSize ){
		p = mNexts[ p ];
		if ( p < mCapacity ){ //�_�~�[�m�[�h�ȊO��Ԃ�
			return p;
		}
	}
	return mCapacity + mTableSize;
}

template< class T, class H > inline bool HashSet< T, H >::isEnd( int position ) const {
	return ( position == mCapacity + mTableSize );
}

template< class T, class H > inline void HashSet< T, H >::copyTo( T* a ) const {
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

template< class T, class H > inline void HashSet< T, H >::copyTo( Array< T >* a ) const {
	a->setSize( mSize );
	if ( mSize > 0 ){
		copyTo( &( ( *a )[ 0 ] ) );
	}
}


template< class T, class H > inline int HashSet< T, H >::size() const {
	return mSize;
}

} //namespace GameLib

#endif
