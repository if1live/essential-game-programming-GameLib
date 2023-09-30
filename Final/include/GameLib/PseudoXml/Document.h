#ifndef INCLUDED_GAMELIB_PSEUDOXML_DOCUMENT_H
#define INCLUDED_GAMELIB_PSEUDOXML_DOCUMENT_H

#include <string>

namespace GameLib{
template< class T > class Array;
namespace PseudoXml{
using namespace std;

class Element;
class ConstElement;

///�t�@�C���A�N�Z�X�@�\�����j�Zxml�����N���X
class Document{
public:
	///�������ݗp�ɋ�Ő���
	static Document create();
	///�t�@�C������R���X�g���N�g(�vFileIO������)
	static Document create( const char* filename );
	///���[�h�ςݕ����񂩂�R���X�g���N�g
	static Document create( const char* data, int size );
	///��Element�Q�b�g
	Element root();
	///��Element�Q�b�g
	ConstElement root() const; 
	///���[�h�ς݂��ۂ�
	bool isReady();
	///�ǂݍ��ݒ��G���[������������
	bool isError() const;
	///�j�Zxml�e�L�X�g�𐶐�
	void convertToString( string* out ) const;
	///�j�Zxml�o�C�i���𐶐�
	void convertToBinary( Array< char >* out ) const;
	///�j�Zxml�t�@�C���𐶐�(�vFileIO������)
	void write( const char* filename, bool isBinary = false ) const;
	///�q�̐��Q�b�g
	int childNumber() const;
	///�q�Q�b�g
	Element child( int i );
	///�q�Q�b�g
	ConstElement child( int i ) const;

	//�ȉ��ӎ����Ȃ��Ă���
	Document();
	Document( const Document& );
	~Document();
	operator void*() const;
	Document& operator=( const Document& );
	void release();
	bool operator==( const Document& ) const;
	bool operator!=( const Document& ) const;

	class Impl;
private:
	Impl* mImpl;
};

//const��
class ConstDocument{
public:
	///��Element�Q�b�g
	ConstElement root() const; 
	///�ǂݍ��ݒ��G���[������������
	bool isError() const;
	///�j�Zxml�e�L�X�g�𐶐�
	void convertToString( string* out ) const;
	///�j�Zxml�o�C�i���𐶐�
	void convertToBinary( Array< char >* out ) const;
	///�j�Zxml�t�@�C���𐶐�(�vFileIO������)
	void write( const char* filename, bool isBinary = false ) const;
	///�q�̐��Q�b�g
	int childNumber() const;
	///�q�Q�b�g
	ConstElement child( int i ) const;

	//�ȉ��m��Ȃ��Ă���
	ConstDocument();
	ConstDocument( const Document& );
	~ConstDocument();
	ConstDocument& operator=( const Document& );
	void release();
	operator void*() const;
	bool operator==( const Document& ) const;
	bool operator!=( const Document& ) const;
private:
	Document mBase;
};

} //namespace PseudoXml
} //namespace GameLib

#endif
