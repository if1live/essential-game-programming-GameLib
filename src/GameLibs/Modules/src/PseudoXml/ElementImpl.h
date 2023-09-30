#ifndef INCLUDED_GAMELIB_PSEUDOXML_ELEMENTIMPL_H
#define INCLUDED_GAMELIB_PSEUDOXML_ELEMENTIMPL_H

#include "GameLib/Base/Vector.h"
#include "GameLib/Base/OStringStream.h"
#include "GameLib/Base/IBinaryStream.h"
#include "PseudoXml/DocumentImpl.h"
#include "PseudoXml/Tag.h"
#include <map>
using namespace std;

namespace GameLib{
namespace PseudoXml{

class Element::Impl{
public:
	Impl() : 
	mChildren( 0 ),
	mChildNumber( 0 ),
	mAttributes( 0 ),
	mAttributeNumber( 0 ),
	mName( 0 ),
	mDocument( 0 ){
	}
	~Impl(){
		//�S��Document�����̂������Ă���̂ł����ł͉������Ȃ�
		mChildren = 0;
		mAttributes = 0;
		mName = 0;
		mDocument = 0;
	}
	void build( Tag* tag, const char** p, const char* e ){
		//����͎����R���X�g���N�^�Ȃ̂ŁA��x�ڂ̌Ăяo���͕s��
		ASSERT( mAttributeNumber == 0 && mChildNumber == 0 );
		//���O���ڐA

		mName = mDocument->allocateString( *tag->name() );

		//�A�g���r���[�g���ڐA
		mAttributeNumber = tag->attributeNumber();
		mAttributes = mDocument->allocateAttribute( mAttributeNumber );
		tag->giveAttributes( mAttributes );

		//��v�f�^�O�Ȃ�q�͂��Ȃ��̂ł����Ŕ�����B
		if ( tag->type() == Tag::TYPE_BEGIN_END ){
			return;
		}

		//�ꎞ�q�G�������g���X�g
		Tank< Element::Impl > children;
		//�^�O�T��
		while ( *p < e ){
			if ( **p == '<' ){ //����
				++( *p ); //<�̎��ֈړ�
				Tag tmpTag( p, e, mDocument ); //�^�O���߂��ۓ���
				Tag::Type type = tmpTag.type();
				if ( type == Tag::TYPE_END ){ //�I���^�O�Ȃ�I���B������B
					break; //�I���
				}else if ( type == Tag::TYPE_COMMENT ){
					; //�������Ȃ��B
				}else{ //�J�n�A��v�f�ǂ���ł����̏����͕K�v
					Element::Impl* tmpE = children.add(); //�q�ɒǉ�
					tmpE->setDocument( mDocument );
					tmpE->build( &tmpTag, p, e );
					tmpE = 0;
				}
			}else{
				++( *p ); //<���o��܂Ŗ������܂���
			}
		}
		//�q�G�������g��{�m��
		mChildNumber = children.size();
		mChildren = mDocument->allocateElement( mChildNumber );
		children.copyTo( mChildren );
	}
	void build( IBinaryStream* s, const char* stringBuffer ){
		mName = stringBuffer + s->readInt();
		mAttributeNumber = s->readInt();
		mAttributes = mDocument->allocateAttribute( mAttributeNumber );
		for ( int i = 0; i < mAttributeNumber ; ++i){
			const char* name = stringBuffer + s->readInt();
			const char* value = stringBuffer + s->readInt();
			mAttributes[ i ].setDocument( mDocument );
			mAttributes[ i ].setNameReference( name );
			if ( *value != '\0' ){
				mAttributes[ i ].setValueReference( value );
			}
		}
		mChildNumber = s->readInt();
		mChildren = mDocument->allocateElement( mChildNumber );
		for ( int i = 0; i < mChildNumber; ++i ){
			mChildren[ i ].setDocument( mDocument );
			mChildren[ i ].build( s, stringBuffer );
		}
	}
	//�P���Ɉ�ÂĂ�ōs���B
	void convertToString( OStringStream* out, int indent ) const {
		//�C���f���g�̐������^�u������
		for ( int i = 0; i < indent; ++i ){
			*out << '\t';
		}
		//�^�O�J�n�ƃG�������g��
		*out << '<' << mName;
		//�A�g���r���[�g��������
		//5�����ɏo������ς���
		int an = mAttributeNumber;
		if ( an >= 5 ){
			*out << "\r\n";
			//��������S�����s
			for ( int i = 0; i < an; ++i ){
				//�C���f���g�̐������^�u������
				for ( int j = 0; j < indent; ++j ){
					*out << '\t';
				}
				*out << mAttributes[ i ].name();
				const char* value = mAttributes[ i ].value();
				if ( value ){
					*out << "=\"" << mAttributes[ i ].value() << '"';
				}
				if ( i != an - 1 ){
					*out << "\r\n"; //�Ō�͉��s���Ȃ�
				}
			}
		}else{
			for ( int i = 0; i < an; ++i ){
				*out << ' '; //�X�y�[�X
				*out << mAttributes[ i ].name();
				const char* value = mAttributes[ i ].value();
				if ( value ){
					*out << "=\"" << mAttributes[ i ].value() << '"';
				}
			}
		}
		if ( mChildNumber > 0 ){ //�q������ꍇ
			*out << ">\r\n"; //�J�n�^�O�I���
			//�q�ɗ���
			for ( int i = 0; i < mChildNumber; ++i ){
				//���O���Z�b�g����Ă���΁B
				if ( mChildren[ i ].name() ){
					mChildren[ i ].convertToString( out, indent + 1 );
				}
			}
			//�C���f���g�̐������^�u������
			for ( int i = 0; i < indent; ++i ){
				*out << '\t';
			}
			//�I���^�O
			*out << "</" << mName << ">\r\n";
		}else{ //�q�����Ȃ��Ȃ��v�f�^�O�ł悩�낤
			*out << "/>\r\n";
		}
	}
	void convertToBinary( 
	OStringStream* out, 
	OStringStream* stringBuffer,
	map< RefString, int >* stringMap ) const {
		typedef map< RefString, int > Map;
		RefString name( mName );
		Map::iterator it = stringMap->find( name );
		int pos = 0;
		if ( it == stringMap->end() ){
			pos = stringBuffer->size();
			*stringBuffer << name;
			stringBuffer->write( "\0", 1 );
			stringMap->insert( make_pair( name, pos ) );
		}else{
			pos = it->second;
		}
		out->write( pos ); //nameOffset
		out->write( mAttributeNumber ); //attributeNumber
		for ( int i = 0; i < mAttributeNumber; ++i ){
			const Attribute::Impl& a = mAttributes[ i ];
			RefString aName( a.name() );
			RefString aValue;
			if ( a.value() ){
				aValue = a.value();
			}
			//name
			it = stringMap->find( aName );
			pos = 0;
			if ( it == stringMap->end() ){
				pos = stringBuffer->size();
				*stringBuffer << aName;
				stringBuffer->write( "\0", 1 );
				stringMap->insert( make_pair( aName, pos ) );
			}else{
				pos = it->second;
			}
			out->write( pos ); //Attribute::nameOffset
			//value
			it = stringMap->find( aValue );
			pos = 0;
			if ( it == stringMap->end() ){
				pos = stringBuffer->size();
				*stringBuffer << aValue;
				stringBuffer->write( "\0", 1 );
				stringMap->insert( make_pair( aValue, pos ) );
			}else{
				pos = it->second;
			}
			out->write( pos ); //Attribute::valueOffset
		}
		//�q��
		out->write( mChildNumber ); //childNumber
		for ( int i = 0;i < mChildNumber; ++ i ){
			if ( mChildren[ i ].name() ){ //���O��������̂����o���Ȃ�
				mChildren[ i ].convertToBinary( out, stringBuffer, stringMap );
			}
		}
	}
	int attributeNumber() const {
		return mAttributeNumber;
	}
	void setAttributeNumber( int n ){
		ASSERT( mAttributeNumber == 0 );
		mAttributeNumber = n;
		mAttributes = mDocument->allocateAttribute( n );
		for ( int i = 0; i < mAttributeNumber; ++i ){
			mAttributes[ i ].setDocument( mDocument );
		}
	}
	Attribute::Impl* attribute( int i ){
		ASSERT( i >= 0 && i < mAttributeNumber );
		return &mAttributes[ i ];
	}
	const Attribute::Impl* attribute( int i ) const {
		return &mAttributes[ i ];
	}
	int childNumber() const {
		return mChildNumber;
	}
	void setChildNumber( int n ){
		ASSERT( mChildNumber == 0 );
		mChildNumber = n;
		mChildren = mDocument->allocateElement( n );
		for ( int i = 0; i < mChildNumber; ++i ){
			mChildren[ i ].setDocument( mDocument );
		}
	}
	Element::Impl* child( int i ){
		return &mChildren[ i ];
	}
	const Element::Impl* child( int i ) const {
		return &mChildren[ i ];
	}
	const char* name() const {
		return mName;
	}
	void setName( const char* name ){
		mName = mDocument->allocateString( name );
	}
	void setNameReference( const char* name ){
		mName = name;
	}
	void setDocument( Document::Impl* d ){
		mDocument = d;
	}
	Document::Impl* document(){
		return mDocument;
	}
private:
	Element::Impl* mChildren;
	int mChildNumber;
	Attribute::Impl* mAttributes;
	int mAttributeNumber;
	const char* mName;
	Document::Impl* mDocument;
};

} //namespace PseudoXml
} //namespace GameLib
#endif
