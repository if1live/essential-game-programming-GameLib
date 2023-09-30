#ifndef INCLUDED_GAMELIB_VECTOR_H
#define INCLUDED_GAMELIB_VECTOR_H

namespace GameLib{

///��@�\��std::vector
/*!
std::vector���g�킸��������g�����_�͂Ȃɂ��H
���͂��܂�Ȃ��B�p�x�̒Ⴂ�֐���A�댯�Ȋ֐��������Ă���̂�
�g���₷���A�w�K�ɂ͓K���Ă��邾�낤�B
���Ƃ�STL��size_t���Ԃ��Ă��邱�Ƃɕ����Ă���l�ɂ͂����߂ł���Bint�ł��������I

��{�I�ɂ��̃N���X�͊y�ɏ������߂ɂ���A�T�C�Y�ύX�n�̊֐���
�ĂׂΓ��R�x���BArray�ƈ���ăR�s�[���ł��邪�A���v���ȃR�s�[�͔j�œI�Ȑ��\�̗򉻂�
�������낤�B
*/
template< class T > class Vector{
public:
	///capacity���w�肷��ƁA���̐��܂ł͑����Ă������z���Ȃ��B
	explicit Vector( int size = 0, int capacity = 0 );
	///�ۂ��ƃR�s�[����B�x�����Ƃ͊o�傹��
	Vector( const Vector& ); //�R�s�[�R���X�g���N�^���֎~���Ȃ�
	~Vector();
	//����B�ۂ��ƃR�s�[����B�x�����Ƃ͊o�傹��
	void operator=( const Vector& ); //������֎~���Ȃ�
	///�ǉ�
	void push( const T& );
	///�ꏊ�������ǉ��B�|�C���^��Ԃ��̂ł����ɓ���邱�ƁB
	T* push();
	///�ŏI�v�f���폜
	void pop();
	///�S�폜
	void clear();
	///�T�C�Y�擾
	int size() const;
	///�T�C�Y�ύX�B���������Ă����z�����N����Ȃ��B
	void setSize( int );
	///�Y����(��const)
	T& operator[]( int );
	///�Y����(const)
	const T& operator[]( int ) const;
	///���g��ʂ̃x�N�^�Ɉڂ��B�����͋�ɂȂ�B����ɓ����Ă������̂͏�����B
	void moveTo( Vector* );
private:
	T* mElements;
	int mSize;
	int mCapacity;
};

} //namespace GameLib
#include "GameLib/Base/Impl/VectorImpl.h" //���g�͂��̒�

#endif

