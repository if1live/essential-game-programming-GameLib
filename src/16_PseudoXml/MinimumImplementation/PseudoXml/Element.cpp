#include "GameLib/GameLib.h"
#include "PseudoXml/Tag.h"
#include "PseudoXml/Element.h"
#include "PseudoXml/Attribute.h"
using namespace std;

namespace PseudoXml{

Element::Element(){
}

Element::Element( const char* name ) :
mName( name ){
}

Element::Element( Tag* beginTag, const char** p, const char* e ){
	//���O���ڐA
	mName = *( beginTag->name() );
	//�A�g���r���[�g���ڐA(�R�s�[�łȂ��|�C���^�����̂܂܂�����č�����)
	mAttributes = *beginTag->attributes(); //�ۂ��ƃR�s�[

	//�^�O�T��
	while ( *p < e ){
		if ( **p == '<' ){ //����
			++( *p ); //<�̎��ֈړ�
			Tag tag( p, e ); //�^�O���߂��ۓ���
			Tag::Type type = tag.type();
			if ( type == Tag::TYPE_BEGIN ){ //�J�n�^�O
				mChildren.push_back( new Element( &tag, p, e ) ); //�q�ɒǉ�
			}else if ( type == Tag::TYPE_END ){ //�I���^�O
				break; //�I���
			}
		}else{
			++( *p ); //<���o��܂Ŗ������܂���
		}
	}
}

Element::~Element(){
	for ( unsigned i = 0; i < mAttributes.size(); ++i ){
		SAFE_DELETE( mAttributes[ i ] );
	}
	for ( unsigned i = 0; i < mChildren.size(); ++i ){
		SAFE_DELETE( mChildren[ i ] );
	}
}

int Element::attributeNumber() const {
	return static_cast< int >( mAttributes.size() );
}

const Attribute* Element::attribute( int i ) const {
	return mAttributes[ i ];
}

Attribute* Element::attribute( int i ){
	return mAttributes[ i ];
}

void Element::setAttributeNumber( int n ){
	//�����镪�͎̂Ă�
	for ( unsigned i = 0; i < mAttributes.size(); ++i ){
		SAFE_DELETE( mAttributes[ i ] );
	}
	mAttributes.resize( n );
	for ( int i = 0; i < n; ++i ){
		mAttributes[ i ] = new Attribute();
	}
}

int Element::childNumber() const {
	return static_cast< int >( mChildren.size() );
}

const Element* Element::child( int i ) const {
	return mChildren[ i ];
}

Element* Element::child( int i ){
	return mChildren[ i ];
}

void Element::setChildNumber( int n ){
	//�����镪�͎̂Ă�
	for ( unsigned i = 0; i < mChildren.size(); ++i ){
		SAFE_DELETE( mChildren[ i ] );
	}
	mChildren.resize( n );
	for ( int i = 0; i < n; ++i ){
		mChildren[ i ] = new Element();
	}
}

const string* Element::name() const {
	return &mName;
}

void Element::setName( const char* name ){
	mName = name;
}

void Element::convertToString( string* out, int indent ) const {
	//�C���f���g�̐������^�u������
	for ( int i = 0; i < indent; ++i ){
		*out += '\t';
	}
	//�^�O�J�n�ƃG�������g��
	*out += '<';
	*out += mName;
	//�A�g���r���[�g��������
	for ( unsigned i = 0; i < mAttributes.size(); ++i ){
		if ( mAttributes[ i ] ){
			*out += ' '; //�X�y�[�X
			*out += *( mAttributes[ i ]->name() );
			*out += "=\"";
			*out += *( mAttributes[ i ]->value() );
			*out += '"';
		}
	}
	*out += ">\r\n"; //�J�n�^�O�I���
	//�q�ɗ���
	for ( unsigned i = 0; i < mChildren.size(); ++i ){
		if ( mChildren[ i ] ){
			mChildren[ i ]->convertToString( out, indent + 1 );
		}
	}
	//�C���f���g�̐������^�u������
	for ( int i = 0; i < indent; ++i ){
		*out += '\t';
	}
	//�I���^�O
	*out += "</";
	*out += mName;
	*out += ">\r\n";
}

} //namespace PseudoXml
