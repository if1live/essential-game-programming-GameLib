#include "GameLib/GameLib.h"
#include "GameLib/Base/Compressor.h"

#define OPTIMIZATION //���ꂪON���ƍ������ŁBOFF���Ɣn�������ŁB

namespace GameLib{

namespace {

//�r�b�g���蓖�Ē萔
const int DIC_BITS = 11; //�ʒubit��
//�ȉ������v�Z�萔
const int LENGTH_BITS = 16 - 1 - DIC_BITS; //����
const int DIC_MASK = ( 1 << DIC_BITS ) - 1;
const int DIC_MASK_HIGH = DIC_MASK & 0xffffff00; //��8bit���Ԃ�������
const int DIC_MASK_SHIFTED = ( DIC_MASK >> 8 ) << LENGTH_BITS; 
const int LENGTH_MASK = ( 1 << LENGTH_BITS ) - 1;
const int DIC_SIZE = DIC_MASK + 1; //�����T�C�Y(1�����Ċi�[����֌W��1�����g����)
const int MAX_LENGTH = LENGTH_MASK + 3; //�ő��v��(3�����Ċi�[����֌W��3�����g����)

//�悭�g���ŏ��l�ƍő�l
inline int min( int a, int b ){
	return ( a < b ) ? a : b;
}

inline int max( int a, int b ){
	return ( a > b ) ? a : b;
}

#ifdef OPTIMIZATION
/*
��������@�́A�����Ɉ��������肻�����Ȃ����炢�P���Ȃ����݂ɗ}�������A����ł����\�����B
�����ɂ��āA�ua�͂����v�ub�͂����v�Ƃ����悤�ȕ\�����B
0-255�܂ł�256��ނ̕����ɂ��āA���ꂼ��o�������X�g������Ă����B
�T��������abc�Ȃ�a����n�܂�̂ŁAa������ꏊ�����X�g����T���āA
�������猟�����n�߂�Ba�łȂ��ꏊ�͖������邽�߁A�������U��΂��Ă���Α��������Ȃ�B

����Node,Index�N���X�͂��̂��߂̕\��\������N���X�ŁA
���X�g�̊e�G���g����Node�ANode���Ǘ�����\�S�̂�Index�N���X�ƂȂ�B
Node�������Ă���mPos���A���̕���������ꏊ��\���B�ꏊ�̓t�@�C���S�̂̒��ł̈ʒu�ŁA
�������ł̈ʒu�ł͂Ȃ��B
*/

class Node{
public:
	int mNext;
	int mPrev;
	int mPos; //�t�@�C�����̈ʒu
};

class Index{ //DIC_SIZE�Ԃ܂ł��g�p���m�[�h�B���̌��256�̓_�~�[�w�b�h�BDIC_SIZE+c�Ƃ����̂́Ac�Ƃ��������̃��X�g�̃w�b�h�A�̈Ӗ��B
public:
	Index(){
		//�_�~�[�w�b�h�̏������Bnext,prev�͎������w���悤�ɏ�����
		for ( int i = DIC_SIZE; i < ( DIC_SIZE + 256 ); ++i ){
			mNodes[ i ].mNext = mNodes[ i ].mPrev = i;
		}
		//�󂢂Ă���ꏊ���L�^�����X�^�b�N�̏������BDIC_SIZE�܂ŁB
		for ( int i = 0; i < DIC_SIZE; ++i ){
			mStack[ i ] = i;
		}
		mStackPos = DIC_SIZE;
	}
	int getFirst( unsigned char c ) const {
		return mNodes[ DIC_SIZE + c ].mNext;
	}
	const Node* getNode( int i ) const {
		return &mNodes[ i ];
	}
	//�擪�ɑ����B�V�������̂قǑ��������ΏۂɂȂ�B
	void add( unsigned char c, int pos ){
		--mStackPos;
		int newIdx = mStack[ mStackPos ];
		Node* newNode = mNodes + newIdx;
		Node* head = &mNodes[ DIC_SIZE + c ];
		newNode->mNext = head->mNext;
		newNode->mPrev = DIC_SIZE + c;
		newNode->mPos = pos;
		mNodes[ head->mNext ].mPrev = newIdx;
		head->mNext = newIdx;
	}
	//��������T���ď����B�����͈̂�ԍŏ���add�������̂Ȃ̂ŁA��΂ɖ����ɂ���B
	//�܂�A���X�g�����������񂷕K�v�͂Ȃ��B
	void remove( unsigned char c, int pos ){
		int idx = mNodes[ DIC_SIZE + c ].mPrev;
		Node* n = &mNodes[ idx ];
		ASSERT( n->mPos == pos ); //��΂ɂ����ɂ���͂��ŁA���̏�������������Ȃ���΃o�O���B
		mStack[ mStackPos ] = mNodes[ n->mPrev ].mNext; //�폜
		++mStackPos;
		mNodes[ n->mPrev ].mNext = n->mNext;
		mNodes[ n->mNext ].mPrev = n->mPrev;
	}
	bool isEnd( int idx ) const { //�_�~�[�w�b�h���ǂ�����Ԃ��BDIC_SIZE�ȏ�Ȃ�_�~�[�B
		return ( idx >= DIC_SIZE );
	}
private:
	Node mNodes[ DIC_SIZE + 256 ];
	int mStack[ DIC_SIZE ];
	int mStackPos;
};

#endif //OPTIMIZATION

} //namespace{}

/*
�������k�BLZ77�B

���k�̈��2�o�C�g�ŁA�ʒu�ƒ����ɕ��z���Ă���B
1�o�C�g�� : 0x80+�T�C�Y+�ʒu�̏�ʃr�b�g�A
2�o�C�g�� : �ʒu�̉���8bit�B
�񈳏k�̈�́A�񈳏k�̈�T�C�Y�A�񈳏k������(1����128)����

���k�̈�́A�ʒu�A��������-3���������i�[���Ă���B
�W�J���͎��o���Ă���3�𑫂��B
�񈳏k�̈�͒�����-1���Ă���A���o�����ɂ�+1����B
*/

void Compressor::compress(
Array< char >* oDataSigned,
int* inPlaceDecompressionBufferSize,
const char* iDataSigned,
int iSize ){
	int worstSize = iSize + ( iSize / 128 ) + 1; //�ň��T�C�Y�v�Z�B���ꂾ���m�ہB+1�͏����_�ȉ��؂�グ���B
	oDataSigned->setSize( worstSize );

	unsigned char* oData = reinterpret_cast< unsigned char* >( &( *oDataSigned )[ 0 ] );
	const unsigned char* iData = reinterpret_cast< const unsigned char* >( iDataSigned );

	int oPos = 0; //�������ݑ��̏������ވʒu
	int i = 0;
	int unmatchBegin = 0; //���v�̈�̊J�n�ʒu
#ifdef OPTIMIZATION
	Index index;
#endif
	while ( i < iSize ){
		//�������猟��
		int matchLength = 0;
		int matchPos = 0;
		//�����̐擪����T���Ă����Bj��i���z���Ȃ��B
		//�ő匟����
		int maxL = min( MAX_LENGTH, iSize - i ); //�t�@�C����������͌����ł��Ȃ��̂ŁAmaxL�𐧌�����B
#ifdef OPTIMIZATION
		//�����J�n �ꕶ���ڂ�T��
		int idx = index.getFirst( iData[ i ] );
		while ( !index.isEnd( idx ) ){
			const Node* n = index.getNode( idx );
			int p = n->mPos;
			//��v���𒲂ׂ�
			int l = 1; //1������v��Ԃ���n�߂�B
			while ( l < maxL ){
				//���̕������}�b�`���Ȃ���ΏI���
				if ( iData[ p + l ] != iData[ i + l ] ){
					break;
				}
				++l; //1��������
			}
			//�O��蒷����v�����Ȃ�L�^�B�����ȃ}�b�`�̎d��������͂�������A�ő�̂��̂��L�^����B
			if ( matchLength < l ){
				matchPos = p;
				matchLength = l;
				if ( matchLength == maxL ){ //��v�����ő�ɂȂ����炻���ŏI���B
					break;
				}
			}
			idx = n->mNext;
		}
		//���āA��v��3�����ȏ゠��Έ��k���[�h�ŋL�^����B
		if ( matchLength >= 3 ){
			//�����X�V�B�����āA�����B�i�񂾕����������폜���āA�i�񂾕��������������B
			for ( int j = 0; j < matchLength; ++j ){
				int delPos = i + j - DIC_SIZE;
				if ( delPos >= 0 ){
					index.remove( iData[ delPos ], delPos );
				}
				index.add( iData[ i + j ], i + j );
			}
			//�񈳏k�w�b�_��������
			if ( unmatchBegin < i ){
				oData[ oPos ] = static_cast< unsigned char >( i - unmatchBegin - 1 ); //�Œ�1�Ȃ̂�1�����ĕۑ�
				++oPos;
				for ( int j = unmatchBegin; j < i; ++j ){
					oData[ oPos ] = iData[ j ];
					++oPos;
				}
			}
			//���k�������L�^
			int wl = matchLength - 3; //3�����Ċi�[
			int wp = i - matchPos - 1; //1�����Ċi�[
			int tmp = 0x80 | wl; //�����Ɉ��k�t���O��ǉ�
			tmp |= ( wp & DIC_MASK_HIGH ) >> ( 8 - LENGTH_BITS ); //mask��&���A������T�C�Y�Ɏg���Ă���r�b�g�̕��������炷�B
			oData[ oPos + 0 ] = static_cast< unsigned char >( tmp );
			oData[ oPos + 1 ] = static_cast< unsigned char >( wp & 0xff );
			oPos += 2;
			i += matchLength;
			unmatchBegin = i; //���v�ʒu�͎�����
		}else{ //�}�b�`���Ȃ������B�������݂͂܂Ƃ߂Ă��̂ŁA���͐i�߂�B
			//�����X�V�B�����āA�����B
			int delPos = i - DIC_SIZE;
			if ( delPos >= 0 ){
				index.remove( iData[ delPos ], delPos );
			}
			index.add( iData[ i ], i );
			++i;
			if ( i - unmatchBegin == 128 ){ //���E�����܂��Ă��܂����B��������
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
#else //�n��������
		//�����̐擪�B
		int dicBegin = max( i - DIC_SIZE, 0 ); //0���O�ɂ͂Ȃ�Ȃ����Ƃɒ��ӁB������max()���g��
		for ( int j = dicBegin; j < i; ++j ){ //�Ȃ��A���̃��[�v�̒��g���v�Z�̑唼���߂Ă���B���������������邱�Ƃ��K�v�ɂȂ邪�A���\��ς��Ǝv���B
			//��v���𒲂ׂ�
			int l = 0;
			while ( l < maxL ){ //j<i�ŁAi+l<sizeIn�B����āAj+l<sizeIn�ŁA�͈͓��ɓ���B������j+l>=i�͂��肤��B�܂�A�������͂ݏo���Č������邱�Ƃ͂��肤��B�������A����ł������������̂��B�}��`���Ē��ׂ悤�B
				//���̕������}�b�`���Ȃ���ΏI���
				if ( iData[ j + l ] != iData[ i + l ] ){
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
		//���āA��v��3�����ȏ゠��Έ��k���[�h�ŋL�^����B
		if ( matchLength >= 3 ){
			if ( unmatchBegin < i ){
				//�񈳏k�w�b�_��������
				oData[ oPos ] = static_cast< unsigned char >( i - unmatchBegin - 1 ); //�Œ�1�Ȃ̂�1�����ĕۑ�
				++oPos;
				for ( int j = unmatchBegin; j < i; ++j ){
					oData[ oPos ] = iData[ j ];
					++oPos;
				}
			}
			//���k�������L�^
			int wl = matchLength - 3; //3�����Ċi�[
			int wp = i - matchPos - 1; //1�����Ċi�[
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
#endif //OPTIMIZATION
	}

	//���v�ʒu���c���Ă���΍Ō�̏�������
	if ( unmatchBegin < i ){
		//�񈳏k�w�b�_��������
		oData[ oPos ] = static_cast< unsigned char >( i - unmatchBegin - 1 ); //�Œ�1�Ȃ̂�1�����ĕۑ�
		++oPos;
		for ( int j = unmatchBegin; j < i; ++j ){
			oData[ oPos ] = iData[ j ];
			++oPos;
		}
	}
	//���̏�W�J�p�ǂ������ʌv�Z
	if ( inPlaceDecompressionBufferSize ){
		//���z�I�ɓW�J���Ē��ׂ�B�����Ƃ����肪�����񂾂���ւ���
		int wp = 0; //�������݃|�C���^
		int diff = 0; //�������݃|�C���^-�ǂݍ��݃|�C���^
		int i = 0;
		int bufferSize = max( iSize, oPos ); //�L�тĂ��܂�������oPos���g���B
		while ( i < oPos ){
			int length;
			if ( oData[ i ] & 0x80 ){ //���k���[�h
				length = oData[ i ] & LENGTH_MASK;
				length += 3; //3�������Ȃ��L�^����Ă���B
				i += 2;
			}else{ //�񈳏k���[�h
				length = oData[ i ] + 1; //1�������Ȃ��L�^���Ă���
				i += length + 1;
			}
			wp += length;
			diff = max( diff, wp - ( i + bufferSize - oPos ) );
		}
		*inPlaceDecompressionBufferSize = bufferSize + diff;
	}
	oDataSigned->setSize( oPos ); //���܂��Ă���͂��Ȃ̂ŏk���B
}

//�W�J�͂Ƃ��Ă��ȒP�ł��B
void Compressor::decompress( 
char* oDataSigned,
int* oSize,
const char* iDataSigned,
int iSize ){
	unsigned char* oData = reinterpret_cast< unsigned char* >( oDataSigned );
	const unsigned char* iData = reinterpret_cast< const unsigned char* >( iDataSigned );

	int oPos = 0;
	for ( int i = 0; i < iSize; ++i ){
		int length;
		if ( iData[ i ] & 0x80 ){ //���k���[�h
			length = iData[ i ] & LENGTH_MASK;
			length += 3; //3�������Ȃ��L�^����Ă���B
			int position = ( ( iData[ i ] & DIC_MASK_SHIFTED ) << ( 8 - LENGTH_BITS ) ) | iData[ i + 1 ]; //���G�Ȃ̂ł悭�������čl���悤
			position += 1; //1�������Ȃ��L�^����Ă���B
			for ( int j = 0; j < length; ++j ){
				oData[ oPos + j ] = oData[ oPos - position + j ]; //out����ڂ��̂��C����������������Ȃ����A���łɏ�����������out�͎����Ȃ̂ł���B
			}
			i += 1; //1�o�C�g�]���ɐi�߂Ă��B
		}else{ //�񈳏k���[�h
			length = iData[ i ] + 1; //1�������Ȃ��L�^���Ă���
			for ( int j = 0; j < length; ++j ){
				oData[ oPos + j ] = iData[ i + 1 + j ];
			}
			i += length; //�ق����Ă����Ă�1�͑������B���f�[�^length�������i�߁A���̑O��1�o�C�g�͎��R�ɂ܂����悤
		}
		oPos += length;
	}
	*oSize = oPos;
}

} //namespace GameLib