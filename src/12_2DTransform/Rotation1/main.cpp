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
double rotation ){
	double xf = static_cast< double >( x );
	double yf = static_cast< double >( y );
	//�Y����������W�ɕϊ�
	xf += 0.5f;
	yf += 0.5f;
	//���_�����炷
	xf -= xOffset;
	yf -= yOffset;
	//�p�x�Ɣ��a�����߂�
	double r = GameLib::sqrt( xf * xf + yf * yf );
	double angle = GameLib::atan2( yf, xf );
	//�p�x��rotation��������B
	angle += rotation;
	//�T�C���R�T�C��
	double sine = GameLib::sin( angle );
	double cosine = GameLib::cos( angle );
	//xf,yf���v�Z���Ȃ���
	xf = r * cosine;
	yf = r * sine;
	//���_�����ɖ߂�
	xf += xOffset;
	yf += yOffset;
	//���W����Y������
	xf -= 0.5;
	yf -= 0.5;
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
		for ( int y = 0; y < ih; ++y ){
			for ( int x = 0; x < iw; ++x ){
				//��]����v�Z
				int rx, ry;
				rotate( &rx, &ry, x, y, offsetX, offsetY, rotation );
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