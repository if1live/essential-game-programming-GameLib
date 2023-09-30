#ifndef INCLUDED_GAMELIB_COMPRESSOR_H
#define INCLUDED_GAMELIB_COMPRESSOR_H

#include "GameLib/Base/Array.h"

namespace GameLib{

///���k�W�J�N���X
/*
���̏�W�J�����ɂ́Acompress()�ŕԂ��Ă���getInPlaceDecompressionBufferSize
�����o�b�t�@���m�ۂ��A���k�f�[�^�����ɋl�߂Ċi�[���Adecompress���ĂԁB
*/
class Compressor{
public:
	///���k
	static void compress(
		Array< char >* compressed,
		int* inPlaceDecompressionBufferSize, //���̏�W�J���̕K�v�o�b�t�@�T�C�Y
		const char* data,
		int size );
	///�W�J�B�W�J���s��outSize�����������Ń`�F�b�N����BoutSize���傫������ꍇ�́A��������j�󂵂Ă���A�v�����B
	static void decompress(
		char* outData,
		int* outSize,
		const char* data,
		int size );
};

} //namespace GameLib

#endif
