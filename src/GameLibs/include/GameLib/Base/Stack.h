#ifndef INCLUDED_GAMELIB_STACK_H
#define INCLUDED_GAMELIB_STACK_H

namespace GameLib{

///�f���ȌŒ�T�C�Y�X�^�b�N
template< class T > class Stack{
public:
	///�f�t�H���g�B�T�C�Y�A�ő�e�ʂ�0�B
	Stack();
	///�ő�e�ʂ��w��B
	explicit Stack( int capacity );
	~Stack();
	///�Ċm�ہB���g�������ԂŌĂԂ�assert�Bclear()����B
	void setCapacity( int size );
	///�ő�e�ʎ擾
	int capacity() const;
	///���݂̗v�f���擾
	int size() const;
	///�S���
	void clear();
	///�����ɒǉ�
	void push( const T& );
	///�����ɒǉ�(�f�t�H���g�R���X�g���N�^�Ăяo��)
	void push();
	///�������폜
	void pop();
	///�������폜(�̂Ă�v�f�̃R�s�[���󂯎��)
	void pop( T* out );
	///�������擾(const)
	const T* get() const;
	///�������擾(��const)
	T* get();
private:
	void operator=( const Stack& ); //����֎~
	Stack( const Stack& ); //�R�s�[�R���X�g���N�^�֎~

	T* mElements;
	int mSize;
	int mCapacity;
	int mBack;
};

} //namespace GameLib
#include "GameLib/Base/Impl/StackImpl.h" //���g�͂��̒�

#endif

