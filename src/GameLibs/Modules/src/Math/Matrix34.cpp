#include "GameLib/GameLib.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Matrix33.h"
#include "GameLib/Math/Functions.h"
#include "GameLib/Math/Vector3.h"

namespace GameLib{
namespace Math{

Matrix34::Matrix34(){
}

void Matrix34::setIdentity(){
	m00 = m11 = m22 = 1.f;
	m01 = m02 = m03 = 0.f;
	m10 = m12 = m13 = 0.f;
	m20 = m21 = m23 = 0.f;
}

void Matrix34::setTranslation( float x, float y, float z ){
	m00 = m11 = m22 = 1.f;
	m03 = x;
	m13 = y;
	m23 = z;
	m01 = m02 = m10 = m12 = m20 = m21 = 0.f;
}

void Matrix34::setTranslation( const Vector3& a ){
	m00 = m11 = m22 = 1.f;
	m03 = a.x;
	m13 = a.y;
	m23 = a.z;
	m01 = m02 = m10 = m12 = m20 = m21 = 0.f;
}

void Matrix34::setRotationX( float r ){
	float c = cos( r );
	float s = sin( r );
	m11 = m22 = c;
	m12 = -s;
	m21 = s;
	m00 = 1.0;
	m01 = m02 = m10 = m20 = 0.0;
	m03 = m13 = m23 = 0.f;
}

void Matrix34::setRotationY( float r ){
	float c = cos( r );
	float s = sin( r );
	m00 = m22 = c;
	m20 = -s;
	m02 = s;
	m11 = 1.0;
	m01 = m10 = m12 = m21 = 0.0;
	m03 = m13 = m23 = 0.f;
}

void Matrix34::setRotationZ( float r ){
	float c = cos( r );
	float s = sin( r );
	m00 = m11 = c;
	m01 = -s;
	m10 = s;
	m22 = 1.0;
	m02 = m12 = m20 = m21 = 0.0;
	m03 = m13 = m23 = 0.f;
}

void Matrix34::setScaling( float xyz ){
	m00 = m11 = m22 = xyz;
	m01 = m02 = m03 = 0.f;
	m10 = m12 = m13 = 0.f;
	m20 = m21 = m23 = 0.f;
}

void Matrix34::setScaling( float x, float y, float z ){
	m00 = x;
	m11 = y;
	m22 = z;
	m01 = m02 = m03 = 0.f;
	m10 = m12 = m13 = 0.f;
	m20 = m21 = m23 = 0.f;
}

void Matrix34::setScaling( const Vector3& a ){
	m00 = a.x;
	m11 = a.y;
	m22 = a.z;
	m01 = m02 = m03 = 0.f;
	m10 = m12 = m13 = 0.f;
	m20 = m21 = m23 = 0.f;
}

/*
3����]�r���[�s��̍�肩���B

��]������3x3�s��ł���B����āA����9����B
����9�{����΂�������߂���킯���B

���āA���̎����ǂ������邩���B
����x�N�^s��s'�Ɉڂ�Ƃ킩���Ă���Ƃ��悤�B
���߂�����]�s���A�Ƃ���΁A

s'=As

�ł���B������΂点�΁A

s'x = a00*sx+a01*sy+a02*sz
s'y = a10*sx+a11*sy+a12*sz
s'z = a20*sx+a21*sy+a22*sz

�Ǝ���3�{�ł���Bs�ȊO��t��r�ɂ��Ă��A
t'=At
r'=Ar
�Ƃ킩���Ă���΁A����Ɏ���6�{������̂ŁA
9�{�̎������B

�����ŁA�v�Z��P�������邽�߂ɁA
s=(1,0,0)
t=(0,1,0)
r=(0,0,1)
�Ƃ��Ă݂悤�B��̎��̓A�z�݂����ɊȒP�ɂȂ��āA

s'x = a00
s'y = a10
s'z = a20

�ƂȂ�Bt,r�ɂ��Ă������悤�Ȃ��ƂɂȂ�A����A��
    | s'x t'x r'x |
A = | s'y t'y r'y |
    | s'z t'z r'z |

�Ə����Ă��܂��킯�ł���B���Ƃ́A
s'�At'�Ar'���킩��΂����B

��z�v���X�����͒����_���王�_�֌������x�N�^�ŁA
r' = ( p - t ) / | p - t |
�ƂȂ�B������1�ɂ��Ă��邱�Ƃɒ��ӂ��悤�B

�����Ă�����A�オ�ǂ��炩��u���^�����邱�Ƃł킩���Ă���B
���̂܂�Y���Ƃ��邱�Ƃ͂ł��Ȃ����A
u��r'�̊O�ς�����X�����o�Ă���B���ꂪs'���B
�����āAs'��r'�̊O�ς�����߂�Y�����o���B���ꂪt'�ƂȂ�B
���̃R�[�h�ɂ����āAx,y,z�͂��ꂼ��s',t',r'�ł���B

��]�s�񂳂��ł��Ă��܂��΁A��͈ړ������v�Z���邾�����B
V=RT�ŁAR�͍�������BT��p�����_�ɗ���悤�Ȉړ��s��ł���B
���̃R�[�h�ł͍s��̏�Z���ہX���v�Z�ʂ�����邽�߂ɁA
RT�̊|���Z�̕K�v�ȕ������������̏�ōs���āAm03,m13,m23���v�Z���Ă���B
*/
void Matrix34::setViewTransform(
const Vector3& p, //���_
const Vector3& t, //�����_
const Vector3& u ){ //���\���x�N�^
	Vector3 x, y, z;
	z.setSub( p, t ); //�����_����J�����ʒu�ւ̃x�N�^��Z����
	z.normalize();
	x.setCross( u, z ); //��x�N�^��Z���̊O�ς�X���ɁB��x�N�^��������Ȃ獶������X���v���X�B
	x.normalize();
	y.setCross( z, x ); //Z,X�̊O�ς�Y

	//��������3�{�s�����œ����Ώo���オ��B
	m00 = x.x; m01 = x.y; m02 = x.z;
	m10 = y.x; m11 = y.y; m12 = y.z;
	m20 = z.x; m21 = z.y; m22 = z.z;
	//�ړ����͍��������]�s��ɃJ�������W����Z���ă}�C�i�X�ɂ���΂����B
	m03 = -( m00 * p.x + m01 * p.y + m02 * p.z );
	m13 = -( m10 * p.x + m11 * p.y + m12 * p.z );
	m23 = -( m20 * p.x + m21 * p.y + m22 * p.z );
}

void Matrix34::rotateX( float r ){
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

void Matrix34::rotateY( float r ){
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

void Matrix34::rotateZ( float r ){
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

void Matrix34::scale( float xyz ){
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

void Matrix34::scale( float x, float y, float z ){
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

void Matrix34::scale( const Vector3& a ){
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

void Matrix34::translate( float x, float y, float z ){
	m03 += m00 * x + m01 * y + m02 * z;
	m13 += m10 * x + m11 * y + m12 * z;
	m23 += m20 * x + m21 * y + m22 * z;
}

void Matrix34::translate( const Vector3& a ){
	m03 += m00 * a.x + m01 * a.y + m02 * a.z;
	m13 += m10 * a.x + m11 * a.y + m12 * a.z;
	m23 += m20 * a.x + m21 * a.y + m22 * a.z;
}


void Matrix34::mul( Vector3* out, const Vector3& in ) const {
	float tx = in.x;
	float ty = in.y;
	out->x = m00 * tx + m01 * ty + m02 * in.z + m03;
	out->y = m10 * tx + m11 * ty + m12 * in.z + m13;
	out->z = m20 * tx + m21 * ty + m22 * in.z + m23;
}

void Matrix34::mul33( Vector3* out, const Vector3& in ) const {
	float tx = in.x;
	float ty = in.y;
	out->x = m00 * tx + m01 * ty + m02 * in.z;
	out->y = m10 * tx + m11 * ty + m12 * in.z;
	out->z = m20 * tx + m21 * ty + m22 * in.z;
}

void Matrix34::operator*=( const Matrix34& a ){
	float tx = m00;
	float ty = m01;
	float tz = m02;
	m00 = tx * a.m00 + ty * a.m10 + tz * a.m20;
	m01 = tx * a.m01 + ty * a.m11 + tz * a.m21;
	m02 = tx * a.m02 + ty * a.m12 + tz * a.m22;
	m03 += tx * a.m03 + ty * a.m13 + tz * a.m23;
	tx = m10;
	ty = m11;
	tz = m12;
	m10 = tx * a.m00 + ty * a.m10 + tz * a.m20;
	m11 = tx * a.m01 + ty * a.m11 + tz * a.m21;
	m12 = tx * a.m02 + ty * a.m12 + tz * a.m22;
	m13 += tx * a.m03 + ty * a.m13 + tz * a.m23;
	tx = m20;
	ty = m21;
	tz = m22;
	m20 = tx * a.m00 + ty * a.m10 + tz * a.m20;
	m21 = tx * a.m01 + ty * a.m11 + tz * a.m21;
	m22 = tx * a.m02 + ty * a.m12 + tz * a.m22;
	m23 += tx * a.m03 + ty * a.m13 + tz * a.m23;
}

void Matrix34::operator*=( const Matrix33& a ){
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

void Matrix34::setMul( const Matrix34& a, const Matrix34& b ){
	//a==*this, b==*this�������l���Ȃ��Ƃ����Ȃ�
	float t00, t01, t02, t03;
	t00 = a.m00 * b.m00 + a.m01 * b.m10 + a.m02 * b.m20;
	t01 = a.m00 * b.m01 + a.m01 * b.m11 + a.m02 * b.m21;
	t02 = a.m00 * b.m02 + a.m01 * b.m12 + a.m02 * b.m22;
	t03 = a.m00 * b.m03 + a.m01 * b.m13 + a.m02 * b.m23 + a.m03;

	float t10, t11, t12, t13;
	t10 = a.m10 * b.m00 + a.m11 * b.m10 + a.m12 * b.m20;
	t11 = a.m10 * b.m01 + a.m11 * b.m11 + a.m12 * b.m21;
	t12 = a.m10 * b.m02 + a.m11 * b.m12 + a.m12 * b.m22;
	t13 = a.m10 * b.m03 + a.m11 * b.m13 + a.m12 * b.m23 + a.m13;

	float t20, t21, t22;
	t20 = a.m20 * b.m00 + a.m21 * b.m10 + a.m22 * b.m20;
	t21 = a.m20 * b.m01 + a.m21 * b.m11 + a.m22 * b.m21;
	t22 = a.m20 * b.m02 + a.m21 * b.m12 + a.m22 * b.m22;
	m23 = a.m20 * b.m03 + a.m21 * b.m13 + a.m22 * b.m23 + a.m23;

	m00 = t00; m01 = t01; m02 = t02; m03 = t03;
	m10 = t10; m11 = t11; m12 = t12; m13 = t13;
	m20 = t20; m21 = t21; m22 = t22;
}

void Matrix34::setMul( const Matrix34& a, const Matrix33& b ){
	float tx = a.m00;
	float ty = a.m01;
	m00 = tx * b.m00 + ty * b.m10 + a.m02 * b.m20;
	m01 = tx * b.m01 + ty * b.m11 + a.m02 * b.m21;
	m02 = tx * b.m02 + ty * b.m12 + a.m02 * b.m22;
	m03 = a.m03;
	tx = a.m10;
	ty = a.m11;
	m10 = tx * b.m00 + ty * b.m10 + a.m12 * b.m20;
	m11 = tx * b.m01 + ty * b.m11 + a.m12 * b.m21;
	m12 = tx * b.m02 + ty * b.m12 + a.m12 * b.m22;
	m13 = a.m13;
	tx = a.m20;
	ty = a.m21;
	m20 = tx * b.m00 + ty * b.m10 + a.m22 * b.m20;
	m21 = tx * b.m01 + ty * b.m11 + a.m22 * b.m21;
	m22 = tx * b.m02 + ty * b.m12 + a.m22 * b.m22;
	m23 = a.m23;
}

float& Matrix34::operator()( int r, int c ){
	ASSERT( r >= 0 && r < 3 );
	ASSERT( c >= 0 && c < 4 );
	return ( &m00 )[ 4 * r + c ];
}

float Matrix34::operator()( int r, int c ) const {
	ASSERT( r >= 0 && r < 3 );
	ASSERT( c >= 0 && c < 4 );
	return ( &m00 )[ 4 * r + c ];
}

void Matrix34::transpose33(){
	float t;
	t = m01; m01 = m10; m10 = t;
	t = m02; m02 = m20; m20 = t;
	t = m12; m12 = m21; m21 = t;
	m03 = m13 = m23 = 0.f;
}

void Matrix34::setTransposed33( const Matrix34& a ){
	m00 = a.m00;
	m01 = a.m10;
	m02 = a.m20;
	m10 = a.m01;
	m11 = a.m11;
	m12 = a.m12;
	m20 = a.m21;
	m21 = a.m21;
	m22 = a.m22;
	m03 = m13 = m23 = 0.f;
}

//�N���[�����@�ł̋t�s��
/*
33�̂����ɁA23�ɏ������l������K�p����ƁA�������������B
Matrix33.cpp��Matrix23.cpp�����悤�B
*/
void Matrix34::setInverse( const Matrix34& a ){
	//�܂�3x3�����̋t�s������
//--3x3���炱�҂�
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
//----�����܂ł��҂�
	float t03 = a.m03; //�o�b�N�A�b�v
	float t13 = a.m13;
	m03 = -( m00 * t03 + m01 * t13 + m02 * a.m23 );
	m13 = -( m10 * t03 + m11 * t13 + m12 * a.m23 );
	m23 = -( m20 * t03 + m21 * t13 + m22 * a.m23 );
}

void Matrix34::invert(){
	setInverse( *this ); //setInverse()�������ł����v�ȍ��Ȃ̂ł��̂܂ܓn���B
}

bool Matrix34::operator==( const Matrix34& a ) const {
	for ( int i = 0; i < 3; ++i ){
		for ( int j = 0; j < 4; ++j ){
			if ( ( *this )( i, j ) != a( i, j ) ){
				return false;
			}
		}
	}
	return true;
}

bool Matrix34::operator!=( const Matrix34& a ) const {
	return !( operator==( a ) );
}




} //namespace Math
} //namespace GameLib
