#ifndef INCLUDED_GAMELIB_ARRAY2D_H
#define INCLUDED_GAMELIB_ARRAY2D_H

namespace GameLib{

///�񎟌��z��
template< class T > class Array2d{
public:
	///�f�t�H���g�B�T�C�Y��0�B
	Array2d();
	///�����T�C�Y�B�S�v�f�̓f�t�H���g�R���X�g���N�^�ŏ����������B
	Array2d( int size0, int size1 );
	~Array2d();
	///�ォ��m�ہB���g�������ASSERT()����B��Uclear()���邱�ƁB
	void setSize( int size0, int size1 );
	///�T�C�Y�擾
	int size( int index ) const;
	///�S���
	void clear();
	///�Y����(const)
	const T& operator()( int i, int j ) const;
	///�Y����(��const)
	T& operator()( int i, int j );
private:
	void operator=( const Array2d& ); //����֎~
	Array2d( const Array2d& ); //�R�s�[�R���X�g���N�^�֎~

	T* mElements;
	int mSize[ 2 ];
};

} //namespace GameLib
#include "GameLib/Base/Impl/Array2dImpl.h" //���g�͂��̒�

#endif

