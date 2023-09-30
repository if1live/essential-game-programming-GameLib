#include "GameLib/GameLib.h"
#include "GameLib/Math/Matrix33.h"
#include "GameLib/Math/Functions.h"
#include "GameLib/Math/Vector3.h"

namespace GameLib{
namespace Math{

Matrix33::Matrix33(){
}

void Matrix33::setIdentity(){
	m00 = m11 = m22 = 1.f;
	m01 = m02 = m10 = m12 = m20 = m21 = 0.f;
}

void Matrix33::setRotationX( float r ){
	float c = cos( r );
	float s = sin( r );
	m11 = m22 = c;
	m12 = -s;
	m21 = s;
	m00 = 1.0;
	m01 = m02 = m10 = m20 = 0.0;
}

void Matrix33::setRotationY( float r ){
	float c = cos( r );
	float s = sin( r );
	m00 = m22 = c;
	m20 = -s;
	m02 = s;
	m11 = 1.0;
	m01 = m10 = m12 = m21 = 0.0;
}

void Matrix33::setRotationZ( float r ){
	float c = cos( r );
	float s = sin( r );
	m00 = m11 = c;
	m01 = -s;
	m10 = s;
	m22 = 1.0;
	m02 = m12 = m20 = m21 = 0.0;
}

void Matrix33::setScaling( float xyz ){
	m00 = m11 = m22 = xyz;
	m01 = m02 = m10 = m12 = m20 = m21 = 0.f;
}

void Matrix33::setScaling( float x, float y, float z ){
	m00 = x;
	m11 = y;
	m22 = z;
	m01 = m02 = m10 = m12 = m20 = m21 = 0.f;
}

void Matrix33::setScaling( const Vector3& a ){
	m00 = a.x;
	m11 = a.y;
	m22 = a.z;
	m01 = m02 = m10 = m12 = m20 = m21 = 0.f;
}

void Matrix33::rotateX( float r ){
	float c = cos( r );
	float s = sin( r );
	float t;
	t = c * m01 + s * m02;
	m02 = -s * m01 + c * m02;
	m01 = t;
	t = c * m11 + s * m12;
	m12 = -s * m11 + c * m12;
	m11 = t;
	t = c * m21 + s * m22;
	m22 = -s * m21 + c * m22;
	m21 = t;
}

void Matrix33::rotateY( float r ){
	float c = cos( r );
	float s = sin( r );
	float t;
	t = c * m00 - s * m02;
	m02 = s * m00 + c * m02;
	m00 = t;
	t = c * m10 - s * m12;
	m12 = s * m10 + c * m12;
	m10 = t;
	t = c * m20 - s * m22;
	m22 = s * m20 + c * m22;
	m20 = t;
}

void Matrix33::rotateZ( float r ){
	float c = cos( r );
	float s = sin( r );
	float t;
	t = c * m00 + s * m01;
	m01 = -s * m00 + c * m01;
	m00 = t;
	t = c * m10 + s * m11;
	m11 = -s * m10 + c * m11;
	m10 = t;
	t = c * m20 + s * m21;
	m21 = -s * m20 + c * m21;
	m20 = t;
}

void Matrix33::scale( float xyz ){
	m00 *= xyz;
	m01 *= xyz;
	m02 *= xyz;
	m10 *= xyz;
	m11 *= xyz;
	m12 *= xyz;
	m20 *= xyz;
	m21 *= xyz;
	m22 *= xyz;
}

void Matrix33::scale( float x, float y, float z ){
	m00 *= x;
	m01 *= y;
	m02 *= z;
	m10 *= x;
	m11 *= y;
	m12 *= z;
	m20 *= x;
	m21 *= y;
	m22 *= z;
}

void Matrix33::scale( const Vector3& a ){
	m00 *= a.x;
	m01 *= a.y;
	m02 *= a.z;
	m10 *= a.x;
	m11 *= a.y;
	m12 *= a.z;
	m20 *= a.x;
	m21 *= a.y;
	m22 *= a.z;
}

void Matrix33::mul( Vector3* out, const Vector3& in ) const {
	float tx = in.x;
	float ty = in.y;
	out->x = m00 * tx + m01 * ty + m02 * in.z;
	out->y = m10 * tx + m11 * ty + m12 * in.z;
	out->z = m20 * tx + m21 * ty + m22 * in.z;
}

void Matrix33::operator*=( const Matrix33& a ){
	float tx = m00;
	float ty = m01;
	m00 = tx * a.m00 + ty * a.m10 + m02 * a.m20;
	m01 = tx * a.m01 + ty * a.m11 + m02 * a.m21;
	m02 = tx * a.m02 + ty * a.m12 + m02 * a.m22;
	tx = m10;
	ty = m11;
	m10 = tx * a.m00 + ty * a.m10 + m12 * a.m20;
	m11 = tx * a.m01 + ty * a.m11 + m12 * a.m21;
	m12 = tx * a.m02 + ty * a.m12 + m12 * a.m22;
	tx = m20;
	ty = m21;
	m20 = tx * a.m00 + ty * a.m10 + m22 * a.m20;
	m21 = tx * a.m01 + ty * a.m11 + m22 * a.m21;
	m22 = tx * a.m02 + ty * a.m12 + m22 * a.m22;
}

void Matrix33::setMul( const Matrix33& a, const Matrix33& b ){
	//a==*this, b==*this�������l���Ȃ��Ƃ����Ȃ�
	float t00, t01, t02;
	t00 = a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20;
	t01 = a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21;
	t02 = a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22;

	float t10, t11, t12;
	t10 = a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20;
	t11 = a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21;
	t12 = a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22;

	float t20, t21;
	t20 = a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20;
	t21 = a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21;
	m22 = a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22;

	m00 = t00; m01 = t01; m02 = t02;
	m10 = t10; m11 = t11; m12 = t12;
	m20 = t20; m21 = t21;
}

float& Matrix33::operator()( int r, int c ){
	ASSERT( r >= 0 && r < 3 );
	ASSERT( c >= 0 && c < 3 );
	return ( &m00 )[ 3 * r + c ];
}

float Matrix33::operator()( int r, int c ) const {
	ASSERT( r >= 0 && r < 3 );
	ASSERT( c >= 0 && c < 3 );
	return ( &m00 )[ 3 * r + c ];
}

void Matrix33::transpose(){
	float t;
	t = m01; m01 = m10; m10 = t;
	t = m02; m02 = m20; m20 = t;
	t = m12; m12 = m21; m21 = t;
}

void Matrix33::setTransposed( const Matrix33& a ){
	m00 = a.m00;
	m01 = a.m10;
	m02 = a.m20;
	m10 = a.m01;
	m11 = a.m11;
	m12 = a.m12;
	m20 = a.m21;
	m21 = a.m21;
	m22 = a.m22;
}

//�N���[�����@�ł̋t�s��
/*
2x2�̎��̂������g�����Ă݂悤�B
|x00 x01 x02| |a00 a01 a02| |1 0 0|
|x10 x11 x12|*|a10 a11 a12|=|0 1 0|
|x20 x21 x22| |a20 a21 a22| |0 0 1|

���l����B�������߂�t�s��A�E�͍�����s�񂾁B
���s���������

a00x00 + a10x01 + a20x02 = 1
a01x00 + a11x01 + a21x02 = 0
a02x00 + a12x01 + a22x02 = 0

������������Ă���2x2�̎��Ɠ����悤�ɋK�����̂���֌W���o�Ă���
�����͂���̂����A��������ǂ�����B
�����͐�l�̒m�b���؂�悤�B

�E�s��
�E�]���q

���̓�̌��t��m��Ȃ��l�͌�������Ȃ�{��ǂނȂ肵�Ăق����B
�����Ő�������͓̂ǂ݂ɂ����������̂��ʓ|���B

det(A)���s�񎮁AA_ij��i�sj��̗]���q�Ƃ���΁A
�t�s��A'��
        1   | A_00 A_10 A_20 |
A' = ------ | A_01 A_11 A_21 |
     det(A) | A_02 A_12 A_22 |

�Ə�����B�s�񂪓]�u����Ă��邱�Ƃɒ��ӁI

3x3�̍s�񎮂�

00*( 11*22 - 12*21 ) - 01*( 10*22 - 12*20 ) + 02*( 10*21 - 11*20 )

�ŏ���(�Y��������������)�A�]���q��i�sj��������Ăł���2x2�s��̍s�񎮂ɁA
���Ɏc�����s��
|a b|
|c d|
�Ȃ�΍s��ad-bc��i+j����Ȃ�-1���|���邱�ƂɂȂ�B

�Ⴆ��A_00��0�s0��������Ăł���s��
|11 12|
|21 22|
�ŁA�s�񎮂�11*22-12*21�ɂȂ�B�����9���Ƃ߂Ă��΂����Ƃ������ƂŁA
����Ă��΁A

A_00 = 11*22-21*12
A_01 = -(10*22-12*20)
A_02 = 10*21-11*20
A_10 = -(01*22-02*21)
A_11 = 00*22-02*20
A_12 = -(00*21-01*20)
A_20 = 01*12-02*11
A_21 = -(00*12-02*10)
A_22 = 00*11-01*10

�ƂȂ�B
*/
void Matrix33::setInverse( const Matrix33& a ){
	//�񍀐ς��܂Ƃ߂č��(�Y���������������ɕ��ׂĂ݂��̂ŊԈႢ�����Ȃ����Ǝv��)
	float m0011 = a.m00 * a.m11;
	float m0012 = a.m00 * a.m12;
	float m0021 = a.m00 * a.m21;
	float m0022 = a.m00 * a.m22;

	float m0110 = a.m01 * a.m10;
	float m0112 = a.m01 * a.m12;
	float m0120 = a.m01 * a.m20;
	float m0122 = a.m01 * a.m22;

	float m0210 = a.m02 * a.m10;
	float m0211 = a.m02 * a.m11;
	float m0220 = a.m02 * a.m20;
	float m0221 = a.m02 * a.m21;

	float m1021 = a.m10 * a.m21;
	float m1022 = a.m10 * a.m22;

	float m1120 = a.m11 * a.m20;
	float m1122 = a.m11 * a.m22;

	float m1220 = a.m12 * a.m20;
	float m1221 = a.m12 * a.m21; //18��

	//2���ς̍����O�B�s�񎮗p�����A��Ŏg���܂킹��B
	float m1122_m1221 = m1122 - m1221;
	float m1220_m1022 = m1220 - m1022; //�}�C�i�X
	float m1021_m1120 = m1021 - m1120;

	//�s��
	//00*( 11*22 - 12*21 ) - 01*( 10*22 - 12*20 ) + 02*( 10*21 - 11*20 )
	float delta = a.m00*( m1122_m1221 ) + a.m01*( m1220_m1022 ) + a.m02*( m1021_m1120 );
	float rcpDelta = 1.f / delta; //0������\���͂��邪�A�����ł͂͂����Ȃ��B

	//�v�f�𖄂߂�B�]�u�ɒ��ӁI���ӂ͓Y�������Ђ�����Ԃ��Ă���I
	m00 = ( m1122_m1221 ) * rcpDelta;
	m10 = ( m1220_m1022 ) * rcpDelta; //�}�C�i�X
	m20 = ( m1021_m1120 ) * rcpDelta;
	m01 = ( m0221-m0122 ) * rcpDelta; //�}�C�i�X
	m11 = ( m0022-m0220 ) * rcpDelta;
	m21 = ( m0120-m0021 ) * rcpDelta; //�}�C�i�X
	m02 = ( m0112-m0211 ) * rcpDelta;
	m12 = ( m0210-m0012 ) * rcpDelta; //�}�C�i�X
	m22 = ( m0011-m0110 ) * rcpDelta;
}

void Matrix33::invert(){
	setInverse( *this ); //setInverse()�������ł����v�ȍ��Ȃ̂ł��̂܂ܓn���B
}

bool Matrix33::operator==( const Matrix33& a ) const {
	for ( int i = 0; i < 3; ++i ){
		for ( int j = 0; j < 3; ++j ){
			if ( ( *this )( i, j ) != a( i, j ) ){
				return false;
			}
		}
	}
	return true;
}

bool Matrix33::operator!=( const Matrix33& a ) const {
	return !( operator==( a ) );
}


} //namespace Math
} //namespace GameLib
