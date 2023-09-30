#ifndef INCLUDED_GAMELIB_HASHSET_H
#define INCLUDED_GAMELIB_HASHSET_H

#include "GameLib/Base/Stack.h"
#include "GameLib/Base/Hash.h"
#include "GameLib/Base/Array.h"

namespace GameLib{

///���E�e�ʎw��n�b�V���e�[�u��
/*!
�n�b�V���֐��͑S�r�b�g�̘a���e�[�u���T�C�Y�ŏ�]�����P���ȕ����ł���B
�S�r�b�g�ł��邽�߁A�N���X�̃T�C�Y���傫���ƃR�X�g���傫���Ȃ�\���������B
�Ȃ��A����v�f�͑}�����Ȃ��Bstd::mutli_set���K�v�ɂȂ����������Ƃ��Ȃ����炾�B
*/
template< class T, class H = Hash< T > > class HashSet{
public:
	///�f�t�H���g�BsetCapacity���Ă΂Ȃ��Ɠ����Ȃ�
	HashSet();
	///�ő�e�ʂ��w��B�e�[�u���T�C�Y�̓f�t�H���g��capacity�ȏ�̍ŏ��̑f���B
	explicit HashSet( int capacity, int tableSize = 0 );
	~HashSet();
	///�Ċm�ہB���g�������ԂŌĂԂ�assert�Bclear()����B
	void setCapacity( int capacity, int tableSize = 0 );
	///�ő�e�ʎ擾
	int capacity() const;
	///���݂̗v�f���擾
	int size() const;
	///�S���
	void clear();
	///�ǉ��B�ǉ������ꏊ��Ԃ��B�������̂����łɂ���Ζ������Ė����C�e���[�^��Ԃ��B
	int add( const T& );
	///�����B�Ԃ�̂̓C�e���[�^
	int find( const T& );
	///�����B�������A���邩�ǂ��������Ԃ�
	bool isIn( const T& ) const;
	///�C�e���[�^���w�肵�č폜�B�폜�����true��Ԃ��B
	bool remove( int position );
	///�l�擾�B���������͋����Ȃ��̂�const�ł����񋟂��Ȃ��B
	const T* value( int position ) const;
	//�ȉ��P���ȃ��X�g�Ƃ��ăA�N�Z�X����ۂ̃C���^�[�t�F�C�X�B�������Е���
	int next( int position ) const;
	int first() const;
	///position�����[������B
	bool isEnd( int position ) const;
	///�P�Ȃ�z��ɕϊ��B�O������size()�����T�C�Y��new�����̈��n�����ƁB
	void copyTo( T* ) const;
	///Array�ɃR�s�[�B��̂��̂�n���B
	void copyTo( Array< T >* ) const;
private:
	void operator=( const HashSet& ); //����֎~
	HashSet( const HashSet& ); //�R�s�[�R���X�g���N�^�֎~

	int* mNexts;
	T* mValues;
	Stack< int > mEmptyStack;
	int mCapacity;
	int mSize;
	int mTableSize;
};

} //namespace GameLib
#include "GameLib/Base/Impl/HashSetImpl.h" //���g�͂��̒�

#endif

