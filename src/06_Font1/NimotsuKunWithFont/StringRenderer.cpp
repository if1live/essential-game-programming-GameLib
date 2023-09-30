#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "StringRenderer.h"
#include "Image.h"

//static�ϐ���cpp�Ɏ��̂��K�v
StringRenderer* StringRenderer::mInstance = 0;

StringRenderer::StringRenderer( const char* fontFileName ) : mImage( 0 ){
	mImage = new Image( fontFileName );
}

StringRenderer::~StringRenderer(){
	SAFE_DELETE( mImage );
}

void StringRenderer::create( const char* fontFileName ){
	//��x�A���ŌĂԂ͕̂s���B���Ƃ��B
	STRONG_ASSERT( !mInstance && "StringRenderer::create() called twice!" );
	mInstance = new StringRenderer( fontFileName );
}

void StringRenderer::destroy(){
	SAFE_DELETE( mInstance );
}

StringRenderer* StringRenderer::instance(){
	return mInstance;
}

void StringRenderer::draw( int x, int y, const char* s, unsigned c ) const {
	//��̒萔�B�����t�H���g�ɂ���ĈႤ�Ȃ�R���X�g���N�^�Ŏ󂯎��K�v�����邪�A���͌Œ�B
	const int CHAR_WIDTH = 8;
	const int CHAR_HEIGHT = 16;
	//�������ݐ��������
	int dstX = x * CHAR_WIDTH;
	int dstY = y * CHAR_HEIGHT;
	//Framework�擾
	Framework f = Framework::instance();
	//�͈͊O�`�F�b�N(Y���O�ꂽ��ⓚ���p�Ŕ�����)
	if ( dstY < 0 || dstY >= ( int )f.height() ){
		return;
	}
	//�摜��
	for ( int i = 0; s[ i ] != '\0'; ++i ){
		//X�͈͔���
		if ( dstX >= 0 && ( dstX + CHAR_WIDTH ) < f.width() ){
			//�����͈̓`�F�b�N
			int t = s[ i ];
			if ( t < 32 || t >= 128 ){
				t = 127; //�͈͊O�͎l�p�ɕϊ�
			}
			t -= 32; //�t�H���g�摜�̍��オ32�ԂȂ̂ŁA32������
			int srcX = ( t % 16 ) * CHAR_WIDTH; //X�͕��Ŋ������]��
			int srcY = ( t / 16 ) * CHAR_HEIGHT; //Y�͕��Ŋ�������
			mImage->drawWithFixedColor( dstX, dstY, srcX, srcY, CHAR_WIDTH, CHAR_HEIGHT, c );
		}
		dstX += CHAR_WIDTH;
	}
}



