#include "Image.h"
#include "File.h"

#include "GameLib/Framework.h"
using namespace GameLib;

Image::Image( const char* filename ) : 
mWidth( 0 ),
mHeight( 0 ),
mData( 0 ){
	File f( filename );
	mHeight = f.getUnsigned( 12 );
	mWidth = f.getUnsigned( 16 );
	mData = new unsigned[ mWidth * mHeight ];
	for ( int i = 0; i < mWidth * mHeight; ++i ){
		mData[ i ] = f.getUnsigned( 128 + i * 4 );
	}
}

Image::~Image(){
	delete[] mData;
	mData = 0;
}

int Image::width() const {
	return mWidth;
}

int Image::height() const {
	return mHeight;
}

//#define USE_FLOAT_VERSION
#define USE_INTEGER_VERSION
//#define USE_OPTIMIZED_VERSION_0
//#define USE_OPTIMIZED_VERSION_1

#ifdef USE_FLOAT_VERSION
//�A���t�@�u�����h�t��(double�o�[�W�����B�x�����m���ɐ��m)
//���������鎞�͂���ƌ��ʂ��ׂ�̂�Y��Ȃ��悤�ɁB
void Image::draw(
int dstX, 
int dstY, 
int srcX, 
int srcY, 
int width, 
int height ) const {
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();
	for ( int y = 0; y < height; ++y ){
		for ( int x = 0; x < width; ++x ){
			unsigned src = mData[ ( y + srcY ) * mWidth + ( x + srcX ) ];
			unsigned* dst = &vram[ ( y + dstY ) * windowWidth + ( x + dstX ) ];
			double srcA = static_cast< double >( ( src & 0xff000000 ) >> 24 ) / 255.f;
			double srcR = static_cast< double >( ( src & 0x00ff0000 ) >> 16 );
			double srcG = static_cast< double >( ( src & 0x0000ff00 ) >> 8 );
			double srcB = static_cast< double >( ( src & 0x000000ff ) >> 0 );

			double dstR = static_cast< double >( ( *dst & 0x00ff0000 ) >> 16 );
			double dstG = static_cast< double >( ( *dst & 0x0000ff00 ) >> 8 );
			double dstB = static_cast< double >( ( *dst & 0x000000ff ) >> 0 );

			double r = ( srcR - dstR ) * srcA + dstR;
			double g = ( srcG - dstG ) * srcA + dstG;
			double b = ( srcB - dstB ) * srcA + dstB;
			*dst = static_cast< unsigned >( r ) << 16;
			*dst |= static_cast< unsigned >( g ) << 8;
			*dst |= static_cast< unsigned >( b );
		}
	}
}
#endif

#ifdef USE_INTEGER_VERSION
//�A���t�@�u�����h�t��
void Image::draw(
int dstX, 
int dstY, 
int srcX, 
int srcY, 
int width, 
int height ) const {
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();
	for ( int y = 0; y < height; ++y ){
		for ( int x = 0; x < width; ++x ){
			unsigned src = mData[ ( y + srcY ) * mWidth + ( x + srcX ) ];
			unsigned* dst = &vram[ ( y + dstY ) * windowWidth + ( x + dstX ) ];
			unsigned srcA = ( src & 0xff000000 ) >> 24;
			unsigned srcR = src & 0xff0000;
			unsigned srcG = src & 0x00ff00;
			unsigned srcB = src & 0x0000ff;
			unsigned dstR = *dst & 0xff0000;
			unsigned dstG = *dst & 0x00ff00;
			unsigned dstB = *dst & 0x0000ff;
			unsigned r = ( srcR - dstR ) * srcA / 255 + dstR;
			unsigned g = ( srcG - dstG ) * srcA / 255 + dstG;
			unsigned b = ( srcB - dstB ) * srcA / 255 + dstB;
			*dst = ( r & 0xff0000 ) | ( g & 0x00ff00 ) | b;
		}
	}
}
#endif

#ifdef USE_OPTIMIZED_VERSION_0
//�A���t�@�u�����h�t��(���񉻔�)
/*
�ƐԂ���C�ɂ��B
�Ԃ�8bit���̌��Ԃ�����̂ŉ����Z�Ə�Z�͈̔͂ł͍�����Ȃ��B
���������Z������Ə�̌��̏��Z�̗]�肪���̌��ɍ�������
���ʂ������B�����ŁA���Z�݂͕̂ʁX�ɍs���B
(A - B) * a/255 + B
���A
((A - B) * a + 255*B)
�ƕό`���A���𕪗����Ă���Ɨ��ɏ��Z����B

10�i�@�̗���o���Ă݂悤�B
(306-703)*7/9 + 703 = 394 -> 304
�ŁA
(3-7)*7/9+7 = 3
(6-3)*7/9+3 = 5
�ňقȂ��Ă��܂����A�O�҂�
((306-703)*7 + 9*703) = 3548
�Ŏ~�߁A�㉺2���Âɕ�������ƁA
3500��48�ɂȂ�A�Ɨ���9�ŏ�����ƁA
388��5�B
��̌��̉��ʂ�؂�̂ĂĂ��獇�������305�ƂȂ�A��v����B

���łȂ̂ŁA�Y�������Z�����������Ă������B
*/
void Image::draw(
int dstX, 
int dstY, 
int srcX, 
int srcY, 
int width, 
int height ) const {
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();
	int dx = srcY * mWidth + srcX;
	int dy = dstY * windowWidth + dstX;
	for ( int y = 0; y < height; ++y ){
		for ( int x = 0; x < width; ++x ){
			unsigned src = mData[ sy + x ];
			unsigned* dst = &vram[ dy + x ];
			unsigned srcA = ( src & 0xff000000 ) >> 24;
			unsigned srcRB = src & 0xff00ff;
			unsigned srcG = src & 0x00ff00;
			unsigned dstRB = *dst & 0xff00ff;
			unsigned dstG = *dst & 0x00ff00;
			unsigned rb = ( srcRB - dstRB ) * srcA  + dstRB * 255;
			unsigned r = ( rb & 0xffff0000 ) / 255;
			unsigned b = ( rb & 0x0000ffff ) / 255;
			unsigned g = ( srcG - dstG ) * srcA / 255 + dstG;
			*dst = ( r & 0xff0000 ) | ( g & 0x00ff00 ) | b;
		}
		sy += mWidth;
		dy += windowWidth;
	}
}
#endif

#ifdef USE_OPTIMIZED_VERSION_1
//�A���t�@�u�����h�t��(���񉻔�)
/*
VERSION_0�ɉ����āA���Z���V�t�g�ɕϊ�����B
�A���t�@��0-255����255�Ŋ���˂΂Ȃ�Ȃ����A
����256�Ŋ����Ώ��Z���V�t�g�ɒu����������B
���̎��A255->255,0->0�͎�肽���B
�ςȂ��̂�������΃C���`�L���΂�Ă��܂����炾�B
�l�Ԃ͈Â����̐F�ω��ɂ͓݊��Ȃ̂ŁA
1�ȏ��+1���Ă��܂����B
�����āA256�Ŋ���ꍇ�͏��Z�ł��S�~������Ȃ����߁A
��̂悤�ɏ��Z��ԂƐʁX�ł��K�v���Ȃ��B
*/
void Image::draw(
int dstX, 
int dstY, 
int srcX, 
int srcY, 
int width, 
int height ) const {
	unsigned* vram = Framework::instance().videoMemory();
	unsigned windowWidth = Framework::instance().width();
	int sy = srcY * mWidth + srcX;
	int dy = dstY * windowWidth + dstX;
	for ( int y = 0; y < height; ++y ){
		for ( int x = 0; x < width; ++x ){
			unsigned src = mData[ sy + x ];
			unsigned* dst = &vram[ dy + x ];
			unsigned srcA = ( src & 0xff000000 ) >> 24;
			srcA += ( srcA >= 1 ) ? 1 : 0; //1�ȏ�Ȃ�1�𑫂��B
			unsigned srcRB = src & 0xff00ff;
			unsigned srcG = src & 0x00ff00;
			unsigned dstRB = *dst & 0xff00ff;
			unsigned dstG = *dst & 0x00ff00;
			unsigned rb = ( ( ( srcRB - dstRB ) * srcA ) >> 8 ) + dstRB;
			unsigned g = ( ( ( srcG - dstG ) * srcA ) >> 8 ) + dstG;
			*dst = ( rb & 0xff00ff ) | ( g & 0x00ff00 );
		}
		sy += mWidth;
		dy += windowWidth;
	}
}
#endif