//�ŋߗ��s��RangeCoder�B�������A�����I�ɑ��v���ǂ����͐r���������̂Œ��ӁB�s���Ȃ�n�t�}�����g�����B

//�M���M���܂ŃR�[�h��P���ɂ��邽�߂ɁA���x�͊��S�ɓx�O�����Ă���B���ɒx���B
//�ǂ�����Α����Ȃ邩�l���Ă݂�Ƃ������낤�B

#include <fstream>
#include <iostream>
#include <cassert> //�W����assert���g�����߁BGameLib����ASSERT�Ǝg�����͓����B
#include "BitStream.h" //�r�b�g�P�ʂ̓ǂݏ��������邽�߂̃N���X
using namespace std;

//��̕��̐����g�������̂�unsigned�𑽗p����B
typedef unsigned U4; //4�o�C�g��Unsigned�̈Ӗ��ŁAU4

//���̐��x�B������傫�ȃt�@�C�������k����ɂ͍H�v���K�v�ɂȂ�B
const U4 RANGE_MAX = 0xffffffff;

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
	//RangeCoder�̍ň��P�[�X�́A�S��8bit�����ɂȂ��āA�����ɕp�x�\1024�o�C�g�ƌ��T�C�Y4�o�C�g����������ꍇ�B
	int outMaxSize = inSize + 256 * 4 + 4;
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
RangeCoder�ɂ�鈳�k�B�R�����g�͑�ʂɏ������A�R�[�h����₱��������H�v�͈�ؓ���Ȃ��B
�������A�Œ���Z�p�����̗������킩���Ă��Ȃ��Ƃǂ��ɂ��Ȃ�Ȃ��Ǝv���B
������{��������΂����Ȃ��Ǝv���Ă͂��邪�B
*/

void encode( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	OBitStream stream( dataOut ); //�o�͐�o�b�t�@�B���̓r�b�g�P�ʂœf���K�v�͂Ȃ��񂾂��A�������킩��₷���悤�ɂ������Ă݂�B
	U4 n = sizeIn; //�������B�ǂ��g������ꕶ���ŕʖ�

	//�ŏ��Ɍ��̃t�@�C���T�C�Y����������
	stream.write( n );

	//�܂������̐��𐔂���
	U4 range[ 256 ];
	for ( U4 i = 0; i < 256; ++i ){
		range[ i ] = 0;
	}
	for ( U4 i = 0; i < n; ++i ){
		++range[ dataIn[ i ] ];
	}
	//�����I������B����͓W�J�̎��Ɏg���̂őS���������ށB
	for ( int i = 0; i < 256; ++i ){
		stream.write( range[ i ] );
	}
	//���x�́A�������n�_�Ɣ͈͂ɕϊ�����B
	U4 begin[ 256 ];
	begin[ 0 ] = 0;
	for ( U4 i = 1; i < 256; ++i ){
		begin[ i ] = begin[ i - 1 ] + range[ i - 1 ];
	}
	//�����A�����W�R�[�_���n�߂��I
	U4 b = 0; //�n�_begin
	U4 r = RANGE_MAX; //��range ���܂�Ɏg�p�񐔂������̂ňꕶ���B���߂�B
	for ( U4 i = 0; i < n; ++i ){
		U4 c = dataIn[ i ]; //����c
		//��/�f�[�^�ʂ��v�Z�Bunit(�P��)��u�Ƃ��悤�B
		U4 u = r / n;
		//�n�_���X�V
		U4 add = u * begin[ c ];
		if ( b > ( b + add ) ){ //�J��オ�邼�I���̃C���M�����[�ւ̑Ώ��͑����d�v�B���̕K�v���������Ō��͓̂���B
			stream.add(); //�J��オ��B�Ō�̌���1�𑫂��B
		}
		b += u * begin[ c ];
		//�����X�V
		r = u * range[ c ];
		//�ŁA����0x40000000��菬�����Ȃ�悤�Ȃ�1�r�b�g�V�t�g���Ă��B
		while ( r <= RANGE_MAX/2 ){
			//��ԏ�̃r�b�g���o�͂���B�n�_b���o�͂Ȃ̂ł���B
			stream.write( ( b >= 0x80000000 ) ? true : false );
			//���Ǝn�_�𗼕��{�B
			r <<= 1;
			b <<= 1;
		}
	}
	//�c����32bit���o��
	for ( int i = 0; i < 32; ++i ){
		stream.write( ( b >= 0x80000000 ) ? true : false );
		b <<= 1;
	}

	*sizeOut = stream.sizeInByte() + 4; //�o�C�g�ŃT�C�Y��������āA�擪�̕�������4�𑫂��B
}

//�W�J�̕����d���B
//�͈͂̌��������邩�炾�B�ł��񕪌��������邱�Ƃ�10�{���炢�͂��������Ȃ�B
void decode( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	IBitStream stream( dataIn );
	//�܂�32bit���o���B���ꂪ�t�@�C���T�C�Y�ł��B
	U4 n = stream.readU4();

	//�e�����̕p�x�����o��
	U4 range[ 256 ];
	for ( U4 i = 0; i < 256; ++i ){
		range[ i ] = stream.readU4();
	}
	//���x�́A�������n�_�Ɣ͈͂ɕϊ�����B
	U4 begin[ 256 ];
	begin[ 0 ] = 0;
	for ( U4 i = 1; i < 256; ++i ){
		begin[ i ] = begin[ i - 1 ] + range[ i - 1 ];
	}
	//�����A�W�J���n�߂��I
	U4 b = 0;
	//�n�_�͍ŏ���32�r�b�g��ǂݏo��
	for ( int i = 0; i < 32; ++i ){
		b <<= 1;
		b |= ( stream.read() ) ? 1 : 0;
	}
	U4 r = RANGE_MAX; //��range ���܂�Ɏg�p�񐔂������̂ňꕶ���B���߂�B
	U4 pos = 0; //�������݈ʒu
	for ( U4 i = 0; i < n; ++i ){
		//��/�f�[�^�ʂ��v�Z�Bunit(�P��)��u�Ƃ��悤�B
		U4 u = r / n;
		//�܂����̂͌����B���݂�b���ǂ��ɑ�������̂��𒲂ׂ�B
		int c = -1; //����
		for ( U4 j = 0; j < 256; ++j ){
			if ( b < ( ( begin[ j ] + range[ j ] ) * u ) ){ //�n�_+����菬������΂��̗̈�ɂ���B
				c = j; //����
				break;
			}
		}
		assert( c != -1 ); //�����������Ă���͂������H
		//��������������c���I
		dataOut[ pos ] = static_cast< unsigned char >( c );
//		cout << static_cast< char >( c ); //�o�Ă��������������Ă݂悤�B
		++pos;
		//�n�_���炱�̏ꏊ�������B
		b -= u * begin[ c ];
		//�����X�V
		r = u * range[ c ];
		//�ŁA����0x40000000��菬�����Ȃ�悤�Ȃ�1�r�b�g�V�t�g���Ă��B
		while ( r <= RANGE_MAX/2 ){
			//���Ǝn�_�𗼕��{�B
			r <<= 1;
			b <<= 1;
			//b�̈�ԉ��̃r�b�g�ɓ��͂�����Ă��ē����B
			b |= ( stream.read() ) ? 1 : 0;
		}
	}
//	cout << endl;
	assert( pos == n ); //�����Ƃ҂�����f�R�[�h�ł������H
	assert( stream.positionInByte() <= sizeIn ); //�ǂ݂����ĂȂ��H
	*sizeOut = pos;
}
