#include "Matrix23.h"
#include "Vector2.h"
#include "GameLib/Math.h"
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

//���ʂ̂Ȃ��ړ��s���Z�֐��̍���
/*
a b c
d e f

��

1 0 tx
0 1 ty

���|����B3x3�Ɋg������΁A

a b c   1 0 tx
d e f   0 1 ty
0 0 1   0 0 1

3�s�ڂ͂ǂ��ł������̂ŏ�2�s�����v�Z����ƁA

a b (a*tx + b*ty + c)
d e (d*tx + e*ty + f)

�̂��Ƃ��B���ꂪ�����ł���B
�܂�Am02��m12�����G��Ȃ��Ă���
*/

void Matrix23::translate( const Vector2& a ){
	m02 += m00 * a.x + m01 * a.y; 
	m12 += m10 * a.x + m11 * a.y;
}

//���ʂ̂Ȃ��g��k���s���Z�̍���
/*
a b c
d e f

��

sx 0 0
0 sy 0

���|����B

a b c   sx 0  0
d e f   0  sy 0
0 0 1   0  0  1

�̏�2�s���v�Z����΁A

a*sx b*sy c
d*sx e*sy f

���ꂪ�����ł���B
m00,m01,m10,m11��4�v�f��G��΂���
*/
void Matrix23::scale( const Vector2& a ){
	m00 *= a.x;
	m01 *= a.y;
	m10 *= a.x;
	m11 *= a.y;
}

//���ʂ̂Ȃ���]�s���Z�̍���
/*
a b c
d e f

��

p -q 0
q p 0

���|����Bp���R�T�C���ŁAq���T�C�����B

a b c   p -q 0
d e f   q  p 0
0 0 1   0  0 1

��2�s���v�Z����ƁA

(ap + bq) (-aq + bp) c
(dp + eq) (-dq + ep) f

m00,m01,m10,m11��4�v�f��G��΂���
*/

void Matrix23::rotate( double r ){
	double c = cos( r );
	double s = sin( r );
	//m00�̓o�b�N�A�b�v�����܂��B��s�ڂ�m00���㏑�����Ă��܂��̂ŁB
	double t = m00;
	m00 = t * c + m01 * s;
	m01 = t * ( -s ) + m01 * c;
	t = m10; //m10���o�b�N�A�b�v
	m10 = t * c + m11 * s;
	m11 = t * ( -s ) + m11 * c;
}
