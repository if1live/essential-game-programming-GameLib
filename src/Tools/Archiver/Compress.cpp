#include "GameLib/GameLib.h"
#include "GameLib/Base/Compressor.h"
#include <fstream>
using namespace std;
using namespace GameLib;

//���k
bool compress(
int* writtenSize, 
int* inplaceDecompressionBufferSize,
bool* compressed,
ofstream* oStream, 
ifstream* iStream,
double threshold ){
	//�{���͕����ǂ݂��Ȃ��Ƃ܂������ʓ|�Ȃ̂ňꊇ
	iStream->seekg( 0, ifstream::end );
	int inSize = static_cast< int >( iStream->tellg() );
	iStream->seekg( 0, ifstream::beg );
	Array< char > inData( inSize + 1 ); //0�o�C�g�ł������悤�ɃC���`�L
	iStream->read( &inData[ 0 ], inSize );

	//���Ⴀ���k�����[
	Array< char > outData;
	Compressor::compress( 
		&outData,
		inplaceDecompressionBufferSize,
		&inData[ 0 ],
		inSize );
	int outSize = outData.size();
//#ifndef NDEBUG //���k�������̂�W�J���Ă����ƌ��ɖ߂邩�m���߂悤�B
	if ( outSize > 0 ){
		{
			Array< char > decompressed( inSize );
			int decompressedSize;
			Compressor::decompress( 
				&decompressed[ 0 ],
				&decompressedSize,
				&outData[ 0 ],
				outSize );
			STRONG_ASSERT( inSize == decompressedSize );
			for ( int i = 0; i < inSize; ++i ){
				STRONG_ASSERT( inData[ i ] == decompressed[ i ] );
			}
		}
		{
			//���̏�W�J�̃e�X�g�B���ɋl�߂�B
			Array< char > decompressed( *inplaceDecompressionBufferSize );
			int offset = *inplaceDecompressionBufferSize - outSize;
			for ( int i = 0; i < outSize; ++i ){
				decompressed[ offset + i ] = outData[ i ];
			}
			int decompressedSize;
			Compressor::decompress( 
				&decompressed[ 0 ],
				&decompressedSize,
				&decompressed[ 0 ] + offset,
				outSize );
			STRONG_ASSERT( inSize == decompressedSize );
			for ( int i = 0; i < inSize; ++i ){
				STRONG_ASSERT( inData[ i ] == decompressed[ i ] );
			}
		}
	}
//#endif
	//��������
	double t = inSize;
	t *= threshold;
	t /= 100.0;
	if ( outSize < t ){ //���k�o�[�W��������������
		oStream->write( &outData[ 0 ], outSize );
		*writtenSize = outSize;
		*compressed = true;
	}else{
		oStream->write( &inData[ 0 ], inSize );
		*writtenSize = inSize;
		*inplaceDecompressionBufferSize = inSize;
		*compressed = false;
	}
	return true;
}

