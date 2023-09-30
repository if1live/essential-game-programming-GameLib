#ifndef INCLUDED_GAMELIB_HASHMULTIMAP_H
#define INCLUDED_GAMELIB_HASHMULTIMAP_H

#include "GameLib/Base/Stack.h"
#include "GameLib/Base/Hash.h"
#include "GameLib/Base/Array.h"

namespace GameLib{

///���E�e�ʎw��n�b�V���e�[�u��
/*!
�n�b�V���֐��͑S�r�b�g�̘a���e�[�u���T�C�Y�ŏ�]�����P���ȕ����ł���B
�S�r�b�g�ł��邽�߁A�N���X�̃T�C�Y���傫���ƃR�X�g���傫���Ȃ�\���������B
�Ȃ��A����L�[�����v�f���ǉ�����Bfind�͓����L�[�������̂̐擪��Ԃ��B
��������getNext�ňقȂ�L�[�̂��̂��Ԃ�܂Ń��[�v�ŉ񂷂��ƁB
�������A�K��isEnd()���Ă�Ŕ͈͊O�łȂ����Ƃ��m�F���邱�ƁB
*/
template< class Key, class Value, class H = Hash< Key > > class HashMultiMap{
public:
	///�f�t�H���g�BsetCapacity���Ă΂Ȃ��Ɠ����Ȃ�
	HashMultiMap();
	///�ő�e�ʂ��w��B�e�[�u���T�C�Y�̓f�t�H���g��capacity�ȏ�̍ŏ��̑f���B
	explicit HashMultiMap( int capacity, int tableSize = 0 );
	~HashMultiMap();
	///�Ċm�ہB���g�������ԂŌĂԂ�assert�Bclear()����B
	void setCapacity( int capacity, int tableSize = 0 );
	///�ő�e�ʎ擾
	int capacity() const;
	///���݂̗v�f���擾
	int size() const;
	///�S���
	void clear();
	///�ǉ��B�ǉ������ꏊ��Ԃ��B
	int add( const Key&, const Value& );
	///�ǉ��B�ǉ������ꏊ��Ԃ��B�l��getValue���Čォ�珑�����ނ��ƁB
	int add( const Key& );
	///�����B�Ԃ�̂̓C�e���[�^�B����L�[�̐擪�B
	int find( const Key& ) const;
	///�C�e���[�^���w�肵�č폜�B�폜�����true��Ԃ��B
	bool remove( int position );
	///�L�[�擾(���������͋֎~�Ȃ̂�const�̂�)
	const Key* key( int position ) const;
	///�l�擾(const)
	const Value* value( int position ) const;
	///�l�擾(��const)
	Value* value( int position );
	//�ȉ��P���ȃ��X�g�Ƃ��ăA�N�Z�X����ۂ̃C���^�[�t�F�C�X�B�������Е���
	int next( int position ) const;
	int first() const;
	///position�����[������B
	bool isEnd( int position ) const;
	///�P�Ȃ�z��ɕϊ��B�O������size()�����T�C�Y��new�����̈��n�����ƁB
	void copyTo( Value* ) const;
	///Array�ɃR�s�[�B��̂��̂�n���B
	void copyTo( Array< Value >* ) const;
private:
	void operator=( const HashMultiMap& ); //����֎~
	HashMultiMap( const HashMultiMap& ); //�R�s�[�R���X�g���N�^�֎~

	int* mNexts;
	Key* mKeys;
	Value* mValues;
	Stack< int > mEmptyStack;
	int mCapacity;
	int mSize;
	int mTableSize;
};

} //namespace GameLib
#include "GameLib/Base/Impl/HashMultiMapImpl.h" //���g�͂��̒�

#endif

