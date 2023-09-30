#include <sstream>
#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Vector2.h"
#include "Matrix22.h"
#include "GameLib/Math.h"
using namespace std;

void rotate(
Vector2* out,
const Vector2& in,
const Vector2& offset,
const Matrix22& matrix ){
	Vector2 tmpOffset( -0.5, -0.5 );
	tmpOffset += offset;
	//���_�����炵��
	out->setSub( in, tmpOffset );
	//�s��������A
	matrix.multiply( out, *out );
	//���_�����ɖ߂��ēY�����ɕϊ�
	*out += tmpOffset;
}

bool gFirstFrame = true;
Image* gImage;
int gCount;

//�O�p�`�����X�^���C�Y
void rasterize( 
const Vector2& a, 
const Vector2& b, 
const Vector2& c, 
const Vector2& ai,
const Vector2& bi,
const Vector2& ci,
const Image* image ){
	//abc��y�����������ɕ��ׂ�B
	Vector2 p0, p1, p2;
	Vector2 i0, i1, i2;
	if ( a.y < b.y ){
		if ( b.y < c.y ){ //abc
			p0 = a; p1 = b; p2 = c;
			i0 = ai; i1 = bi; i2 = ci;
		}else if ( a.y < c.y ){ //acb
			p0 = a; p1 = c; p2 = b;
			i0 = ai; i1 = ci; i2 = bi;
		}else{ //cab
			p0 = c; p1 = a; p2 = b;
			i0 = ci; i1 = ai; i2 = bi;
		}
	}else{
		if ( a.y < c.y ){ //bac
			p0 = b; p1 = a; p2 = c;
			i0 = bi; i1 = ai; i2 = ci;
		}else if ( b.y < c.y ){ //bca
			p0 = b; p1 = c; p2 = a;
			i0 = bi; i1 = ci; i2 = ai;
		}else{ //cba
			p0 = c; p1 = b; p2 = a;
			i0 = ci; i1 = bi; i2 = ai;
		}
	}
	//p0��p2�����ԕӂ�y=p1.y�̐������̌�_�����߂�B
	//y�͓��Rp1.y������Ax�������߂�΂����B
	//���܂��������g����
	//x = p0x + (p2x-p0x) * (p1y-p0y)/(p2y-p0y);
	//�ł��邱�Ƃ��킩��Bp1.y��p0.y��p2.y�̐^�񒆂ɂ����p2x-p0x�𔼕��ɂ���
	//p0x�ɑ����Ă��΂����Ƃ������Ƃ��B
	Vector2 p01, p02, p03;
	p01.setSub( p1, p0 );
	p02.setSub( p2, p0 );

	Vector2 p3;
	double t = ( p1.y - p0.y ) / p02.y; //���ꂩ��t�͈ꎞ�I�ȗp�r�ɂ��낢��g��
	p3.x = p0.x + p02.x * t;
	p3.y = p1.y;

	p03.setSub( p3, p0 );

	//VRAM�Q�b�g
	unsigned* vram = Framework::instance().videoMemory();
	int ww = Framework::instance().width();

	//�摜���̓Y�����Ɖ�ʏ�̈ʒu�̊֌W�����ɂ��Ă����B
	//x,y���킩���ix,iy���킩��悤�ɂ��Ă������Ƃ��K�v���B
	/*
	ix,iy��x��y�̈ꎟ���ŕ\����͂��Ȃ̂ŁA

	ix = ax + by + c
	iy = dx + ey + f

	�Ə�����B���m����6�����玮��6�{�K�v���B
	3��uv������̂����炱��ő����͂��ł���B
	i0x = p0x*a + p0y*b + c
	i1x = p1x*a + p1y*b + c
	i2x = p2x*a + p2y*b + c

	i0y = p0x*d + p0y*e + f
	i1y = p1x*d + p1y*e + f
	i2y = p2x*d + p2y*e + f

	3�{�Â����Ă��΂����B
	i01x = p01x*a + p01y*b
	i02x = p02x*a + p02y*b

	�Ώ̐��𗘗p���Ă���
	a = ( i01x*p02y - i02x*p01y ) / ( p01x*p02y - p02x*p01y )
	b = ( i01x*p02x - i02x*p01x ) / ( p01y*p02x - p02y*p01x )
	c = uv0x - p0x*a - p0y*b
	d = ( i01y*p02y - i02y*p01y ) / ( p01x*p02y - p02x*p01y )
	e = ( i01y*p02x - i02y*p01x ) / ( p01y*p02x - p02y*p01x )
	f = uv0y - p0x*a - p0y*b
	*/
	double a0,a1,a2,a3,a4,a5; //abcdef�̑���
	Vector2 i01, i02;
	i01.setSub( i1, i0 );
	i02.setSub( i2, i0 );
	a0 = ( i01.x * p02.y - i02.x * p01.y ) / ( p01.x * p02.y - p02.x * p01.y );
	a1 = ( i01.x * p02.x - i02.x * p01.x ) / ( p01.y * p02.x - p02.y * p01.x );
	a2 = i0.x - p0.x * a0 - p0.y * a1;
	a3 = ( i01.y * p02.y - i02.y * p01.y ) / ( p01.x * p02.y - p02.x * p01.y );
	a4 = ( i01.y * p02.x - i02.y * p01.x ) / ( p01.y * p02.x - p02.y * p01.x );
	a5 = i0.y - p0.x * a3 - p0.y * a4;

	//�ł͏㔼���̎O�p�`����n�߂悤�Bp0,p1,p3���B
	//�܂�Y�͈̔͂𐮐��ɂ���B
	if ( p1.y > p0.y + 1.0 ){
		int yBegin = static_cast< int >( p0.y );
		int yEnd = static_cast< int >( p1.y );
		double rcpP01y = 1.0 / p01.y;
		for ( int y = yBegin; y <= yEnd; ++y ){
			//�w���y�ɂ�����x�͈̔͂𒲂ׂ�B
			double yf = y;
			//p3-p0��p1-p0�̓�ӂƍ���y�̐������̌�_�����߂Ă��΂����B
			//�v�Z�͐�قǂ̂Ɠ������B
			t = ( y - p0.y ) * rcpP01y;
			double x0 = p0.x + p01.x * t; //p0.x + p01.x * ( y - p0.y ) / p01.y
			double x1 = p0.x + p03.x * t; //p0.x + p01.x * ( y - p0.y ) / p03.y ������p01.y=p03.y�B
			int xBegin, xEnd;
			if ( x0 > x1 ){
				xBegin = static_cast< int >( x1 ); 
				xEnd = static_cast< int >( x0 ); 
			}else{
				xBegin = static_cast< int >( x0 );
				xEnd = static_cast< int >( x1 ); 
			}
			for ( int x = xBegin; x <= xEnd; ++x ){
				double xf = x;
				//x,y����ix,iy�����߂�
				Vector2 ip;
				ip.x = a0 * xf + a1 * yf + a2;
				ip.y = a3 * xf + a4 * yf + a5;
				//�s�N�Z���Q�b�g
				vram[ y * ww + x ] = image->pixel( ip );
			}
		}
	}
	//���͉������̎O�p�`�B��邱�Ƃ͂قƂ�Ǔ����B
	if ( p2.y > p1.y + 1.0 ){
		Vector2 p21, p23;
		p21.setSub( p1, p2 );
		p23.setSub( p3, p2 );
		int yBegin = static_cast< int >( p1.y );
		int yEnd = static_cast< int >( p2.y );
		double rcpP21y = 1.0 / p21.y;
		for ( int y = yBegin; y <= yEnd; ++y ){
			//�w���y�ɂ�����x�͈̔͂𒲂ׂ�B
			double yf = y;
			//p3-p2��p1-p2�̓�ӂƍ���y�̐������̌�_�����߂Ă��΂����B
			//�v�Z�͐�قǂ̂Ɠ������B
			t = ( y - p2.y ) * rcpP21y;
			double x0 = p2.x + p21.x * t; //p2.x + p21.x * ( y - p2.y ) / p21.y
			double x1 = p2.x + p23.x * t; //p2.x + p21.x * ( y - p2.y ) / p23.y ������p21.y=p23.y�B
			int xBegin, xEnd;
			if ( x0 > x1 ){
				xBegin = static_cast< int >( x1 ); 
				xEnd = static_cast< int >( x0 ); 
			}else{
				xBegin = static_cast< int >( x0 );
				xEnd = static_cast< int >( x1 ); 
			}
			for ( int x = xBegin; x <= xEnd; ++x ){
				double xf = x;
				//x,y����ix,iy�����߂�
				Vector2 ip;
				ip.x = a0 * xf + a1 * yf + a2;
				ip.y = a3 * xf + a4 * yf + a5;
				//�s�N�Z���Q�b�g
				vram[ y * ww + x ] = image->pixel( ip );
			}
		}
	}
}


namespace GameLib{
	void Framework::update(){
		if ( gFirstFrame ){
			gImage = new Image( "background.dds" );
			gFirstFrame = false;
		}
		int ww = width(); //window width
		int wh = height(); //window height

		//���t���[���N���A
		unsigned* vram = videoMemory();
		for ( int i = 0; i < ww * wh; ++i ){
			vram[ i ] = 0;
		}

		int iw = gImage->width(); //image width
		int ih = gImage->height(); //image height
		//�I�t�Z�b�g�͉�ʒ��S�B(160,120)
		Vector2 offset;
		offset.x = static_cast< double >( ww ) / 2.0;
		offset.y = static_cast< double >( wh ) / 2.0;
		
		double rotation = static_cast< double >( gCount );
		double sine = sin( rotation ); //���x�͎��������̂Ń}�C�i�X�͒����Ȃ�
		double cosine = cos( rotation );
		Matrix22 matrix( cosine, -sine, sine, cosine ); //�s��������
		//4�_���
		Vector2 a, b, c, d;
		rotate( &a, Vector2( ( ww - iw )/2, ( wh - ih )/2 ), offset, matrix ); //����
		rotate( &b, Vector2( ( ww + iw )/2 - 1, ( wh - ih )/2 ), offset, matrix ); //�E��
		rotate( &c, Vector2( ( ww - iw )/2, ( wh + ih )/2 - 1 ), offset, matrix ); //����
		//4�_�ڂ�a + (b-a) + (c-a)�Ōv�Z�ł���B
		Vector2 ab, ac;
		ab.setSub( b, a );
		ac.setSub( c, a );
		d.setAdd( a, ab );
		d += ac;
		//a,b,c,d�ɂ����āA�摜���̓Y�����������Ȃ̂����v�Z
		Vector2 ai( 0, 0 );
		Vector2 bi( iw - 1, 0 ); //(w-1)/w, 0)
		Vector2 ci( 0, ih - 1 ); //(0,(h-1)/h)
		Vector2 di( bi.x, ci.y );
		//���X�^���C�Y�J�n
		rasterize( a, b, c, ai, bi, ci, gImage ); //�O�p�`abc
		rasterize( d, b, c, di, bi, ci, gImage ); //�O�p�`dbc

		++gCount;

		ostringstream oss;
		oss << frameRate();
		drawDebugString( 0, 0, oss.str().c_str() );
	}
}