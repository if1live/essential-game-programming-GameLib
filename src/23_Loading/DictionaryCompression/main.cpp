#include <fstream>
#include <iostream>
#include <cassert> //�W����assert���g�����߁BGameLib����ASSERT�Ǝg�����͓����B
using namespace std;

void compress( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn );
void decompress( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn );

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
	//���̖{�Ő������鎫�����k�͍ň��ł�127�������Ƃ�
	//�u���ꂩ��127�������k���܂���v�Ƃ������邵���������Ȃ̂ŁA
	//�t�@�C���T�C�Y��128/127�ɂ܂ł��������Ȃ��B
	//�ň��P�[�X��������ł��̗e�ʂ��m�ۂ��Ă��܂��B
	//�������AinSize * 128 / 127�Ɣn�������Ɍv�Z����ƁA*128��int�̌��E�𒴂����˂Ȃ��B
	//�����ŁAinSize/127�𑫂����Ƃŏ�������B�܂��A���܂�̕�+1���邱�Ƃ��Y�ꂸ�ɁB
	//�Ȃ��A���삪�����������͖{���ɂ�����z���ď������܂Ȃ����`�F�b�N���邱�ƁB���̂��߂�compress()�ɂ�outMaxSize���n�������������B
	int outMaxSize = inSize + ( inSize / 127 ) + 1; //���܂�̕�+1��Y�ꂸ�ɁB
	char* outData = new char[ outMaxSize ];

	//���Ⴀ���k�����[
	int outSize;
	compress( 
		reinterpret_cast< unsigned char* >( outData ), 
		&outSize, 
		reinterpret_cast< unsigned char* >( inData ), 
		inSize );

	//�T�C�Y�����܂Ō���܂���
	cout << "FileSize: " << inSize << " -> " << outSize << endl;

	//���k�������̂�W�J���Ă����ƌ��ɖ߂邩�m���߂悤�B
	char* outData2 = new char[ inSize ]; //�����ł����͂�����ˁH
	int outSize2;
	decompress( 
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

//�悭�g���ő�ŏ�
int min( int a, int b ){
	return ( a < b ) ? a : b;
}

int max( int a, int b ){
	return ( a > b ) ? a : b;
}

void compress( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	int oPos = 0; //�������ݑ��̏������ވʒu
	int i = 0;
	int unmatchBegin = 0; //���v�̈�̊J�n�ʒu
	while ( i < sizeIn ){
		//�������猟��
		int matchLength = 0;
		int matchPos = 0;
		//�����̐擪����T���Ă����Bj��i���z���Ȃ��B
		//�����̐擪�B
		int dicBegin = max( i - 255, 0 ); //0���O�ɂ͂Ȃ�Ȃ����Ƃɒ��ӁB������max()���g��
		//�ő匟����
		int maxL = min( 127, sizeIn - i ); //�t�@�C����������͌����ł��Ȃ��̂ŁAmaxL�𐧌�����B
		for ( int j = dicBegin; j < i; ++j ){ //�Ȃ��A���̃��[�v�̒��g���v�Z�̑唼���߂Ă���B���������������邱�Ƃ��K�v�ɂȂ邪�A���\��ς��Ǝv���B
			//��v���𒲂ׂ�
			int l = 0;
			while ( l < maxL ){ //j<i�ŁAi+l<sizeIn�B����āAj+l<sizeIn�ŁA�͈͓��ɓ���B������j+l>=i�͂��肤��B�܂�A�������͂ݏo���Č������邱�Ƃ͂��肤��B�������A����ł������������̂��B�}��`���Ē��ׂ悤�B
				//���̕������}�b�`���Ȃ���ΏI���
				if ( dataIn[ j + l ] != dataIn[ i + l ] ){
					break;
				}
				++l; //1��������
			}
			//�O��蒷����v�����Ȃ�L�^�B�����ȃ}�b�`�̎d��������͂�������A�ő�̂��̂��L�^����B
			if ( matchLength < l ){
				matchPos = j;
				matchLength = l;
				if ( matchLength == maxL ){ //��v�����ő�ɂȂ����炻���ŏI���B
					break;
				}
			}
		}
		//��v��3�����ȏ゠��Έ��k���[�h�ŋL�^����B
		if ( matchLength >= 3 ){
			if ( unmatchBegin < i ){
				//�񈳏k�w�b�_��������
				dataOut[ oPos ] = static_cast< unsigned char >( i - unmatchBegin );
				++oPos;
				for ( int j = unmatchBegin; j < i; ++j ){
					dataOut[ oPos ] = dataIn[ j ];
					++oPos;
				}
			}
			//���k�������L�^
			dataOut[ oPos + 0 ] = static_cast< unsigned char >( 0x80 + matchLength ); //����
			dataOut[ oPos + 1 ] = static_cast< unsigned char >( i - matchPos ); //�ꏊ
			oPos += 2;
			i += matchLength;
			unmatchBegin = i; //���v�ʒu�͎�����
		}else{ //�}�b�`���Ȃ������B�������݂͂܂Ƃ߂Ă��̂ŁA���͐i�߂邾��
			++i;
			if ( i - unmatchBegin == 127 ){ //���E�����܂��Ă��܂����B��������
				//�񈳏k�w�b�_��������
				dataOut[ oPos ] = static_cast< unsigned char >( i - unmatchBegin );
				++oPos;
				for ( int j = unmatchBegin; j < i; ++j ){
					dataOut[ oPos ] = dataIn[ j ];
					++oPos;
				}
				unmatchBegin = i;
			}
		}
	}

	//���v�ʒu���c���Ă���΍Ō�̏�������
	if ( unmatchBegin < i ){
		//�񈳏k�w�b�_��������
		dataOut[ oPos ] = static_cast< unsigned char >( i - unmatchBegin );
		++oPos;
		for ( int j = unmatchBegin; j < i; ++j ){
			dataOut[ oPos ] = dataIn[ j ];
			++oPos;
		}
	}
	*sizeOut = oPos; //�T�C�Y��������
}

//�W�J�͂Ƃ��Ă��ȒP�ł��B
//�ł��G���[�`�F�b�N�͂��ׂ��B�r���ŉ�ꂽ�t�@�C����ǂ񂾎��ɂ�����ƔF���ł���悤�ɂ��Ȃ��Ǝ��p�ɂ͂Ȃ�Ȃ��B
void decompress( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	int outPos = 0;
	for ( int i = 0; i < sizeIn; ++i ){
		int length;
		if ( dataIn[ i ] & 0x80 ){ //���k���[�h
			length = dataIn[ i ] - 0x80;
			int position = dataIn[ i + 1 ];
			for ( int j = 0; j < length; ++j ){
				dataOut[ outPos + j ] = dataOut[ outPos - position + j ]; //out����ڂ��̂��C����������������Ȃ����A���łɏ�����������out�͎����Ȃ̂ł���B
			}
			i += 1; //1�o�C�g�]���ɐi�߂Ă��B
		}else{ //�񈳏k���[�h
			length = dataIn[ i ];
			for ( int j = 0; j < length; ++j ){
				dataOut[ outPos + j ] = dataIn[ i + 1 + j ];
			}
			i += length; //�ق����Ă����Ă�1�͑������B���f�[�^rawLength�������i�߁A���̑O��1�o�C�g�͎��R�ɂ܂����悤
		}
		outPos += length;
	}
	*sizeOut = outPos;
}
