#ifndef INCLUDED_GAMELIB_POOL_H
#define INCLUDED_GAMELIB_POOL_H

namespace GameLib{

///����^�̒u���ꏊ�B�����ς�new���팸���邽�߂����ɗp������B
/*!
�m�ۂ��邽�߂����ɂ���A���̌�̑���͉����ł��Ȃ����Ƃɒ��ӂ��邱�ƁB
�܂��A����������s�\�B�ł���̂͑S�̉�������ł���B
*/
template< class T > class Pool{
public:
	explicit Pool( int defaultBlockSize = 16 );
	~Pool();
	///�W���u���b�N����ύX�B
	void setDefaultBlockSize( int blockSize );
	///�S���
	void clear();
	///n�m��
	T* allocate( int n = 1 );
	///n�R�s�[�R���X�g���N�g
	T* allocate( const T*, int n = 1 );
private:
	struct Block{
		Block* mNext; //���̃u���b�N
		T* mElements; //�z��
		int mUsedNumber; //�g��ꂽ��
		int mSize; //�u���b�N�̃T�C�Y
	};
	void operator=( const Pool& ); //����֎~
	Pool( const Pool& ); //�R�s�[�R���X�g���N�^�֎~

	//�萔
	int mDefaultBlockSize; //�P�ʗ�
	Block mHead; //�_�~�[�m�[�h�B
	Block* mLastBlock; //�ŏI�m�[�h
};

} //namespace GameLib
#include "GameLib/Base/Impl/PoolImpl.h" //���g�͂��̒�

#endif
