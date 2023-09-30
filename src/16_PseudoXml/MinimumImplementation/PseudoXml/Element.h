#ifndef INCLUDED_PSEUDO_XML_ELEMENT_H
#define INCLUDED_PSEUDO_XML_ELEMENT_H

#include <string>
#include <vector>
using namespace std;

namespace PseudoXml{

class Tag;
class Attribute;

class Element{
public:
	Element();
	Element( const char* name ); //���O����R���X�g���N�g
	Element( Tag*, const char** readPointer, const char* end );
	~Element();
	//�A�g���r���[�g�Q�b�g�n
	int attributeNumber() const;
	const Attribute* attribute( int i ) const;
	Attribute* attribute( int i );
	//�A�g���r���[�g�Z�b�g�n
	void setAttributeNumber( int );
	//�q���Q�b�g�n
	int childNumber() const;
	const Element* child( int i ) const;
	Element* child( int i );
	//�q���Z�b�g�n
	void setChildNumber( int );
	//���O�Q�b�g
	const string* name() const;
	//���O�ύX
	void setName( const char* );
	//������
	void convertToString( string* out, int indent ) const;
private:
	vector< Attribute* > mAttributes;
	vector< Element* > mChildren;
	string mName;
};

} //namespace PseudoXml

#endif
