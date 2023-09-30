#include <fstream>
#include <iostream>
#include <cassert> //�W����assert���g������
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
	//���̖{�Ő�������A�����k�͍ň��ł�127�������Ƃ�
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

	assert( inSize == outSize2 );
	for ( int i = 0; i < inSize; ++i ){
		assert( inData[ i ] == outData2[ i ] );
	}
	cout << "succeeded." << endl;
#ifndef NDEBUG
	while ( true ){ ; }
#endif
}

//���k�����[
/*
�A�����k�B
���k�̈��0x80+�A���A����������2�o�C�g�B
�񈳏k�̈�́A�񈳏k�̈�T�C�Y(1-127)�A�񈳏k������(1-127����)�B

�������ނ̂͂����H�ȉ���5�̏ꍇ��
1.127�����A�������Ƃ��B0xff�ƘA�����镶�����������ށB
2.127�����A�����Ȃ������Ƃ��B0x7f�ƘA�����Ȃ��̈�127�������������ށB
  ���������̏ꍇ�A�Ō��1�����Ǝ���2�����A���邢�͍Ō��2�����Ǝ���1�������A�����Ă��鋰�ꂪ����B
  �{�T���v���R�[�h�ł͂��̏ꍇ�͖�������127������������ł��܂��B���k���������邪�A�����ł���ɕ��򂳂���ƃR�[�h���Ƃ�ł��Ȃ���₱�����Ȃ邩�炾�B
  �Ȃ�1�����Ȃ���2���������Ɏc����Ă��邩�𒲂ׂ˂΂Ȃ�Ȃ��Ȃ邩��ł���B
3.3�����ȏ�A�����Ă��ĈႤ�����������Ƃ��B�A����������+0x80�ƁA�A�����Ă����������������ށB
4.�A������3�����ڂ��������ɁA���̑O�ɔ�A���̈悪����ꍇ�B�A�����Ă��Ȃ������ƘA�����Ă��Ȃ��̈���������ށB
5.�t�@�C�������ɗ��āA�܂���������ł��Ȃ����������鎞�B��������1�Ȃ���2�̂͂��ł���B��A���Ƃ��ď�������
*/
void compress( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	int runBegin = 0; //�Ȃ���n�߂��ꏊ
	int inPos = 0; //�ǂݍ��ݑ��̎��ɏ������ޏꏊ
	int outPos = 0; //�������ݑ��̏������݈ʒu
	for ( int i = 0; i <= sizeIn; ++i ){ //�Ō���������ނ��߂�+1�܂ŉ񂷁B���\��ǋ�����Ȃ炻�������[�v�̌��ɏ����ׂ������R�[�h�������Ȃ�̂ł��������B
		int runLength = i - runBegin; //�A�����B�Ȃ����Ă鐔�́u��-�J�n�v�B
		int rawLength = i - inPos; //��A�����B�Ȃ����ĂȂ����́u��-�Ō�ɏ������񂾕����̎��v�B���݂̕����͊܂܂Ȃ��B
		bool case5;
		bool match;
		if ( i == sizeIn ){
			match = false;
			case5 = ( rawLength > 0 ); //�Ō�܂ł��ď����ĂȂ�����������Ȃ珑���˂΂Ȃ�ʁB
		}else{
			match = ( dataIn[ i ] == dataIn[ runBegin ] );
			case5 = false;
		}
		bool case1 = ( runLength == 127 ); //�P�[�X1
		bool case2 = ( rawLength == 127 ); //�P�[�X2
		bool case3 = ( runLength >= 3 ) && ( !match ); //�P�[�X3
		bool case4 = ( runLength == 2 ) && match  && ( rawLength > 2 ); //�P�[�X4 �u3�����ڂ������Ƃ��v�Ƃ����̂́u2���������Ă��āA�O�Ɠ��������������v�Ɠǂݑւ�����B�u���̑O�ɔ�A���̈悪����v�́urawLength > 2�v���B3����Ȃ���2�B�悭�l����B
		if ( case1 || case3 ){ //��̃P�[�X1�ƃP�[�X3
			dataOut[ outPos + 0 ] = static_cast< unsigned char >( 0x80 | runLength );
			dataOut[ outPos + 1 ] = dataIn[ runBegin ]; //�Ȃ��镶���������B
			outPos += 2; //2�o�C�g������
			inPos = i; //���͌��݂̈ʒu���珑�����݂܂��B
			runBegin = i; //match���悤�����܂������Z�b�g
		}else if ( case2 || case4 || case5 ){ //��̃P�[�X2,4,5
			if ( case4 ){ //4�̃P�[�X�̏ꍇ�A�������ޕ������͘A����������Ă��B�Ȃ��A2����4�̏ꍇ������B125������A����3�����Ȃ������ꍇ���B�����4���D��ł���B
				rawLength -= 2; //-2,-1,0�ƘA�����Ă���󋵂ɂ���̂ŁA�������ݕ�������2�����ւ炵�Ă��B���̂ւ��₱�����̂Œ��ӁB 
			}
			dataOut[ outPos ] = static_cast< unsigned char >( 0x00 | rawLength ); //�Ȃ����ĂȂ�����
			++outPos;
			for ( int j = 0; j < rawLength; ++j ){ //�񈳏k�̈����������
				dataOut[ outPos + j ] = dataIn[ inPos + j ];
			}
			outPos += rawLength; //���ꂾ���i��
			inPos += rawLength; //���������񂾕���������������B
			if ( case2 ){ //case2�̏ꍇ�A���̒��O��128�����܂ŏ�������ł��܂����̂Ń��Z�b�g���K�v�B����������͎蔲���ł���B
				runBegin = i;
			}
		}
		if ( !match ){ //�A�����ĂȂ������킯������A��������A�����J�n����B
			runBegin = i;
		}
	}
	*sizeOut = outPos;
}


//�W�J�͂Ƃ��Ă��ȒP�ł��B
//�ł��G���[�`�F�b�N�͂��ׂ��B�r���ŉ�ꂽ�t�@�C����ǂ񂾎��ɂ�����ƔF���ł���悤�ɂ��Ȃ��Ǝ��p�ɂ͂Ȃ�Ȃ��B
void decompress( unsigned char* dataOut, int* sizeOut, const unsigned char* dataIn, int sizeIn ){
	int outPos = 0;
	for ( int i = 0; i < sizeIn; ++i ){
		if ( dataIn[ i ] & 0x80 ){ //���k���[�h
			int runLength = dataIn[ i ] - 0x80;
			for ( int j = 0; j < runLength; ++j ){
				dataOut[ outPos + j ] = dataIn[ i + 1 ];
			}
			outPos += runLength;
			i += 1; //1�o�C�g�]���ɐi�߂Ă��B
		}else{ //�񈳏k���[�h
			int rawLength = dataIn[ i ];
			for ( int j = 0; j < rawLength; ++j ){
				dataOut[ outPos + j ] = dataIn[ i + 1 + j ];
			}
			outPos += rawLength;
			i += rawLength; //�ق����Ă����Ă�1�͑������B���f�[�^rawLength�������i�߁A���̑O��1�o�C�g�͎��R�ɂ܂����悤
		}
	}
	*sizeOut = outPos;
}
