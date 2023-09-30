#ifndef INCLUDED_GAMELIB_SHAREDPTR_H
#define INCLUDED_GAMELIB_SHAREDPTR_H

namespace GameLib{

/*�Q�ƃJ�E���^�����L�|�C���^

�X���b�h�Z�[�t�ł͂Ȃ����Ƃɒ��ӁB
ConstSharedPtr�͌��̌^��const�֐������Ă΂������Ȃ����̂��߂�
������̂ŁAConstSharedPtr�Ƃ���Ƃ����SharedPtr�ł������B

ConstSharedPtr< int > foo(){
   return SharedPtr< int >( 0 );
}
��A
ConstSharedPtr< int > a = SharedPtr< int >();
���I�[�P�[���Ƃ������Ƃ��B
*/
template< class T > class SharedPtr{
public:
	SharedPtr();
	SharedPtr( T* p );
	SharedPtr( const SharedPtr< T >& p );
	~SharedPtr();
	SharedPtr& operator=( T* p );
	SharedPtr& operator=( const SharedPtr< T >& p );
	T& operator*();
	const T& operator*() const;
	T* operator->();
	const T* operator->() const;
	operator void*() const;
private:
	struct Reference{
		Reference( T* p );
		~Reference();
		void increment();
		int decrement();

		T* mPointer;
		unsigned mCount;
	};
	Reference* mReference;
};

//const��
template< class T > class ConstSharedPtr{
public:
	ConstSharedPtr();
	ConstSharedPtr( const SharedPtr< T >& );
	~ConstSharedPtr();
	ConstSharedPtr& operator=( const SharedPtr< T >& );
	const T& operator*() const;
	const T* operator->() const;
	operator void*() const;
private:
	SharedPtr< T > mPointer;
};

} //namespace GameLib
#include "GameLib/Base/Impl/SharedPtrImpl.h"

#endif