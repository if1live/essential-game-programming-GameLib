#ifndef INCLUDED_GAMELIB_HASHMAP_H
#define INCLUDED_GAMELIB_HASHMAP_H

#include "GameLib/Base/Stack.h"
#include "GameLib/Base/Hash.h"
#include "GameLib/Base/Array.h"

namespace GameLib{

///���E�e�ʎw��n�b�V���e�[�u��
/*!
�n�b�V���֐��͑S�r�b�g�̘a���e�[�u���T�C�Y�ŏ�]�����P���ȕ����ł���B
�S�r�b�g�ł��邽�߁A�N���X�̃T�C�Y���傫���ƃR�X�g���傫���Ȃ�\���������B
�Ȃ��A����L�[�����v�f�͒ǉ����Ȃ��B
*/
template< class Key, class Value, class H = Hash< Key > > class HashMap{
public:
	///�f�t�H���g�BsetCapacity���Ă΂Ȃ��Ɠ����Ȃ�
	HashMap();
	///�ő�e�ʂ��w��B�e�[�u���T�C�Y�̓f�t�H���g��capacity�ȏ�̍ŏ��̑f���B
	explicit HashMap( int capacity, int tableSize = 0 );
	~HashMap();
	///�Ċm�ہB���g�������ԂŌĂԂ�assert�Bclear()����B
	void setCapacity( int capacity, int tableSize = 0 );
	///�ő�e�ʎ擾
	int capacity() const;
	///���݂̗v�f���擾
	int size() const;
	///�S���
	void clear();
	///�ǉ��B�ǉ������ꏊ��Ԃ��B�������̂����łɂ���Ζ������Ė����C�e���[�^��Ԃ��B
	int add( const Key&, const Value& );
	///�ǉ��B�ǉ������ꏊ��Ԃ��B�������̂����łɂ���Ζ������Ė����C�e���[�^��Ԃ��B�l��getValue���Čォ�珑�����ނ��ƁB
	int add( const Key& );
	///�����B�Ԃ�̂̓C�e���[�^
	int find( const Key& ) const;
	///�C�e���[�^���w�肵�č폜�B�폜�����true��Ԃ��B
	bool remove( int position );
	///�L�[�擾(���������͋֎~�Ȃ̂�const�̂�)
	const Key* key( int position ) const;
	///�l�擾(const)
	const Value* value( int position ) const;
	///�l�擾(��const)
	Value* value( int position );
	///�������Ēl�擾(const)
	const Value* operator[]( const Key& ) const;
	///�������Ēl�擾(��const)
	Value* operator[]( const Key& );
	//�ȉ��P���ȃ��X�g�Ƃ��ăA�N�Z�X����ۂ̃C���^�[�t�F�C�X�B�������Е���
	int next( int position ) const;
	int first() const;
	///position�����[������B
	bool isEnd( int position ) const;
	///�P�Ȃ�z��ɃR�s�[�B�O������size()�����T�C�Y��new�����̈��n�����ƁB
	void copyTo( Value* ) const;
	///Array�ɃR�s�[�B��̂��̂�n���B
	void copyTo( Array< Value >* ) const;
private:
	void operator=( const HashMap& ); //����֎~
	HashMap( const HashMap& ); //�R�s�[�R���X�g���N�^�֎~

	int* mNexts;
	Key* mKeys;
	Value* mValues;
	Stack< int > mEmptyStack;
	int mCapacity;
	int mSize;
	int mTableSize;
};

} //namespace GameLib
#include "GameLib/Base/Impl/HashMapImpl.h" //���g�͂��̒�

#endif

