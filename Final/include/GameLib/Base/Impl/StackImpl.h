#ifndef INCLUDED_GAMELIB_STACKIMPL_H
#define INCLUDED_GAMELIB_STACKIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/Base/Stack.h"

namespace GameLib{

template< class T > inline Stack< T >::Stack() :
mElements( 0 ),
mSize( 0 ),
mCapacity( 0 ),
mBack( 0 ){
}

template< class T > inline Stack< T >::Stack( int capacity ) :
mElements( 0 ),
mSize( 0 ),
mCapacity( 0 ),
mBack( 0 ){
	if ( capacity <= 0 ){
		return;
	}
	mCapacity = capacity;
	//�R���X�g���N�^���Ăт����Ȃ��̂Ő�new
	mElements = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
}

template< class T > inline Stack< T >::~Stack(){
	if ( mElements ){
		clear();
		OPERATOR_DELETE( mElements );
	}
}

template< class T > inline void Stack< T >::setCapacity( int capacity ){
	ASSERT( mSize == 0 && "NOT EMPTY!. you must call clear()." );
	//��U���
	if ( mElements ){
		clear();
		OPERATOR_DELETE( mElements );
	}
	mCapacity = mBack = 0;
	if ( capacity <= 0 ){
		return;
	}
	mCapacity = capacity;
	//�Ċm��
	//�R���X�g���N�^���Ăт����Ȃ��̂Ő�new
	mElements = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
}

template< class T > inline int Stack< T >::capacity() const {
	return mCapacity;
}

template< class T > inline int Stack< T >::size() const {
	return mSize;
}

template< class T > inline void Stack< T >::clear(){
	for ( int i = 0; i < mBack; ++i ){
		mElements[ i ].~T(); //�����I�Ƀf�X�g���N�g
	}
	mBack = mSize = 0;
}

template< class T > inline void Stack< T >::push( const T& v ){
	ASSERT( mSize < mCapacity );
	new ( &mElements[ mBack ] ) T( v ); //�R�s�[�R���X�g���N�^�Ăяo��
	++mBack;
	++mSize;
}

template< class T > inline void Stack< T >::push(){
	ASSERT( mSize < mCapacity );
	new ( &mElements[ mBack ] ) T; //�f�t�H���g�R���X�g���N�^�Ăяo��
	++mBack;
	++mSize;
}

template< class T > inline void Stack< T >::pop(){
	ASSERT( mSize > 0 );
	--mBack;
	mElements[ mBack ].~T(); //�����I�f�X�g���N�g
	--mSize;
}

template< class T > inline void Stack< T >::pop( T* o ){
	ASSERT( mSize > 0 );
	*o = mElements[ mBack - 1 ];
	--mBack;
	mElements[ mBack ].~T(); //�����I�f�X�g���N�g
	--mSize;
}

template< class T > inline const T* Stack< T >::get() const {
	ASSERT( mSize > 0 );
	return &mElements[ mBack - 1 ];
}

template< class T > inline T* Stack< T >::get(){
	ASSERT( mSize > 0 );
	return &mElements[ mBack - 1 ];
}

} //namespace GameLib

#endif
