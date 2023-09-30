#include "Matrix23.h"
#include "Vector2.h"
#include "GameLib/Math.h" //sin,cos�̂��߂�
using namespace GameLib;

Matrix23::Matrix23(){
}

void Matrix23::setTranslation( const Vector2& a ){
	m00 = m11 = 1.0;
	m01 = m10 = 0.0;
	m02 = a.x;
	m12 = a.y;
}

void Matrix23::setRotation( double r ){
	double s = sin( r );
	double c = cos( r );
	m00 = m11 = c;
	m01 = s;
	m10 = -s;
	m02 = m12 = 0.0;
}

void Matrix23::setScaling( const Vector2& a ){
	m00 = a.x;
	m11 = a.y;
	m01 = m10 = m02 = m12 = 0.0;
}

void Matrix23::operator*=( const Matrix23& m ){
	double tx, ty;
	tx = m00; ty = m01;
	m00 = tx * m.m00 + ty * m.m10;
	m01 = tx * m.m01 + ty * m.m11;
	m02 = tx * m.m02 + ty * m.m12 + m02;
	tx = m10; ty = m11;
	m10 = tx * m.m00 + ty * m.m10;
	m11 = tx * m.m01 + ty * m.m11;
	m12 = tx * m.m02 + ty * m.m12 + m12;
}

void Matrix23::multiply( Vector2* out, const Vector2& in ) const {
	double tx = in.x; //out��in��������������Ȃ��̂ŁA�o�b�N�A�b�v
	out->x = m00 * tx + m01 * in.y + m02; //����*x + ����*y + �E��*1
	out->y = m10 * tx + m11 * in.y + m12; //����*x + ����*y + ����*1
}
