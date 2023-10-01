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

void transform(
Vector2* out,
const Vector2& in,
const Vector2& scalingOffset,
const Vector2& scalingRatio,
const Vector2& rotationOffset,
const Matrix22& rotationMatrix ){
	//拡大縮小
	out->setMul( scalingRatio, in );
	//移動
	*out += scalingOffset;
	//回転中心ずらし
	*out -= rotationOffset;
	//回転
	rotationMatrix.multiply( out, *out );
	//回転中心ずらしもどし
	*out += rotationOffset;
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
		//一旦全部真っ黒に
		for ( int i = 0; i < ww * wh; ++i ){
			vram[ i ] = 0;
		}
		int iw = gImage->width(); //image width
		int ih = gImage->height(); //image height
		double rotation = static_cast< double >( gCount ); //これを拡大率に使おう
		//拡大率
		Vector2 scalingRatio( 1.1 + sin( rotation ), 1.1 + cos( rotation ) );
		//拡大オフセット(拡大率に応じて変わることに注意)
		Vector2 scalingOffset( ww/2 - iw/2*scalingRatio.x, wh/2 - ih/2*scalingRatio.y );
		//回転オフセット
		Vector2 rotationOffset( ww/2, wh/2 );
		//回転行列
		double sine = sin( rotation );
		double cosine = cos( rotation );
		Matrix22 rotationMatrix( cosine, -sine, sine, cosine );
		
		//3点作る
		Vector2 a, b, c;
		transform( &a, Vector2( 0, 0 ), scalingOffset, scalingRatio, rotationOffset, rotationMatrix ); //左上
		transform( &b, Vector2( iw, 0 ), scalingOffset, scalingRatio, rotationOffset, rotationMatrix ); //右上
		transform( &c, Vector2( 0, ih ), scalingOffset, scalingRatio, rotationOffset, rotationMatrix ); //左下
		//b-a,c-aを計算
		Vector2 ab, ac;
		ab.setSub( b, a );
		ac.setSub( c, a );
		//補間開始
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
				p -= Vector2( 0.5, 0.5 ); //添え字へ
				int rx, ry;
				rx = round( p.x );
				ry = round( p.y );
				//範囲内なら貼り付け
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