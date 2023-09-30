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


//���Z�u�����h
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
			unsigned r = srcR * srcA / 255 + dstR;
			unsigned g = srcG * srcA / 255 + dstG;
			unsigned b = srcB * srcA / 255 + dstB;
			//255�𒴂�����255��
			r = ( r > 0xff0000 ) ? 0xff0000 : r;
			g = ( g > 0x00ff00 ) ? 0x00ff00 : g;
			b = ( b > 0x0000ff ) ? 0x0000ff : b;
			*dst = ( r & 0xff0000 ) | ( g & 0x00ff00 ) | b;
		}
	}
}


