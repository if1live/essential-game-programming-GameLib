#ifndef INCLUDED_GAMELIB_SHAREDPTRIMPL_H
#define INCLUDED_GAMELIB_SHAREDPTRIMPL_H

namespace GameLib{

template< class T > inline SharedPtr< T >::SharedPtr() : mReference( 0 ){
}

template< class T >	inline SharedPtr< T >::SharedPtr( T* p ){
	if ( p ){
		mReference = NEW Reference( p );
	}
}

template< class T >	inline SharedPtr< T >::SharedPtr( const SharedPtr< T >& p ) : 
mReference( p.mReference ){
	if ( mReference ){
		mReference->increment();
	}
}
template< class T >	inline SharedPtr< T >::~SharedPtr(){
	if ( mReference ){
		if ( mReference->decrement() == 0 ){
			delete mReference->mPointer;
			delete mReference;
		}
		mReference = 0; //������ɂ���|�C���^��0�ɂ���B
	}
}

template< class T >	inline SharedPtr< T >& SharedPtr< T >::operator=( T* p ){
	if ( mReference ){
		if ( mReference->decrement() == 0 ){
			delete mReference->mPointer;
			if ( p ){ //�|�C���^����łȂ����
				mReference->mPointer = p; //���ɒN���g���ĂȂ��̂ł�������������ė��p�B
				mReference->mCount = 1;
			}else{ //�N���g��Ȃ��̂ŏ���
				delete mReference;
				mReference = 0;
			}
		}else if ( p ){ //�|�C���^����łȂ��̂ŐV���ɐ����B�����Areference�͑����猩�Ă���̂ŐG��Ȃ��B
			mReference = NEW Reference( p );
		}else{ //�|�C���^����Breference�͑����猩�Ă���̂ŐG�炸�A�Q�Ɗ֌W��؂�B
			mReference = 0;
		}
	}else if ( p ){
		mReference = NEW Reference( p );
	}
	return *this;
}

template< class T >	inline SharedPtr< T >& SharedPtr< T >::operator=( const SharedPtr< T >& p ){
	if ( mReference ){
		if ( mReference->decrement() == 0 ){ //�Ⴄ����0�ɂȂ�����J��
			delete mReference->mPointer;
			delete mReference;
			mReference = 0;
		}
	}
	//�����ւ�
	mReference = p.mReference;
	if ( mReference ){ //���m������΃C���N�������g
		mReference->increment();
	}
	return *this;
}

template< class T >	inline T& SharedPtr< T >::operator*(){
	T* p = ( mReference ) ? mReference->mPointer : 0;
	ASSERT( p );
	return *p;
}

template< class T >	inline const T& SharedPtr< T >::operator*() const { 
	T* p = ( mReference ) ? mReference->mPointer : 0;
	ASSERT( p );
	return *p;
}

template< class T >	inline T* SharedPtr< T >::operator->(){ 
	return ( mReference ) ? mReference->mPointer : 0;
}

template< class T >	inline const T* SharedPtr< T >::operator->() const { 
	return ( mReference ) ? mReference->mPointer : 0;
}

//�|�C���^�����̂܂ܕԂ��ƈ��p����鋰�ꂪ����̂ŁA��[���Ȃ�0xffffffff��Ԃ��B
template< class T >	inline SharedPtr< T >::operator void*() const {
	T* p = ( mReference ) ? mReference->mPointer : 0;
	return ( p ) ? reinterpret_cast< void* >( ~0 ) : 0;
}

//Reference
template< class T > inline SharedPtr< T >::Reference::Reference( T* p ) :
mPointer( p ),
mCount( 1 ){
}

template< class T > inline SharedPtr< T >::Reference::~Reference(){
	ASSERT( mCount == 0 );
	mPointer = 0;
}

template< class T > inline void SharedPtr< T >::Reference::increment(){
	++mCount;
}

template< class T > inline int SharedPtr< T >::Reference::decrement(){
	--mCount;
	return mCount;
}


//�ȉ�const��
template< class T > inline ConstSharedPtr< T >::ConstSharedPtr(){
}

template< class T >	inline ConstSharedPtr< T >::ConstSharedPtr( const SharedPtr< T >& p ) : 
mPointer( p ){
}

template< class T >	inline ConstSharedPtr< T >::~ConstSharedPtr(){}

template< class T >	inline ConstSharedPtr< T >& ConstSharedPtr< T >::operator=( const SharedPtr< T >& p ){
	mPointer = p;
	return *this;
}

template< class T >	inline const T& ConstSharedPtr< T >::operator*() const { 
	return mPointer.operator*();
}

template< class T >	inline const T* ConstSharedPtr< T >::operator->() const { 
	return mPointer.operator->();
}

template< class T >	inline ConstSharedPtr< T >::operator void*() const {
	return mPointer.operator void*();
}

} //namespace GameLib

#endif