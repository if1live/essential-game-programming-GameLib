#include <sstream>
#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
#include "Image.h"
#include "Vector2.h"
#include "Matrix22.h"
#include "GameLib/Math.h"
using namespace std;

int round( double a ){
	a += ( a > 0.0 ) ? 0.5 : -0.5f;
	return static_cast< int >( a );
}

void rotate(
Vector2* out,
const Vector2& in,
const Vector2& offset,
const Matrix22& matrix ){
	//���_�����炵��
	out->setSub( in, offset );
	//�s��������A
	matrix.multiply( out, *out );
	//���_�����ɖ߂�
	*out += offset;
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
		Vector2 offset;
		offset.x = static_cast< double >( iw ) / 2.0;
		offset.y = static_cast< double >( ih ) / 2.0;
		
		double rotation = static_cast< double >( gCount );
		double sine = sin( rotation );
		double cosine = cos( rotation );
		Matrix22 matrix( cosine, -sine, sine, cosine ); //�s��������
		//3�_���
		Vector2 a, b, c;
		rotate( &a, Vector2( 0, 0 ), offset, matrix ); //����
		rotate( &b, Vector2( iw, 0 ), offset, matrix ); //�E��
		rotate( &c, Vector2( 0, ih ), offset, matrix ); //����
		//b-a,c-a���v�Z
		Vector2 ab, ac;
		ab.setSub( b, a );
		ac.setSub( c, a );
		//��ԊJ�n
		double rcpWidth = 1.0 / static_cast< double >( iw );
		double rcpHeight = 1.0 / static_cast< double >( ih );
		for ( int y = 0; y < ih; ++y ){
			double yf = static_cast< double >( y ) + 0.5;
			double v = yf * rcpHeight;
			for ( int x = 0; x < iw; ++x ){
				double xf = static_cast< double >( x ) + 0.5;
				double u = xf * rcpWidth;
				Vector2 p;
				p.setInterporation( a, ab, ac, u, v );
				p -= Vector2( 0.5, 0.5 ); //�Y������
				int rx, ry;
				rx = round( p.x );
				ry = round( p.y );
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