#ifndef INCLUDED_GAMELIB_QUEUE_H
#define INCLUDED_GAMELIB_QUEUE_H

namespace GameLib{

///�L���[�B�v�f�������E�𒴂��Ȃ���Ώz����̂ł��܂ł��g����B
template< class T > class Queue{
public:
	///�f�t�H���g�B�T�C�Y�A�ő�e�ʂ�0�B
	Queue();
	///�ő�e�ʂ��w��B
	explicit Queue( int capacity );
	~Queue();
	///�Ċm�ہB���g�������ԂŌĂԂ�assert�Bclear()����B
	void setCapacity( int size );
	///�ő�e�ʎ擾
	int capacity() const;
	///���݂̗v�f���擾
	int size() const;
	///�S���
	void clear();
	///�����ɋ�Œǉ��B���������̂ւ̃|�C���^��Ԃ��̂ŁA�����ݒ�͂�����g�����ƁB
	T* pushBack();
	///�����ɒǉ�
	void pushBack( const T& );
	///�擪���폜
	void popFront();
	///�擪���폜(�̂Ă���̂̃R�s�[���擾)
	void popFront( T* out );
	///�擪���擾(const)
	const T* front() const;
	///�擪���擾(��const)
	T* front();
private:
	void operator=( const Queue& ); //����֎~
	Queue( const Queue& ); //�R�s�[�R���X�g���N�^�֎~

	T* mElements;
	int mSize;
	int mCapacity;
	int mFront;
};

} //namespace GameLib
#include "GameLib/Base/Impl/QueueImpl.h" //���g�͂��̒�

#endif

