#include <sstream>
#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
#include "Image.h"
#include "GameLib/Math.h"
using namespace GameLib;
using namespace std;

int round( double a ){
	a += ( a > 0.0 ) ? 0.5 : -0.5f;
	return static_cast< int >( a );
}

void rotate(
int* rx,
int* ry,
int x,
int y,
double xOffset,
double yOffset,
double sine,
double cosine ){
	double xf = static_cast< double >( x );
	double yf = static_cast< double >( y );
	//�I�t�Z�b�g��0.5���܂Ƃ߂�
	double tmpOffsetX = xOffset - 0.5;
	double tmpOffsetY = yOffset - 0.5;
	//�Y����������W�ɕϊ����āA���_�����炷
	xf -= tmpOffsetX;
	yf -= tmpOffsetY;
	//�����܂�ܓ��Ă͂߂�
	double xf2 = cosine * xf - sine * yf;
	double yf2 = sine * xf + cosine * yf;
	xf = xf2;
	yf = yf2;
	//���_�����ɖ߂��ēY������
	xf += tmpOffsetX;
	yf += tmpOffsetY;
	//�l�̌ܓ����Đ�����
	*rx = round( xf );
	*ry = round( yf );
}

bool gFirstFrame = true;
Image* gImage;
int gCount;

namespace GameLib{
	void Framework::update(){
		if ( gFirstFrame ){
			gImage = new Image( "background.dds" );
			gFirstFrame = false;
		}
		unsigned* vram = videoMemory();
		int ww = width(); //window width
		int wh = height(); //window height
		//��U�S���^������
		for ( int i = 0; i < ww * wh; ++i ){
			vram[ i ] = 0;
		}
		int iw = gImage->width(); //image width
		int ih = gImage->height(); //image height
		double offsetX = static_cast< double >( iw ) / 2.0;
		double offsetY = static_cast< double >( ih ) / 2.0;
		double rotation = static_cast< double >( gCount );
		double sine = sin( rotation );
		double cosine = cos( rotation );
		for ( int y = 0; y < ih; ++y ){
			for ( int x = 0; x < iw; ++x ){
				//��]����v�Z
				int rx, ry;
				rotate( &rx, &ry, x, y, offsetX, offsetY, sine, cosine );
				//�͈͓��Ȃ�\��t��
				if ( rx >= 0 && rx < ww && ry >= 0 && ry < wh ){
					vram[ ry * ww + rx ] = gImage->pixel( x, y );
				}
			}
		}
		++gCount;

		ostringstream oss;
		oss << frameRate();
		drawDebugString( 0, 0, oss.str().c_str() );
	}
}

