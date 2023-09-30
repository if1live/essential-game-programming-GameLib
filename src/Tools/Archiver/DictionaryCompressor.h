#include <fstream>
#include <iostream>
#include <cassert>
#include "GameLib/Base/Compressor.h"
using namespace std;

class DictionaryCompressor{
public:
	static bool compress( 
		int* outSize, 
		int* necessaryBufferSize,
		bool* compressed,
		ofstream* oStream, 
		ifstream* iStream,
		double threshold );
private:
	static bool compress( 
		unsigned char* outData, 
		int* outSize, 
		int* necessaryBufferSize,
		int outBufferSize,
		const unsigned char* inData, 
		int inSize );
	static bool decompress( 
		unsigned char* outData, 
		int* outSize,
		int outBufferSize,
		const unsigned char* inData, 
		int inSize );
};

bool DictionaryCompressor::compress(
int* writtenSize, 
int* necessaryBufferSize,
bool* compressed,
ofstream* oStream, 
ifstream* iStream,
double threshold ){
	//�{���͕����ǂ݂��Ȃ��Ƃ܂������ʓ|�Ȃ̂ňꊇ
	iStream->seekg( 0, ifstream::end );
	int inSize = static_cast< int >( iStream->tellg() );
	iStream->seekg( 0, ifstream::beg );
	char* inData = new char[ inSize ];
	iStream->read( inData, inSize );

	//�������ݐ�p��
	int outBufferSize = inSize + ( inSize / 127 ) + 1; //���܂�̕�+1��Y�ꂸ�ɁB
	char* outData = new char[ outBufferSize ];
	if ( !outData ){
		cerr << "memory allocation failed : " << outBufferSize << endl;
		return false;
	}
	//���Ⴀ���k�����[
	int outSize;
	bool succeeded = compress( 
		reinterpret_cast< unsigned char* >( outData ), 
		&outSize, 
		necessaryBufferSize,
		outBufferSize,
		reinterpret_cast< unsigned char* >( inData ), 
		inSize );
	assert( succeeded );
#ifndef NDEBUG
	{
		//���k�������̂�W�J���Ă����ƌ��ɖ߂邩�m���߂悤�B
		char* outData2 = new char[ inSize ]; //�����ł����͂�����ˁH
		int outSize2;
		succeeded = decompress( 
			reinterpret_cast< unsigned char* >( outData2 ), 
			&outSize2, 
			inSize,
			reinterpret_cast< unsigned char* >( outData ), 
			outSize );
		assert( succeeded );
		for ( int i = 0; i < inSize; ++i ){
			assert( inData[ i ] == outData2[ i ] );
		}
		assert( inSize == outSize2 );
		delete[] outData2;
		outData2 = 0;
	}
#endif
	//��������
	double t = inSize;
	t *= threshold;
	t /= 100.0;
	if ( outSize < t ){ //���k�o�[�W��������������
		oStream->write( outData, outSize );
		*writtenSize = outSize;
		*compressed = true;
	}else{
		oStream->write( inData, inSize );
		*writtenSize = inSize;
		*necessaryBufferSize = inSize;
		*compressed = false;
	}
	delete[] inData;
	delete[] outData;
	inData = 0;
	outData = 0;
	return true;
}

//�r�b�g���蓖��
const int DIC_BITS = 10; //�ʒubit��
//�ȉ������v�Z�萔
const int LENGTH_BITS = 16 - 1 - DIC_BITS; //����
const int DIC_MASK = ( 1 << DIC_BITS ) - 1;
const int DIC_MASK_HIGH = DIC_MASK & 0xffffff00; //��8bit���Ԃ�������
const int DIC_MASK_SHIFTED = ( DIC_MASK >> 8 ) << LENGTH_BITS; 
const int LENGTH_MASK = ( 1 << LENGTH_BITS ) - 1;
const int DIC_SIZE = DIC_MASK + 3; //�����T�C�Y
const int MAX_LENGTH = LENGTH_MASK + 3; //�ő��v��

//�悭�g���ŏ��l�ƍő�l
inline int min( int a, int b ){
	return ( a < b ) ? a : b;
}

inline int max( int a, int b ){
	return ( a > b ) ? a : b;
}

//���k�����[
/*
�������k�BLZ77�̉����B
���k�̈��2�o�C�g�ŁA�ʒu�ƒ����ɕ��z���Ă���B
1�o�C�g�� : 0x80+�T�C�Y(5bit)+�ʒu�̏�ʃr�b�g(2bit)�A
2�o�C�g�� : �ʒu�̉���8bit�B
�񈳏k�̈�́A�񈳏k�̈�T�C�Y�A�񈳏k������(1����128)����

���k�̈�́A�ʒu�A��������-3���������i�[���Ă���B
�W�J���͎��o���Ă���3�𑫂��B
�񈳏k�̈�͒�����-1���Ă���A���o�����ɂ�+1����B

23�͂̃T���v���ɔ�ׂāA�G���[������A���k������̍H�v�ŃR�[�h�������Ȃ��Ă���B
*/
bool DictionaryCompressor::compress( 
unsigned char* oData, 
int* oSize, 
int* oNecessaryBufferSize,
int oBufferSize, 
const unsigned char* iData, 
int iSize ){
	int oPos = 0; //�������ݑ��̏������ވʒu
	int maxPosDiff = 0; //�W�J���ɁA�������݃|�C���^���ǂݍ��݃|�C���^��ǂ������ʂ̍ő�l�B���̏�W�J���邽�߂ɂ́A���̕������傫�ȓW�J�o�b�t�@���K�v�B
	int i = 0;
	int unmatchBegin = 0; //���v�̈�̊J�n�ʒu
	while ( i < iSize ){
		//�������猟��
		int matchLength = 0;
		int matchPos = 0;
		//�����̐擪�B
		int dicBegin = max( i - DIC_SIZE, 0 ); //0���O�ɂ͂Ȃ�Ȃ����Ƃɒ��ӁB������max()���g��
		//�ő匟����
		int maxL = min( MAX_LENGTH, iSize - dicBegin ); //���͂̍Ō���z���Č����͂ł��Ȃ��B
		//�����̐擪����T���Ă����B�����ŉ���ȍH�v������Ɠ����Ɉ���������̂ł���Ă��Ȃ��B
		int jEnd = i - 3; //�����擪��3�����ȏ�O����n�߂�B3�����ȏ�}�b�`���Ȃ����舳�k���Ȃ����炾�B
		//�����ł͌��̃T���v���ƈ���ă��[�v����d�ɂ��Ă���B�s��v��j��l-1�����߂���肾�B������̕����킩��ɂ������A�Ō�܂Ń}�b�`�������ɂ��������Ă����̂œ���̎�ނ̃t�@�C��(�^�����ȉ摜�Ƃ�)�ő����Ȃ�B
		int j = dicBegin;
		int l = 0;
		while ( ( j < jEnd ) && ( l < maxL ) ){
			if ( iData[ j ] == iData[ i + l ] ){
				++j;
				++l;
			}else{
				//�O��蒷����v�����Ȃ�L�^�B�����ȃ}�b�`�̎d��������͂�������A�ő�̂��̂��L�^����B
				if ( matchLength < l ){
					matchPos = j - l;
					matchLength = l;
				}
				j -= l - 1; //l��0�ł�1�����͐i�ށBl��������Ƃ�������߂�B
				l = 0;
			}
		}
		if ( matchLength < l ){ //�Ō�܂ŗ��Ă��܂����ꍇ�ɑΏ�
			matchPos = j - l;
			matchLength = l;
		}
		//���āA��v��3�����ȏ゠��Έ��k���[�h�ŋL�^����B
		if ( matchLength >= 3 ){
			if ( unmatchBegin < i ){
				if ( ( oPos + i - unmatchBegin + 1 ) > oBufferSize ){ //�������݃o�b�t�@�s���G���[���o �Ō��+1�̓w�b�_��
					return false; //���s
				}
				//�񈳏k�w�b�_��������
				oData[ oPos ] = static_cast< unsigned char >( i - unmatchBegin - 1 ); //�Œ�1�Ȃ̂�1�����ĕۑ�
				++oPos;
				for ( int j = unmatchBegin; j < i; ++j ){
					oData[ oPos ] = iData[ j ];
					++oPos;
				}
			}
			if ( ( oPos + 2 ) > oBufferSize ){ //�������݃o�b�t�@�s���G���[���o
				return false; //���s
			}
			//���k�������L�^
			int wl = matchLength - 3; //3�����Ċi�[
			int wp = i - matchPos - 3; //3�����Ċi�[
			int tmp = 0x80 | wl; //�����Ɉ��k�t���O��ǉ�
			tmp |= ( wp & DIC_MASK_HIGH ) >> ( 8 - LENGTH_BITS ); //mask��&���A������T�C�Y�Ɏg���Ă���r�b�g�̕��������炷�B
			oData[ oPos + 0 ] = static_cast< unsigned char >( tmp );
			oData[ oPos + 1 ] = static_cast< unsigned char >( wp & 0xff );
			oPos += 2;
			i += matchLength;
			unmatchBegin = i; //���v�ʒu�͎�����
		}else{ //�}�b�`���Ȃ������B�������݂͂܂Ƃ߂Ă��̂ŁA���͐i�߂�B
			++i;
			if ( i - unmatchBegin == 128 ){ //���E�����܂��Ă��܂����B��������
				if ( ( oPos + i - unmatchBegin + 1 ) > oBufferSize ){ //�������݃o�b�t�@�s���G���[���o �Ō��+1�̓w�b�_��
					return false; //���s
				}
				//�񈳏k�w�b�_��������
				oData[ oPos ] = static_cast< unsigned char >( i - unmatchBegin - 1 ); //�Œ�1�Ȃ̂�1�����ĕۑ�
				++oPos;
				for ( int j = unmatchBegin; j < i; ++j ){
					oData[ oPos ] = iData[ j ];
					++oPos;
				}
				unmatchBegin = i;
			}
		}
		//�������݈ʒu���ǂ��������ʂ��X�V
		maxPosDiff = max( maxPosDiff, i - oPos );
	}

	//���v�ʒu���c���Ă���΍Ō�̏�������
	if ( unmatchBegin < i ){
		if ( ( oPos + i - unmatchBegin + 1 ) > oBufferSize ){ //�������݃o�b�t�@�s���G���[���o �Ō��+1�̓w�b�_��
			return false; //���s
		}
		//�񈳏k�w�b�_��������
		oData[ oPos ] = static_cast< unsigned char >( i - unmatchBegin - 1 ); //�Œ�1�Ȃ̂�1�����ĕۑ�
		++oPos;
		for ( int j = unmatchBegin; j < i; ++j ){
			oData[ oPos ] = iData[ j ];
			++oPos;
		}
	}

	*oNecessaryBufferSize = max( maxPosDiff + oPos, iSize );
	*oSize = oPos;
	return true;
}

//�W�J�͂Ƃ��Ă��ȒP�ł��B�ł��G���[�`�F�b�N�ł�����ƒ����Ȃ��Ă���B
bool DictionaryCompressor::decompress( 
unsigned char* oData, 
int* oSize,
int oBufferSize,
const unsigned char* iData, 
int iSize ){
	int oPos = 0;
	for ( int i = 0; i < iSize; ++i ){
		int length;
		if ( iData[ i ] & 0x80 ){ //���k���[�h
			length = iData[ i ] & LENGTH_MASK;
			length += 3; //3�������Ȃ��L�^����Ă���B
			int position = ( ( iData[ i ] & DIC_MASK_SHIFTED ) << ( 8 - LENGTH_BITS ) ) | iData[ i + 1 ]; //���G�Ȃ̂ł悭�������čl���悤
			position += 3; //3�������Ȃ��L�^����Ă���B
			if ( oPos + length > oBufferSize ){ //�������݃o�b�t�@�T�C�Y�`�F�b�N
				return false;
			}
			for ( int j = 0; j < length; ++j ){
				oData[ oPos + j ] = oData[ oPos - position + j ]; //out����ڂ��̂��C����������������Ȃ����A���łɏ�����������out�͎����Ȃ̂ł���B
			}
			i += 1; //1�o�C�g�]���ɐi�߂Ă��B
		}else{ //�񈳏k���[�h
			length = iData[ i ] + 1; //1�������Ȃ��L�^���Ă���
			if ( oPos + length > oBufferSize ){ //�������݃o�b�t�@�T�C�Y�`�F�b�N
				return false;
			}
			for ( int j = 0; j < length; ++j ){
				oData[ oPos + j ] = iData[ i + 1 + j ];
			}
			i += length; //�ق����Ă����Ă�1�͑������B���f�[�^length�������i�߁A���̑O��1�o�C�g�͎��R�ɂ܂����悤
		}
		oPos += length;
	}
	*oSize = oPos;
	return true;
}
