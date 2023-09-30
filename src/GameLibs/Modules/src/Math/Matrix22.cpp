#include "GameLib/GameLib.h"
#include "GameLib/Math/Matrix22.h"
#include "GameLib/Math/Functions.h"
#include "GameLib/Math/Vector2.h"

namespace GameLib{
namespace Math{

Matrix22::Matrix22(){
}

void Matrix22::setIdentity(){
	m00 = m11 = 1.f;
	m01 = m10 = 0.f;
}

void Matrix22::setRotation( float r ){
	float s = sin( r );
	float c = cos( r );
	m00 = m11 = c;
	m01 = -s;
	m10 = s;
}

void Matrix22::setScaling( float xy ){
	m00 = m11 = xy;
	m01 = m10 = 0.f;
}

void Matrix22::setScaling( float x, float y ){
	m00 = x;
	m11 = y;
	m01 = m10 = 0.f;
}

void Matrix22::setScaling( const Vector2& a ){
	m00 = a.x;
	m11 = a.y;
	m01 = m10 = 0.f;
}

void Matrix22::rotate( float r ){
	float s = sin( r );
	float c = cos( r );
	float t = m00;
	m00 = t * c + m01 * s;
	m01 = t * ( -s ) + m01 * c;
	t = m10;
	m10 = t * c + m11 * s;
	m11 = t * ( -s ) + m11 * c;
}

void Matrix22::scale( float xy ){
	m00 *= xy;
	m01 *= xy;
	m10 *= xy;
	m11 *= xy;
}

void Matrix22::scale( float x, float y ){
	m00 *= x;
	m01 *= y;
	m10 *= x;
	m11 *= y;
}

void Matrix22::scale( const Vector2& a ){
	m00 *= a.x;
	m01 *= a.y;
	m10 *= a.x;
	m11 *= a.y;
}

void Matrix22::mul( Vector2* out, const Vector2& in ) const {
	float tx = in.x;
	out->x = m00 * tx + m01 * in.y;
	out->y = m10 * tx + m11 * in.y;
}

void Matrix22::operator*=( const Matrix22& a ){
	float tx = m00;
	m00 = tx * a.m00 + m01 * a.m10;
	m01 = tx * a.m01 + m01 * a.m11;
	tx = a.m10;
	m10 = tx * a.m00 + m11 * a.m10;
	m11 = tx * a.m01 + m11 * a.m11;
}

void Matrix22::setMul( const Matrix22& a, const Matrix22& b ){
	//a==*this, b==*this�������l���Ȃ��Ƃ����Ȃ�
	float t00, t01;
	t00 = a.m00 * b.m00 + a.m01 * b.m10;
	t01 = a.m00 * b.m01 + a.m01 * b.m11;

	float t10;
	t10 = a.m10 * b.m00 + a.m11 * b.m10;
	m11 = a.m10 * b.m01 + a.m11 * b.m11;

	m00 = t00; m01 = t01;
	m10 = t10;
}

float& Matrix22::operator()( int r, int c ){
	ASSERT( r >= 0 && r < 2 );
	ASSERT( c >= 0 && c < 2 );
	return ( &m00 )[ 2 * r + c ];
}

float Matrix22::operator()( int r, int c ) const {
	ASSERT( r >= 0 && r < 2 );
	ASSERT( c >= 0 && c < 2 );
	return ( &m00 )[ 2 * r + c ];
}

void Matrix22::transpose(){
	float t = m01;
	m01 = m10;
	m10 = t;
}

void Matrix22::setTransposed( const Matrix22& a ){
	m00 = a.m00;
	m01 = a.m10;
	m10 = a.m01;
	m11 = a.m11;
}

//�N���[�����@�ł̋t�s��
/*
|x y| |a b| |1 0|
|z w|*|c d|=|0 1|
���l����B�������߂�t�s��A�E�͍�����s�񂾁B
�㔼����������ƁA

ax+cy=1
bx+dy=0

�ƂȂ�B���d�A����c�������Ĉ�����

x(ad - bc) = (1*d-0*c)
x = (1*d-0*c)/(ad-bc)

y�́A���b�A����a�������Ĉ�����

y(bc - ad) = (1*b-0*a)
y = (1*b-0*a)/(bc-ad)

�����x�Ƃ��낦�邽�߂ɕ��q�����-1��������

y = (0*a-1*b)/(ad-bc)

z,w�Ɋւ��Ă�0��1������ւ�邾��������A

z = (0*d-1*c)/(ad-bc)
w = (1*a-0*b)/(ad-bc)

ad-bc=D�ƒu����(�s��)

x = (1*d-0*c)/D
y = (0*a-1*b)/D
z = (0*d-1*c)/D
w = (1*a-0*b)/D

�ƁA���炩�̋K�����̌�����`���o�Ă���B
�����2x2�ȊO�ł��g����悤�ɂ������̂��N���[�����̌����ƌ����B
�������A����2x2�Ȃ̂ŏ�̎������̂܂܎g���΂����B

x = d/D
y = -b/D
z = -c/D
w = a/D

|1 2|    | -2    1|
|3 4| -> |1.5 -0.5|
���m�F���悤
*/
void Matrix22::setInverse( const Matrix22& a ){
	float delta = a.m00 * a.m11 - a.m01 * a.m10;
	float rcpDelta = 1.f / delta;
	float t = a.m00; //a==*this�̏ꍇ�ɔ����ăo�b�N�A�b�v
	m00 = a.m11 * rcpDelta;
	m01 = -a.m01 * rcpDelta;
	m10 = -a.m10 * rcpDelta;
	m11 = t * rcpDelta;
}

void Matrix22::invert(){ 
	setInverse( *this ); //���ȏ��������Ή����Ă���̂ł���ł悢
}

bool Matrix22::operator==( const Matrix22& a ) const {
	for ( int i = 0; i < 2; ++i ){
		for ( int j = 0; j < 2; ++j ){
			if ( ( *this )( i, j ) != a( i, j ) ){
				return false;
			}
		}
	}
	return true;
}

bool Matrix22::operator!=( const Matrix22& a ) const {
	return !( operator==( a ) );
}

} //namespace Math
} //namespace GameLib
