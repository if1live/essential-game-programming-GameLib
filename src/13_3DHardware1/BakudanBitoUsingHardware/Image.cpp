#include "Image.h"
#include "File.h"
#include "Vector2.h"

#include "GameLib/Framework.h"
using namespace GameLib;

static int powerOfTwo( int a ){
	ASSERT( a < 0x40000000 ); //a��0x40000000���傫���ƁA�������[�v�ɂȂ�B�����Ă݂悤�B���ꂪ�����B
	int r = 1;
	while ( r < a ){
		r *= 2;
	}
	return r;
}

Image::Image( const char* filename ) : 
mWidth( 0 ),
mHeight( 0 ),
mTextureWidth( 0 ),
mTextureHeight( 0 ),
mTexture( 0 ){
	File f( filename );
	mHeight = f.getUnsigned( 12 );
	mWidth = f.getUnsigned( 16 );
	//�ꎞ�o�b�t�@�ɃR�s�[�Bchar*���璼��createTexture�͊�Ȃ�����B
	unsigned* buffer = new unsigned[ mWidth * mHeight ];
	for ( int i = 0; i < mWidth * mHeight; ++i ){
		buffer[ i ] = f.getUnsigned( 128 + i * 4 );
	}
	//createTexture���܂��B
	//���̂��߂ɁA2�̙p��(�ׂ����傤)�����𑜓x�����߂�K�v������B
	mTextureWidth = powerOfTwo( mWidth );
	mTextureHeight = powerOfTwo( mHeight );

	Framework::instance().createTexture(
		&mTexture,
		mTextureWidth,
		mTextureHeight,
		buffer,
		mWidth,
		mHeight );
	SAFE_DELETE_ARRAY( buffer ); //��������Ȃ�
}

Image::~Image(){
	Framework::instance().destroyTexture( &mTexture );
}

int Image::width() const {
	return mWidth;
}

int Image::height() const {
	return mHeight;
}

//�A���t�@�u�����h�t��
void Image::draw(
int dstX, 
int dstY, 
int srcX, 
int srcY, 
int width, 
int height ) const {
	//x,y�͈̔͂��v�Z
	double x0 = static_cast< double >( dstX );
	double y0 = static_cast< double >( dstY );
	double x1 = x0 + static_cast< double >( width );
	double y1 = y0 + static_cast< double >( height );
	//�ړ��㒸�_
	Vector2 p0( x0, y0 );
	Vector2 p1( x1, y0 );
	Vector2 p2( x0, y1 );
	Vector2 p3( x1, y1 );
	//�e�N�X�`�����W����
	double rcpTw = 1.0 / static_cast< double >( mTextureWidth );
	double rcpTh = 1.0 / static_cast< double >( mTextureHeight );
	double u0 = static_cast< double >( srcX ) * rcpTw;
	double u1 = static_cast< double >( srcX + width ) * rcpTw;
	double v0 = static_cast< double >( srcY ) * rcpTh;
	double v1 = static_cast< double >( srcY + height ) * rcpTh;
	Vector2 t0( u0, v0 );
	Vector2 t1( u1, v0 );
	Vector2 t2( u0, v1 );
	Vector2 t3( u1, v1 );
	
	Framework f = Framework::instance();
	//�e�N�X�`���Z�b�g
	f.setTexture( mTexture );
	//���`����
	f.setBlendMode( Framework::BLEND_LINEAR );
	//�`��
	f.drawTriangle2D( &p0.x, &p1.x, &p2.x, &t0.x, &t1.x, &t2.x );
	f.drawTriangle2D( &p3.x, &p1.x, &p2.x, &t3.x, &t1.x, &t2.x );
}

void Image::draw() const {
	draw( 0, 0, 0, 0, mWidth, mHeight );
}

