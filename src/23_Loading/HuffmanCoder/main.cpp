//�M���M���܂ŃR�[�h��P���ɂ��邽�߂ɁA���x�͊��S�ɓx�O�����Ă���B���ɒx���B
//�ǂ�����Α����Ȃ邩�l���Ă݂�Ƃ������낤�B

#include <fstream>
#include <iostream>
#include <cassert> //�W����assert���g�����߁BGameLib����ASSERT�Ǝg�����͓����B
#include "BitStream.h" //�r�b�g�P�ʂ̓ǂݏ��������邽�߂̃N���X
using namespace std;

void encode( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn );
void decode( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn );
//�r�b�g�P�ʂŔ�ׂ�֐�
bool isEqual( unsigned char* a, unsigned char* b, int sizeInBit );

//�R�}���h���C���̑��������t�@�C������
int main( int, char** argv ){
	//�Ƃ肠�����ہX�ǂݍ��ށB
	ifstream in( argv[ 1 ], ifstream::binary ); 
	//argv[1]���������Ȃ̂ɂ͊���Ă��炤�����Ȃ��B
	//�v���W�F�N�g�̃v���p�e�B�́u�f�o�O�v�̂Ƃ���ŃR�}���h���C���������ݒ�ł���Btest.txt�Ə����Ă���͂����B
	in.seekg( 0, ifstream::end );
	int inSize = static_cast< int >( in.tellg() );
	in.seekg( 0, ifstream::beg );
	char* inData = new char[ inSize ];
	in.read( inData, inSize );

	//�������݃o�b�t�@���ہX�m�ہB
	//�n�t�}�������̍ň��P�[�X�́A�S��8bit�����ɂȂ��āA�����ɕ����\����������ꍇ�B
	//�����\�͕�����1byte+�����ő�255bit(8byte)�ŁA9*256�o�C�g�B����Ɍ��T�C�Y4�o�C�g
	int outMaxSize = inSize + 9 * 256 + 4;
	char* outData = new char[ outMaxSize ];

	//���Ⴀ���k�����[
	int outSize;
	encode( 
		reinterpret_cast< unsigned char* >( outData ), 
		&outSize, 
		reinterpret_cast< unsigned char* >( inData ), 
		inSize );

	//�T�C�Y�����܂Ō���܂���
	cout << "FileSize: " << inSize << " -> " << outSize << endl;

	//���k�������̂�W�J���Ă����ƌ��ɖ߂邩�m���߂悤�B
	char* outData2 = new char[ inSize ]; //�����ł����͂�����ˁH
	int outSize2;
	decode( 
		reinterpret_cast< unsigned char* >( outData2 ), 
		&outSize2, 
		reinterpret_cast< unsigned char* >( outData ), 
		outSize );

	for ( int i = 0; i < inSize; ++i ){
		assert( inData[ i ] == outData2[ i ] );
	}
	assert( inSize == outSize2 );
	cout << "succeeded." << endl;
#ifndef NDEBUG
	while ( true ){ ; }
#endif
}

//���k�����[
/*
�n�t�}�����k�B�ŏ��ɂ�邱�Ƃ͂��ꂼ��̕����̉񐔂𐔂��邱�ƁB
���̌�͖؍\��������΂�B
*/

//�n�t�}���؂̃m�[�h�B�񕪖؂Ȃ̂ō��E�̃|�C���^������A�J�E���g�������A���������B�������Ȃ�����-1�B
class Node{
public:
	Node() : mLeft( 0 ), mRight( 0 ), mCount( 0xffffffff ), mLetter( -1 ){} //�J�E���g�͖����ɂ��Ƃ��B

	//����������Ă����܂��B
	void makeCodeTable( int* lengths, unsigned char* codes, unsigned char* currentCode, int currentCodeLength ){
		//unsigned char���8bit����̂ŁA8�Ŋ����ăo�C�g���m��A�]��ŃV�t�g���m��B
		int index = currentCodeLength / 8;
		int shift = currentCodeLength % 8;
		//�q������΁A�q�Ɋۓ���
		if ( mLeft ){
			//���Ȃ̂�0�����݂̕����ɒǉ�
			currentCode[ index ] &= ~( 1 << shift ); //�r�b�g��0�ɁB
			mLeft->makeCodeTable( lengths, codes, currentCode, currentCodeLength + 1);
		}
		if ( mRight ){
			//�E�Ȃ̂�1�����݂̕����ɒǉ�
			currentCode[ index ] |= ( 1 << shift ); //�r�b�g��1�ɁB
			mRight->makeCodeTable( lengths, codes, currentCode, currentCodeLength + 1 );
		}
		//���������������Ȃ當�����o�͂��˂΂Ȃ�ʁB
		if ( mLetter != -1 ){
			lengths[ mLetter ] = currentCodeLength;
			for ( int i = 0; i < 8; ++i ){
				codes[ mLetter * 8 + i ] = currentCode[ i ];
			}
		}
	}


	Node* mLeft;
	Node* mRight;
	unsigned mCount; //���̐߂̃J�E���g(0xffffffff�����Ă�悤��unsigned)
	int mLetter; //����(-1�������߂�int�B�P�`��Ȃ�short�ɂ��悤)
};


void encode( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	//������
	int counts[ 256 ];
	for ( int i = 0; i < 256; ++i ){
		counts[ i ] = 0;
	}
	for ( int i = 0; i < sizeIn; ++i ){
		++counts[ dataIn[ i ] ];
	}
	//�����I������B�؂����܂��B
	//�؂͂܂�256�̐߂���ׂ邱�Ƃ���n�߂�B
	//�ߔz���p�ӂ��邪�A�߂̐��͍ő傢���ɂȂ邾�낤�H

	//�A�[�@�I�ɍl����B
	//������1�̎��ɂ͐߂�1�B
	//�����ɕ���������₷���ɉ����N���邩�ƌ����΁A�ő�œ�߂�������
	//���E�Ɏ}���o���Ċi�[���邩��B�����255��J��Ԃ��񂾂���A511�̐߂��ő�B�ł�512�p�ӂ��Ă����B
	Node nodes[ 512 ];
	//�܂��ŏ���256�̃m�[�h�����������܂��B
	for ( int i = 0; i < 256; ++i ){
		nodes[ i ].mCount = counts[ i ];
		nodes[ i ].mLetter = i;
	}
	int nodeNumber = 256; //����256�g���Ă܂��B

	//�����A�؂��\�z���悤�I
	//255��񂹂ΑS���̐߂��������܂��B����͏ؖ��ł���̂ł��Ă݂悤�B
	//�v����ɖ���߂���g���킯�ŁA511�ɂȂ������Ɏg���؂�̂�����A�����ŏI���̂��B
	//�����āA�؂̌`���ǂ��ł���A255�̒[�m�[�h�����؂͐�΂�511�̐߂����̂ł���B
	for ( int j = 0; j < 255; ++j ){
		//�܂���邱�ƁB�J�E���g����ԏ������z�ƁA��Ԗڂɏ������z��T���B
		//�֌W�Ȃ��Ȃ����߂̓J�E���g��0xffffffff�ɂȂ��Ă���̂ŃX�L�b�v�����B
		//�Ȃ��A511�Ԗڂ̓_�~�[�B�������ł����J�E���g��������ςȂ��ɂȂ��Ă���̂ŁA�����
		int merged0 = 511; //��ԃJ�E���g����������
		int merged1 = 511; //��ԖڂɃJ�E���g����������
		for ( int i = 0; i < nodeNumber; ++i ){ //���̌����͌������@���g���Ƃ������荂�����ł��邪�A�R�[�h�������Ȃ�̂ł��Ȃ��B
			if ( nodes[ i ].mCount < nodes[ merged1 ].mCount ){
				if ( nodes[ i ].mCount < nodes[ merged0 ].mCount ){
					merged1 = merged0; //2�ʂ����܂ł�1�ʂ�
					merged0 = i; //1�ʂ�����
				}else{
					merged1 = i; //2�ʂ�������
				}
			}
		}
		//1�ʂ�2�ʂ����������߂����B
		Node* newNode = &nodes[ nodeNumber ];
		++nodeNumber;
		newNode->mLeft = &nodes[ merged0 ];
		newNode->mRight = &nodes[ merged1 ];
		newNode->mCount = nodes[ merged0 ].mCount + nodes[ merged1 ].mCount; //�J�E���g�͘a�ɂȂ�
		nodes[ merged0 ].mCount = nodes[ merged1 ].mCount = 0xffffffff; //��������ꂽ�z�̃J�E���g�͂���Ȃ��̂Ŗ����ɖ߂��B
	}
	//�؂��ł����I
	//���̂܂܈��k���J�n���Ă������̂����A
	//�ł���������z��ɓ���Ă�����������̏������y�Ȃ̂ŁA�������Ă����B
	int lengths[ 256 ]; //������
	unsigned char codes[ 256 * 8 ]; //����(�ő�8�o�C�g�ł�����)
	//������������
	for ( int i = 0; i < 256; ++i ){
		for ( int j = 0; j < 8; ++j ){
			codes[ i * 8 + j ] = 0;
		}
	}
	//�������͍Ō�ɍ�����߂ŁA�����510�Ԃƌ��܂��Ă���B510�Ԃ���ċA�ŕ����\����낤�B
	int currentCodeLength = 0;
	unsigned char currentCode[ 8 ];
	for ( int i = 0; i < 8; ++i ){
		currentCode[ i ] = 0;
	}
	nodes[ 510 ].makeCodeTable( lengths, codes, currentCode, currentCodeLength ); //�Ō�̈����͍��̐[��

	//�ł��������\��\�����Ă݂悤
	for ( int i = 0; i < 256; ++i ){
		cout << i << "\t" << lengths[ i ] << "\t";
		for ( int j = 0; j < lengths[ i ]; ++j ){
			cout << ( ( codes[ i * 8 + ( j / 8 ) ] & ( 1 << ( j % 8 ) ) ) >> ( j % 8 ) );
		}
		cout << endl;
	}

	//�����\���ł����I�������݊J�n�B
	OBitStream stream( dataOut ); //�o�͐�o�b�t�@�𓖂Ă�
	
	//�ŏ��Ɍ��̃t�@�C���T�C�Y����������
	stream.write( static_cast< unsigned >( sizeIn ) );

	//�����\�������������B
	for ( int i = 0; i < 256; ++i ){
		unsigned char l = static_cast< unsigned char >( lengths[ i ] );
		stream.write( &l, 8 ); //��������8bit�Œ�
		stream.write( &codes[ i * 8 ], lengths[ i ] ); //�����͉ϒ�
	}
	
	//���k�J�n�I�Ƃ��Ă��P���ɏ�����B�u�����ď������ނ���������B
	for ( int i = 0; i < sizeIn; ++i ){
		int c = dataIn[ i ];
		stream.write( &codes[ c * 8 ], lengths[ c ] );
	}
	*sizeOut = stream.sizeInByte() + 4; //�o�C�g�ŃT�C�Y��������āA�擪�̕�������4�𑫂��B
}

//���͔n�������ɏ����ƓW�J�̕����d���B
//1�r�b�g�ǂޓx�ɕ����\�ɒT���Ă�����̂��Ȃ������ׂ�H�ڂɂȂ邩�炾�B
void decode( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	IBitStream stream( dataIn );
	//�܂�32bit���o���B���ꂪ�t�@�C���T�C�Y�ł��B
	int origSize = stream.readS4();

	//�����\�����܂��B
	int lengths[ 256 ]; //������
	unsigned char codes[ 256 * 8 ]; //����(�ő�8�o�C�g�ł�����)
	for ( int i = 0; i < 256 * 8; ++i ){
		codes[ i ] = 0; //�R�[�h0������
	}
	for ( int i = 0; i < 256; ++i ){
		unsigned char buf[ 8 ];
		stream.read( buf, 8 ); //������
		lengths[ i ] = buf[ 0 ];
		stream.read( &codes[ i * 8 ], lengths[ i ] ); //�����ǂݍ���
	}
	//�ǂ񂾕����\��\�����Ă݂悤
	for ( int i = 0; i < 256; ++i ){
		cout << i << "\t" << lengths[ i ] << "\t";
		for ( int j = 0; j < lengths[ i ]; ++j ){
			cout << ( ( codes[ i * 8 + ( j / 8 ) ] & ( 1 << ( j % 8 ) ) ) >> ( j % 8 ) );
		}
		cout << endl;
	}

	//�����A���g��ǂݍ������B
	int pos = 0; //�������݈ʒu
	for ( int i = 0; i < origSize; ++i ){
		//0�����������̈������āA
		unsigned char code[ 8 ];
		for ( int j = 0; j < 8; ++j ){
			code[ j ] = 0;
		}
		//1�r�b�g�ǂ�ł͕����\�Ɣ�ׂă}�b�`���Ă��邩���ׂ�
		bool found = false;
		for ( int j = 0; j < 255; ++j ){ //�ő�255�r�b�g�ł��B
			int index = j / 8;
			int shift = j % 8;
			if ( stream.read() ){ //1bit�ǂ��1�Ȃ�1����������
				code[ index ] |= ( 1 << shift );
			}
			//�����\����T���܂��B
			int length = j + 1;
			for ( int k = 0; k < 256; ++k ){
				if ( lengths[ k ] == length ){ //�����������ŁA
					if ( isEqual( &codes[ k * 8 ], code, length ) ){ //�ꏏ�Ȃ�
						dataOut[ pos ] = static_cast< unsigned char >( k ); //�������o��
//						cout << static_cast< char >( k ); //�f�R�[�h�����������o���Ă݂悤
						++pos;
						found = true;
						break;
					}
				}
			}
			if ( found ){
				break;
			}
		}
		assert( found ); //������Ȃ����Ƃ͂��肦�Ȃ�
	}
	cout << endl;
	assert( pos == origSize ); //�����Ƃ҂�����f�R�[�h�ł������H
	assert( stream.positionInByte() <= sizeIn );
	*sizeOut = pos;
}


//�r�b�g��r�֐�
bool isEqual( unsigned char* a, unsigned char* b, int sizeInBit ){
	//�܂��o�C�g�P�ʂłł��镪�������
	int sizeInByte = sizeInBit / 8;
	for ( int i = 0; i < sizeInByte; ++i ){
		if ( a[ i ] != b[ i ] ){
			return false;
		}
	}
	sizeInBit = sizeInBit % 8; //�]��r�b�g��
	if ( sizeInBit > 0 ){ //�]�肪����΁A�]����ׂ�
		//���̃r�b�g�ȊO��0�ɂ��邽�߂�1�̕��т����B
		unsigned char mask = static_cast< unsigned char >( 0xff >> ( 8 - sizeInBit ) ); //�Ⴆ��6bit���܂��Ă�Ȃ�A00111111����肽���킯���B11111111���V�t�g���č��B
		return ( ( a[ sizeInByte ] & mask ) == ( b[ sizeInByte ] & mask ) );
	}else{ //�]�肪�Ȃ��̂ŁA�����܂ŗ����Ƃ������Ƃ�true���Ƃ������Ƃ��B
		return true;
	}
}
