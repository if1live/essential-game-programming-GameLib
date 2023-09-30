#ifndef INCLUDED_GAMELIB_ARRAY_H
#define INCLUDED_GAMELIB_ARRAY_H

namespace GameLib{

///�����Y�ꂪ���Ȃ���Ă�ڂ���̂��߂̎�������z��
template< class T > class Array{
public:
	///�f�t�H���g�B�T�C�Y��0�B
	Array();
	///�����T�C�Y�B�S�v�f�̓f�t�H���g�R���X�g���N�^�ŏ����������B
	explicit Array( int size );
	//�����T�C�Y�A�����l��n��
	explicit Array( int size, const T& );
	~Array();
	///�ォ��m�ہB�k���͉\�����A�g��͕s�B��Uclear()���邱�ƁB
	void setSize( int size );
	///�T�C�Y�擾
	int size() const;
	///�S���
	void clear();
	///�Y����(const)
	const T& operator[]( int i ) const;
	///�Y����(��const)
	T& operator[]( int i );
private:
	void operator=( const Array& ); //����֎~
	Array( const Array& ); //�R�s�[�R���X�g���N�^�֎~

	T* mElements;
	int mSize;
};

} //namespace GameLib
#include "GameLib/Base/Impl/ArrayImpl.h" //���g�͂��̒�

#endif

