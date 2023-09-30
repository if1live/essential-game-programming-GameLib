#ifndef INCLUDED_GAMELIB_LIST_H
#define INCLUDED_GAMELIB_LIST_H

#include "GameLib/Base/Stack.h"
#include "GameLib/Base/Array.h"

namespace GameLib{

///new�̃I�[�o�[�փb�h�ɑς����Ȃ�������������̂��߂̑o�������X�g
template< class T > class List{
public:
	List();
	explicit List( int capacity );
	~List();
	///�ォ��T�C�Y�w��B���g������Ǝ��ʁBclear()���悤�B
	void setCapacity( int capacity );
	int capacity() const;
	//�ǉ� �S�ďꏊ��Ԃ�
	int addBefore( int position, const T& value ); //�w��ꏊ�̑O�ɒǉ�
	int addAfter( int position, const T& value ); //�w��ꏊ�̌�ɒǉ�
	int addHead( const T& value ); //�擪�ǉ�
	int addTail( const T& value ); //�����ǉ�
	//�폜
	void remove( int position ); //�w��ꏊ���폜
	void removeHead(); //�擪���폜
	void removeTail(); //�������폜
	void clear(); //�S���폜
	///�l�擾(const)
	const T* value( int position ) const;
	///�l�擾(��const)
	T* value( int position );
	///���[���ǂ�����isEnd()�Ŋm�F����
	int next( int position ) const;
	///���[���ǂ�����isEnd()�Ŋm�F����
	int previous( int position ) const;
	int first() const;
	int last() const;
	///position�����[������B
	bool isEnd( int position ) const;
	///�P�Ȃ�z��ɕϊ��B�O������size()�����T�C�Y��new�����̈��n�����ƁB
	void copyTo( T* ) const;
	///Array�ɃR�s�[�B��̂��̂�n���B
	void copyTo( Array< T >* ) const;
private:
	struct Node{
		int mNext;
		int mPrev;
	};
	List( const List& ); //�R�s�[�R���X�g���N�^�֎~
	void operator=( const List& ); //����֎~

	T* mValues;
	Node* mNodes;
	Stack< int > mEmptyStack;
	int mCapacity;
	int mSize;
};

} //namespace GameLib
#include "GameLib/Base/Impl/ListImpl.h" //���g�͂��̒�

#endif

