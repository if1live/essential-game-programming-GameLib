#include "GameLib/GameLib.h"
#include "PseudoXml/Tag.h"
#include "PseudoXml/AttributeImpl.h"
#include "PseudoXml/DocumentImpl.h"
#include "GameLib/Base/RefString.h"
using namespace std;

namespace GameLib{
namespace PseudoXml{

//��ԑJ�ڕ\�B
/*
<��Ԕԍ����X�g>
0:<�̏�B�������
1:�G�������g��������
2:�G�������g���̌�̋󔒁B�A�g���r���[�g�̊Ԃ̋󔒁B
3:�A�g���r���[�g��
4:=�̏�
5:�A�g���r���[�g�l(""�̒��g)
6:��v�f�^�O�L��/�̏�
7:�A�g���r���[�g����=�̊Ԃ̋�
8:�A�g���r���[�g�l(''�̒��g)
9:�A�g���r���[�g�l(����܂�ĂȂ��z)
10:�R�����g <! ... -->���R�����g�B�{����<!-- ... -->�����A!���o���i�K�ŕ��ʂ���Ȃ����A�����B(���͂܂����񂾂���...<!ELEMENT�݂����Ȃ̂�����)
11:�R�����g��-����
12:�R�����g��--����
13:?�R�����g�� <? ... ?>���R�����g
14:?�R�����g��?����
E:�I��

<��ԑJ�ڃ��X�g>
�L��:
c = [a-zA-Z0-9_] �܂蕁�ʂ̕���
s = ���̏����Ɉ���������Ȃ��c��̕���

0,/,0 �I���^�O�t���O���Ă�
0,!,10 �R�����g�����
0,?,13 �R�����g�����
0,c,1 �G�������g��������ŏ��̈ꕶ��
0,s,0 �Ƃǂ܂�
1,c,1 �G�������g��������ɒǉ�
1,>,E
1,s,2 �G�������g���m��
2,>,E
2,/,6 ��v�f�^�O/����
2,c,3 �A�g���r���[�g���ŏ��̈ꕶ��
2,s,2 �ǂݔ�΂�
3,=,4 =�����B�A�g���r���[�g���m��
3,c,3 �A�g���r���[�g���ɒǉ�
3,s,7 �󔒂ցB�A�g���r���[�g���m��
4,",5 �_�u���N�H�e�C�V���������B�A�g���r���[�g�l�Ɉڍs
4,/,8 �V���O���N�H�e�C�V���������B�A�g���r���[�g�l�Ɉڍs
4,c,9 �A�g���r���[�g�l�Ɉڍs
4,s,4 �ǂݔ�΂�
5,",2 �A�g���r���[�g�l�m��B�A�g���r���[�g�����X�g�ɒǉ��B
5,s,5 �A�g���r���[�g�l������ɒǉ�
6,>,E ��v�f�Ƃ��ďI���
6,s,6 �ǂݔ�΂�
7,=,4 =����
7,s,7 �ǂݔ�΂�
8,',2 �A�g���r���[�g�l�m��B
8,s,8 �A�g���r���[�g�l�ɒǉ�
9,/,6 ��v�f�^�O�����B
9,>,E
9,c,9 �A�g���r���[�g�l�ɒǉ�
9,s,2 �A�g���r���[�g�l�m��
10,-,11
10,s,10
11,-,12
11,s,10
12,>,E
12,s,10
13,?,14
13,s,13
14,>,E
14,s,13
*/
Tag::Tag( const char** p, const char* e, Document::Impl* document ) : 
mType( TYPE_BEGIN ),
mDocument( document ){
	//�G�������g���A�A�g���r���[�g�̖��O�ƒl�̈ꎞ�I�Ȋi�[�ꏊ
	const char* eName = 0;
	int eNameSize = 0;
	const char* aName = 0;
	int aNameSize = 0;
	const char* aValue = 0;
	int aValueSize = 0;

	int m = 0; //���[�h�Bmode�Ƃ��ɂ����ق��������񂾂낤���A�g�p�񐔂��q�킶��Ȃ��̂�1�����ɂ���
	bool end = false; //���[�v�𔲂���t���O
	while ( *p < e ){
		bool wide = false; //���{��ł����H
		char c = **p;
		switch ( m ){
			case 0: //�������
				if ( c == '/' ){
					mType = TYPE_END;
				}else if ( c == '!' ){ //�R�����g
					mType = TYPE_COMMENT;
					m = 10;
				}else if ( c == '?'){
					mType = TYPE_COMMENT;
					m = 13;
				}else if ( isNormalChar( c, &wide ) ){
					eName = *p;
					eNameSize = 1;
					if ( wide && ( ( *p + 1 ) < e ) ){
						++eNameSize;
					}
					m = 1;
				}else{
					; //�������Ȃ�
				}
				break;
			case 1: //�G�������g��
				if ( c == '>' ){
					end = true;
				}else if ( isNormalChar( c, &wide ) ){
					++eNameSize;
					if ( wide && ( ( *p + 1 ) < e ) ){
						++eNameSize;
					}
				}else{
					m = 2; //�G�������g���𔲂���
				}
				break;
			case 2: //�G�������g���̌�̋�
				if ( c == '>' ){
					end = true;
				}else if ( c == '/' ){
					mType = TYPE_BEGIN_END; //��v�f�^�O�ł�
					m = 6;
				}else if ( isNormalChar( c, &wide ) ){
					aName = *p;
					aNameSize = 1;
					if ( wide && ( ( *p + 1 ) < e ) ){
						++aNameSize;
					}
					m = 3;
				}else{
					; //�������Ȃ�
				}
				break;
			case 3: //�A�g���r���[�g��
				if ( c == '=' ){
					m = 4;
				}else if ( isNormalChar( c, &wide ) ){
					++aNameSize;
					if ( wide && ( ( *p + 1 ) < e ) ){
						++aNameSize;
					}
				}else{
					m = 7; //�󔒂�
				}
				break;
			case 4: //=�̏�
				if ( c == '"' ){
					aValue = *p + 1;
					m = 5;
				}else if ( c == '\'' ){
					aValue = *p + 1;
					m = 8;
				}else if ( isNormalChar( c, &wide ) ){
					aValue = *p;
					aValueSize = 1;
					if ( wide && ( ( *p + 1 ) < e ) ){
						++aValueSize;
					}
					m = 9;
				}else{
					; //�������Ȃ�
				}
				break;
			case 5: //�A�g���r���[�g�l
				if ( c == '"' ){
					m = 2; //�G�������g���̌�̋�
					addAttribute( aName, aNameSize, aValue, aValueSize );
					aNameSize = aValueSize = 0;
				}else{
					++aValueSize;
					isNormalChar( c, &wide ); //���C�h���ǂ��������m�肽��
					if ( wide && ( ( *p + 1 ) < e ) ){
						++aValueSize;
					}
				}
				break;
			case 6: //��v�f�^�O��/�o����
				switch ( c ){
					case '>': end = true; break;
					default: break; //�ǂݔ�΂�������
				}
				break;
			case 7: //�A�g���r���[�g���̌�̋�
				if ( c == '=' ){
					m = 4;
				}else if ( c == '>' ){
					addAttribute( aName, aNameSize, aValue, aValueSize );
					aNameSize = aValueSize = 0;
					end = true;
				}else if ( isNormalChar( c, &wide ) ){ //�l�̂Ȃ��A�g���r���[�g
					addAttribute( aName, aNameSize, aValue, aValueSize );
					aNameSize = aValueSize = 0;
					//���̃A�g���r���[�g�ɓ���܂�
					aName = *p;
					aNameSize = 1;
					if ( wide && ( ( *p + 1 ) < e ) ){
						++aNameSize;
					}
					m = 3;
				}else{
					; //�������Ȃ�
				}
				break;
			case 8: //�A�g���r���[�g�l
				if ( c == '\'' ){
					m = 2; //�G�������g���̌�̋�
					addAttribute( aName, aNameSize, aValue, aValueSize );
					aNameSize = aValueSize = 0;
				}else{
					++aValueSize;
					isNormalChar( c, &wide ); //���C�h���ǂ��������m�肽��
					if ( wide && ( ( *p + 1 ) < e ) ){
						++aValueSize;
					}
				}
				break;
			case 9: //�A�g���r���[�g�l
				if ( c == '/' ){
					mType = TYPE_BEGIN_END; //��v�f�^�O�ł�
					m = 6;
					addAttribute( aName, aNameSize, aValue, aValueSize );
					aNameSize = aValueSize = 0;
				}else if ( c == '>' ){
					addAttribute( aName, aNameSize, aValue, aValueSize );
					aNameSize = aValueSize = 0;
					end = true;
				}else if ( isNormalChar( c, &wide ) ){
					++aValueSize;
					if ( wide && ( ( *p + 1 ) < e ) ){
						++aValueSize;
					}
				}else{
					m = 2; //�G�������g���̌�̋�
					addAttribute( aName, aNameSize, aValue, aValueSize );
					aNameSize = aValueSize = 0;
				}
			case 10: //�R�����g
				switch ( c ){
					case '-': m = 11; break;
					default: break; //�ǂݔ�΂�������
				}
				break;
			case 11: //�R�����g-
				switch ( c ){
					case '-': m = 12; break;
					default: m = 10; break; //�R�����g����
				}
				break;
			case 12: //�R�����g--
				switch ( c ){
					case '>': end = true; break;
					default: m = 10; break; //�R�����g����
				}
				break;
			case 13: //?�R�����g
				switch ( c ){
					case '?': m = 14; break;
					default: break; //�ǂݔ�΂�������
				}
				break;
			case 14: //?�R�����g?
				switch ( c ){
					case '>': end = true; break;
					default: m = 13; break; //�R�����g����
				}
				break;
		}
		*p += ( wide ) ? 2 : 1; //�i�߂�
		if ( end ){ //�I���ł� �i�߂Ă���I��邱�Ƃɒ��ӂ��悤�B��̍s�Ƌt�ɂȂ�ƃo�O��
			break;
		}
	}
	mName.set( eName, eNameSize );
}

} //namespace PseudoXml
} //namespace GameLib
