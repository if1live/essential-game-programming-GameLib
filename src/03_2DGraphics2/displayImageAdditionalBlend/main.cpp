#include "GameLib/Framework.h"
#include "Image.h"

bool gFirst = true;
Image* gForeground;
Image* gBackground;

namespace GameLib{
	void Framework::update(){
		if ( gFirst ){
			gFirst = false;
			gForeground = new Image( "forground.dds" );
			gBackground = new Image( "background.dds" );
		}
		//��ʂ�^�����œh��Ԃ�(���Z���ƑO�̃t���[���ɂ��ǂ�ǂ񑫂���Đ^�����ɂȂ�)
		unsigned* vram = Framework::instance().videoMemory();
		int ww = Framework::instance().width();
		int wh = Framework::instance().height();
		for ( int i = 0; i < ww * wh; ++i ){
			vram[ i ] = 0;
		}

		gBackground->draw( 0, 0, 0, 0, gBackground->width(), gBackground->height() );
		gForeground->draw( 0, 0, 0, 0, gForeground->width(), gForeground->height() );
	}
}
