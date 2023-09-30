#include "GameLib/GameLib.h"
#include "PseudoXml/Document.h"
#include "PseudoXml/Element.h"
#include "PseudoXml/Attribute.h"
#include "PseudoXml/Parser.h"

namespace PseudoXml{

class ElementData{
public:
	~ElementData(){
		//��̂͂��ł�
		assert( mAttributes.size() == 0 && mChildren.size() == 0 );
	}
	string mName; //�G�������g��
	list< Attribute* > mAttributes; //�A�g���r���[�g
	list< ElementData* > mChildren; //�q���f�[�^
};

Parser::Parser( const char* data, int size ) :
mData( data ),
mDataSize( size ),
mMode( 0 ),
mInEndTag( false ){
}

Parser::~Parser(){
	//�����ɗ���Ƃ��ɂ͋�ɂȂ��Ă���͂�
	assert( mElements.size() == 0 );
}

namespace {
//a-z,A-Z,0-9,_�̂ǂꂩ���𒲂ׂ�֐�
//�킩��₷�����x���B�������̕��@���������Ă݂悤�B
bool static isNormalChar( char c ){
	if ( c >= '0' && c <= '9' ){
		return true;
	}
	if ( c >= 'a' && c <= 'z' ){
		return true;
	}
	if ( c >= 'A' && c <= 'Z' ){
		return true;
	}
	if ( c == '_' ){
		return true;
	}
	return false;
}

} //namespace {}

//��ԑJ�ڕ\�B�Œ���Ȃ̂ŁA�G���[�ɂُ͈�ȂقǊ��e�ȍ��ɂȂ��Ă���B
/*
<��Ԕԍ����X�g>
0:�������
1:<�̏�
2:�G�������g��������
3:�G�������g���̌�̋󔒁B�A�g���r���[�g�̊Ԃ̋󔒁B
4:�A�g���r���[�g��
5:=�̏�
6:�A�g���r���[�g�l(""�̒��g)
7:>�̏�

<��ԑJ�ڃ��X�g>
�L��:
c = [a-zA-Z0-9_] �܂蕁�ʂ̕���
s = ���̏����Ɉ���������Ȃ��c��̕���

0,<,1 �J�n
1,/,1 �I���^�O�t���O���Ă�
1,s,2 �G�������g��������ŏ��̈ꕶ��
2,c,2 �G�������g��������ɒǉ�
2,>,7
2,s,3 �G�������g���m��
3,>,7
3,c,4 �A�g���r���[�g���ŏ��̈ꕶ��
3,s,3 �ǂݔ�΂�
4,=,5 =�����B�A�g���r���[�g���m��
4,s,4 �A�g���r���[�g���ɒǉ�
5,",6 �_�u���N�H�e�C�V���������B�A�g���r���[�g�l�Ɉڍs
5,s,5 �ǂݔ�΂�
6,",3 �A�g���r���[�g�l�m��B�A�g���r���[�g�����X�g�ɒǉ��B
6,s,6 �A�g���r���[�g�l������ɒǉ�
7,c,0 �������Ă�����0�B�^�O�����������̂ł����ł��낢�낷��B
*/
Element* Parser::build(){
	//�A�g���r���[�g�̖��O�ƒl�̈ꎞ�I�Ȋi�[�ꏊ
	string name;
	string value;
	
	//�_�~�[�e
	ElementData* parent = new ElementData;
	parent->mName = "<ROOT>"; //���ʈ����̍����G�������g
	elements.push_back( parent ); //�����Z�b�g

	//���ꂩ����G�������g
	ElementData* child = new ElementData;

	//�I���^�O�ł����H
	bool isEndTag = false;

	for ( int i = 0; i < mDataSize; ++i ){
		char c = mData[ i ];
		//���[�h�ŕ���
		switch ( mMode ){
			case 0: //�������
				switch ( c ){
					case '<': mMode = 1; break;
					default: break; //�������Ȃ�
				}
				break;
			case 1: //<�̏�
				switch ( c ){
					case '/': mInEndTag = true; break; //�I���^�O�ł���
					default: child->mName += c; mMode = 2; break; //�G�������g���˓�
				}
				break;
			case 2: //�G�������g��
				if ( c == '>' ){
					mMode = 7;
				}else if ( isNormalChar( c ) ){
					child->mName += c;
				}else{
					mMode = 3; //�G�������g���𔲂���
				}
				break;
			case 3: //�G�������g���̌�̋�
				if ( c == '>' ){
					mMode = 7;
				}else if ( isNormalChar( c ) ){
					name += c; //�A�g���r���[�g���ǉ�
					mMode = 4;
				}else{
					; //�������Ȃ�
				}
				break;
			case 4: //�A�g���r���[�g��
				switch ( c ){
					case '=': mMode = 5; break;
					default: name += c; break;
				}
				break;
			case 5: //=�̏�
				switch ( c ){
					case '"': mMode = 6; break;
					default: break; //�������Ȃ�
				}
				break;
			case 6: //�A�g���r���[�g�l
				switch ( c ){
					case '"': 
						mMode = 3; //�G�������g���̌�̋�
						//�A�g���r���[�g�ǉ�
						child->mAttributes.push_back( new Attribute( name.c_str(), value.c_str() ) );
						//���O�ƒl��������
						name.clear();
						value.clear();
						break;
					default: value += c; break;
				}
				break;
			case 7: //>�̏�B�^�O����
				if ( mInEndTag ){ //�I���^�O�Ȃ̂ŁAElement���X�g����i�߂�
					elements.pop_back();
					parent = elements.back(); //�Ō�̂����̐e�ł��B
					mInEndTag = false;
				}else{ //�J�n�^�O�Ȃ̂ŃG�������g�����̎q���ɒǉ�
					parent->mChildren.push_back( child );
					parent = child; //�V�����e�͂��̐l�ł��B
					child = new ElementData; //�V�����̂�����ď���
				}
				mMode = 0; //�ⓚ���p��0��
				break;
		}
	}
	//�߂ł����ޗ�����������̂ŁAElement������Ă����܂��B
	Element* root = new Element;
	ElementData* rootData = elements.front();
	root->setName( rootData->mName );
	root->setAttributeNumber( rootData->mAttributes.size() );
	int j = 0;
	for ( list< Attributes* >::iterator i = rootData->mAttributes.begin(); i != rootData->mAttributes.end(); ++i ){
		root->setAttribute( j, *i );
		++j;
		*i = 0; //�Ǘ����ڂ�
	}
	root->setChildNumber( rootData->mChildren.size() );

}

} //namespace PseudoXml
