#ifndef INCLUDED_GAMELIB_VECTORIMPL_H
#define INCLUDED_GAMELIB_VECTORIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/Base/Vector.h"

namespace GameLib{

template< class T > inline Vector< T >::Vector( int size, int capacity ) : 
mElements( 0 ),
mSize( size ),
mCapacity( capacity ){
	if ( capacity <= 0 ){
		mCapacity = mSize;
	}
	if ( mSize > 0 ){
		mElements = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
		for ( int i = 0; i < mSize; ++i ){
			new ( &mElements[ i ] ) T;
		}
	}
}

template< class T > inline Vector< T >::Vector( const Vector& a ) : 
mElements( 0 ),
mSize( a.mSize ),
mCapacity( a.mCapacity ){
	if ( mSize > 0 ){
		mElements = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
		for ( int i = 0; i < mSize; ++i ){
			new ( &mElements[ i ] ) T( a.mElements[ i ] ); //�R�s�[�R���X�g���N�^�Ăяo��
		}
	}
}

template< class T > inline Vector< T >::~Vector(){
	clear();
}

template< class T > inline void Vector< T >::operator=( const Vector& a ){ 
	//operator=()���������ɂ͎�����������P�[�X�ɋC�����悤�B
	if ( this == &a ){
		return;
	}
	//�܂��ۂ��ƍ폜�B�\���L���̈悪�����Ă��A���ʂɂȂ�\��������̂ō폜����B
	clear();

	mSize = a.size;
	mCapacity = a.mCapacity;
	if ( mSize > 0 ){
		mElements = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
		for ( int i = 0; i < mSize; ++i ){
			new ( &mElements[ i ] ) T( a.mElements[ i ] ); //�R�s�[�R���X�g���N�^�Ăяo��
		}
	}
}


template< class T > inline void Vector< T >::push( const T& a ){
	if ( mSize == mCapacity ){ //�ꏊ���Ȃ��B���z�����s���B
		//1.5�{+1�T�C�Y�̗̈���m�ہB+1��2�����ł�������悤�ɂ��邽�߁B
		mCapacity = ( mCapacity * 3 / 2 ) + 1;
		T* newElements = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
		//�S�R�s�[����ьÂ��v�f�̔j��
		for ( int i = 0; i < mSize; ++i ){
			new ( &newElements[ i ] ) T( mElements[ i ] );
			mElements[ i ].~T();
		}
		//���̈�̔j��
		OPERATOR_DELETE( mElements );
		//�����ւ�
		mElements = newElements;
	}
	//�V�v�f�̒ǉ�
	new ( &mElements[ mSize ] ) T( a );
	//�T�C�Y�X�V
	++mSize;
}

template< class T > inline T* Vector< T >::push(){
	if ( mSize == mCapacity ){ //�ꏊ���Ȃ��B���z�����s���B
		//1.5�{+1�T�C�Y�̗̈���m�ہB+1��2�����ł�������悤�ɂ��邽�߁B
		mCapacity = ( mCapacity * 3 / 2 ) + 1;
		T* newElements = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
		//�S�R�s�[����ьÂ��v�f�̔j��
		for ( int i = 0; i < mSize; ++i ){
			new ( &newElements[ i ] ) T( mElements[ i ] );
			mElements[ i ].~T();
		}
		//���̈�̔j��
		OPERATOR_DELETE( mElements );
		//�����ւ�
		mElements = newElements;
		newElements = 0;
	}
	//�V�v�f�̒ǉ�
	new ( &mElements[ mSize ] ) T;
	//�T�C�Y�X�V
	++mSize;
	//�V�v�f�̃|�C���^��Ԃ�
	return &mElements[ mSize - 1 ];
}

template< class T > inline void Vector< T >::clear(){
	//�f�X�g���N�^�Ăяo��
	for ( int i = 0; i < mSize; ++i ){
		mElements[ i ].~T();
	}
	//�̈�J��
	OPERATOR_DELETE( mElements );
	mSize = mCapacity = 0;
}

template< class T > inline int Vector< T >::size() const {
	return mSize;
}

template< class T > inline void Vector< T >::setSize( int n ){
	//�w��T�C�Y������capacity�ȉ��Ȃ���z�����Ȃ�
	if ( n < mCapacity ){
		for ( int i = n; i < mSize; ++i ){ //���������̕��̉���͕K�v�B5������3�ɂ���Ȃ�Y����3,4�͉������K�v������
			mElements[ i ].~T();
		}
	}else if ( n > mCapacity ){ //�_�����B���z��
		mCapacity = n;
		T* newElements = static_cast< T* >( OPERATOR_NEW( sizeof( T ) * mCapacity ) );
		//�S�R�s�[����ьÂ��v�f�̔j��
		for ( int i = 0; i < mSize; ++i ){
			new ( &newElements[ i ] ) T( mElements[ i ] );
			mElements[ i ].~T();
		}
		//���̈�̔j��
		OPERATOR_DELETE( mElements );
		//�����ւ�
		mElements = newElements;
		newElements = 0;
		//�����Ȃ��̈�Ƀf�t�H���g�R���X�g���N�^�Ăяo��
		for ( int i = mSize; i < n; ++i ){
			new ( &mElements[ i ] ) T;
		}
	}
	mSize = n;
}

template< class T > inline T& Vector< T >::operator[]( int i ){
	ASSERT( i >= 0 && i < mSize && "Range Error!" );
	return mElements[ i ];
}

template< class T > inline const T& Vector< T >::operator[]( int i ) const {
	ASSERT( i >= 0 && i < mSize && "Range Error!" );
	return mElements[ i ];
}

template< class T > inline void Vector< T >::moveTo( Vector* a ){
	a->clear();
	a->mElelents = mElements;
	a->mSize = mSize;
	a->mCapacity = mCapacity;
	mElements = 0;
	mSize = mCapacity = 0;
}

} //namespace GameLib

#endif
