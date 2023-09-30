#ifndef INCLUDED_GAMELIB_TANK_H
#define INCLUDED_GAMELIB_TANK_H

#include "GameLib/Base/Array.h"

namespace GameLib{

///�����I���܂ŃA�N�Z�X�����A�������������킩��Ȃ��A�Ƃ����p�r�̂��߂̂���
/*!
�������߂Ĉ�C�ɗ����C���[�W�Ń^���N�Ƃ������A�����Ƃ������O������Ε�W�B
�����I�ɂ͗e�ʒP��(blockSize)�Â����ŃA���P�[�g���Čp�������Ă����B

�A�N�Z�X��getCurrent()�Ŏ擾�AtoNext()�Ŏ��ցBrewind()�ōŏ�����A�N�Z�X���Ȃ�����B
*/
template< class T > class Tank{
public:
	explicit Tank( int blockSize = 16 );
	~Tank();
	///�Ċm�ہB���g�������ԂŌĂԂ�assert�Bclear()����B
	void setBlockSize( int blockSize );
	///���݂̗v�f���擾
	int size() const;
	///�S���
	void clear();
	///�����ɋ�Œǉ��B���������̂ւ̃|�C���^��Ԃ��̂ŁA�����ݒ�͂�����g�����ƁB
	T* add();
	///�����ɒǉ�
	void add( const T& );
	///����
	void toNext();
	///�I���ł�
	bool isEnd() const;
	///���݂̗v�f���擾(��const)
	T* get();
	///���݂̗v�f���擾(const)
	const T* get() const;
	///�ŏ��̗v�f�Ƀ|�C���^��߂�
	void rewind();
	///�P�Ȃ�z��ɕϊ��B�O������size()�����T�C�Y��new�����̈��n�����ƁB
	void copyTo( T* ) const;
	///Array�ɃR�s�[�B��łȂ��Ɠ{����B
	void copyTo( Array< T >* ) const;
private:
	struct Block{
		Block* mNext; //���̃m�[�h
		T* mElements; //�z��
	};
	void operator=( const Tank& ); //����֎~
	Tank( const Tank& ); //�R�s�[�R���X�g���N�^�֎~

	//�萔
	int mBlockSize; //�P�ʗ�
	int mSize;
	Block mHead; //�_�~�[

	Block* mLastBlock; //�ŏI�m�[�h
	int mLastPos; //�ŏI�m�[�h���̈ʒu
	Block* mCurrentBlock; //���݂̃C�e���[�^
	int mCurrentPos; //�m�[�h���ʒu
};

} //namespace GameLib
#include "GameLib/Base/Impl/TankImpl.h" //���g�͂��̒�

#endif
