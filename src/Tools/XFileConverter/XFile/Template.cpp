#include "GameLib/GameLib.h"
#include "XFile/Template.h"
#include "XFile/Token.h"
#include "XFile/Element.h"
#include "GameLib/Base/Tank.h"
#include "XFile/Parser.h"
#include <sstream>

namespace XFile{

using namespace GameLib;
using namespace std;

Template::Template( RefString name, const RefString& uuid, int childNumber ) :
mName( name ),
mChildren( childNumber ), 
mUuid( uuid ){
}

Template::~Template(){
}

void Template::setChild( int i, TemplateLine** tl ){
	mChildren[ i ] = *tl;
	*tl = 0;
}

const RefString* Template::getType() const {
	return &mName;
}

const RefString* Template::getChildName( int index ) const {
	if ( index >= mChildren.size() ){ //���傫���ԍ���v������邱�Ƃ�����B�g�������B0��Ԃ��B
		return 0;
	}else{
		return &mChildren[ index ]->mName;
	}
}

const RefString* Template::getChildType( int index ) const {
	if ( index >= mChildren.size() ){ //���傫���ԍ���v������邱�Ƃ�����B�g�������B0��Ԃ��B
		return 0;
	}else if ( mChildren[ index ]->mTemplate ){ //�����^
		return mChildren[ index ]->mTemplate->getType();
	}else{ //�v���~�e�B�u
		return &mChildren[ index ]->mPrimitiveType;
	}
}


int Template::createElement( 
Element** elementOut, 
const Array< Token >& t,
int p,
const Parser& parser,
const RefString& name,
map< RefString, Element* >* namedElements,
int* level ) const {

//if ( p % 1000 == 0 ){ cout << p << endl; }

	const int n = t.size();
	Tank< CompositeElement::Child > children;
	map< RefString, int > arrayIndices;
	//���M�����[�f�[�^
	for ( int i = 0; i < mChildren.size(); ++i ){
		++( *level );
		CompositeElement::Child c; //�q
		if ( ( p < n ) && ( t[ p ] == Token::LEFT_BRACE ) ){ //�Q�Ɨ��܂�����...
			p = parseReference( &c.mElement, t, p, namedElements );
		}else{
			const TemplateLine& line = *mChildren[ i ];
			if ( line.mIndices.size() > 0 ){ //�z��ł�
				p = createArray( &c.mElement, line, t, p, arrayIndices, parser, namedElements, level );
			}else if ( line.mTemplate ){ //�����^�ł���
				RefString nullStr;
				p = line.mTemplate->createElement( &c.mElement, t, p, parser, nullStr, namedElements, level );
			}else{ //�v���~�e�B�u�ł�
				c.mPrimitive = *t[ p ].get();
				++p;
				int size = parser.toInt( c.mPrimitive ); //�Y�����ɂȂ肤�邩���ׂ܂�
				if ( size > 0 ){
					arrayIndices.insert( make_pair( line.mName, size ) );
				}
			}
		}
		children.add( c );
		Parser::dieIf( p >= n );
		if ( t[ p ] == Token::SEMICOLON ){
			++p;
		}else{ //���肦�Ȃ�
#if 0 //���^�Z�R�C�A�Ή��B�Z�~�R�����������X���[
			Parder::dieIf( true );
#endif
		}
		--( *level );
#if 1 //���^�Z�R�C�A�Ή��R�[�h �]���ȃZ�~�R����������
		if ( ( *level == 0 ) && ( p < n ) && ( t[ p ] == Token::SEMICOLON ) ){ //���肦�Ȃ�
			++p;
		}
#endif
	}
	//�ǉ��f�[�^
	while ( p < n ){
		CompositeElement::Child c;
		if ( t[ p ] == Token::LEFT_BRACE ){ //�Q�Ɨ��܂�����...
			p = parseReference( &c.mElement, t, p, namedElements );
		}else if ( t[ p ] == Token::IDENTIFIER ){
			//�^�������o���܂�
			const RefString* typeName = t[ p ].get();
			++p;
			//�ϐ������o��
			RefString childName;
			if ( ( p < n ) && ( t[ p ] == Token::IDENTIFIER ) ){
				childName = *t[ p ].get();
				++p;
			}
			//{
			Parser::dieIf( ( p >= n ) || ( t[ p ] != Token::LEFT_BRACE ) ); //���肦�Ȃ�
			++p;
			//�e���v���[�g�Ƀf�[�^������Ă��炢�܂��B
			if ( parser.isPrimitiveType( *typeName ) ){ //�v���~�e�B�u
				c.mPrimitive = *t[ p ].get();
				++p;
			}else{ //�����^
				const Template* tmpl = parser.getTemplate( *typeName );
				int childLevel = 0;
				p = tmpl->createElement( &c.mElement, t, p, parser, childName, namedElements, &childLevel );
				namedElements->insert( make_pair( childName, c.mElement ) );
			}
			//}
			Parser::dieIf( ( p >= n ) || ( t[ p ] != Token::RIGHT_BRACE ) ); //���肦�Ȃ�
			++p;
		}else{
			break; //�Q�Ƃł��f�[�^���̂ł��Ȃ��B�܂�I���B
		}
		children.add( c );
	}
	CompositeElement* e = NEW CompositeElement( this, children.size(), name );
	for ( int i = 0; !children.isEnd(); ++i ){
		CompositeElement::Child* c = children.get();
		e->setChild( i, *c );
		children.toNext();
	}
	*elementOut = e;

	return p;
}

int Template::createArray(
Element** elementOut, 
const TemplateLine& line,
const Array< Token >& t,
int p,
const map< RefString, int >& arrayIndices,
const Parser& parser,
map< RefString, Element* >* namedElements,
int* level ) const {
	//�ŏ��ɂ�邱�Ƃ͔z��̓Y�����̎����Ɗe�����̃T�C�Y���擾���邱�ƁB�����͍œK���̗]�n������
	const int n = t.size();
	const Array< RefString >& indices = line.mIndices;
	int dimension = indices.size();
	Array< int > arraySizes( dimension );
	int k = 0;
	int arraySize = 1;
	for ( int i = 0; i < indices.size(); ++i ){
		const RefString& s = indices[ i ];
		int size = parser.toInt( s );
		if ( size > 0 ){
			arraySizes[ k ] = size;
		}else{
			map< RefString, int >::const_iterator l = arrayIndices.find( s );
			Parser::dieIf( l == arrayIndices.end() );
			arraySizes[ k ] = l->second;
		}
		arraySize *= arraySizes[ k ];
	}
	//�v���~�e�B�u�z�񂩕����^���ŕ���
	if ( line.mTemplate ){ //�����^
		CompositeArrayElement* a = NEW CompositeArrayElement( arraySizes );
		for ( int i = 0; i < arraySize; ++i ){
			Element* te = 0;
			RefString nullStr;
			p = line.mTemplate->createElement( &te, t, p, parser, nullStr, namedElements, level );
			if ( i < arraySize - 1 ){ //�ŏI�v�f�����̓J���}���Ȃ��B�Ȃ�Ă������I
				Parser::dieIf( ( p >= n ) || ( t[ p ] != Token::COMMA ) );
				++p;
			}
			a->setElement( i, &te );
		}
		*elementOut = a;
	}else{ //�v���~�e�B�u
		PrimitiveArrayElement* a = NEW PrimitiveArrayElement( arraySizes, line.mPrimitiveType );
		for ( int i = 0; i < arraySize; ++i ){
			a->setElement( i, *t[ p ].get() );
			++p;
			if ( i < arraySize - 1 ){ //�ŏI�v�f�����̓J���}���Ȃ��B�Ȃ�Ă������I
				Parser::dieIf( ( p >= n ) || ( t[ p ] != Token::COMMA ) );
				++p;
			}
		}
		*elementOut = a;
	}
	return p;
}

//�Q�Ə��� { name | UUID | name UUID }
int Template::parseReference(
Element** elementOut,
const Array< Token >& t,
int p,
map< RefString, Element* >* ) const {
	const int n = t.size();

	++p; //�����������΂�
	RefString refName;
	if ( ( p < n ) && ( t[ p ] == Token::IDENTIFIER ) ){
		refName = *t[ p ].get();
		++p;
	}
	if ( ( p < n ) && ( t[ p ] == Token::UUID ) ){ //UUID�͖��O���Ȃ�����������Ă���B
		++p;
		if ( refName.size() == 0 ){
			refName = *t[ p ].get();
		}
	}
	Parser::dieIf( refName.size() == 0 );
	Parser::dieIf( ( p >= n ) || ( t[ p ] != Token::RIGHT_BRACE ) ); //���肦�Ȃ�
	++p;

	*elementOut = NEW ReferenceElement( refName );
/*
	//�Q�Ƃ��������݂܂�
	map< RefString, Element* >::iterator it = namedElements->find( refName );
	Parser::dieIf( it == namedElements->end() );
	*elementOut = it->second;
*/
	return p;
}


} //namespace XFile

