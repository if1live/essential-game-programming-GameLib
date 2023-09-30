#include "Matrix34.h"
#include "Vector3.h"
#include "GameLib/Math.h"
using namespace GameLib;

Matrix34::Matrix34(){
}

void Matrix34::setTranslation( const Vector3& a ){
	m00 = m11 = m22 = 1.0;
	m01 = m10 = m20 = m02 = m12 = m21 = 0.0;
	m03 = a.x;
	m13 = a.y;
	m23 = a.z;
}

void Matrix34::setScaling( const Vector3& a ){
	m00 = a.x;
	m11 = a.y;
	m22 = a.z;
	m01 = m10 = m02 = m20 = m12 = m21 = 0.0;
	m03 = m13 = m23 = 0.0;
}

void Matrix34::setRotationX( double r ){
	double c = cos( r );
	double s = sin( r );
	m11 = m22 = c;
	m12 = -s;
	m21 = s;
	m00 = 1.0;
	m01 = m02 = m10 = m20 = 0.0;
	m03 = m13 = m23 = 0.0;
}

void Matrix34::setRotationY( double r ){
	double c = cos( r );
	double s = sin( r );
	m00 = m22 = c;
	m20 = -s;
	m02 = s;
	m11 = 1.0;
	m01 = m10 = m12 = m21 = 0.0;
	m03 = m13 = m23 = 0.0;
}

void Matrix34::setRotationZ( double r ){
	double c = cos( r );
	double s = sin( r );
	m00 = m11 = c;
	m01 = -s;
	m10 = s;
	m22 = 1.0;
	m02 = m12 = m20 = m21 = 0.0;
	m03 = m13 = m23 = 0.0;
}

void Matrix34::operator*=( const Matrix34& m ){
	double tx, ty, tz;
	tx = m00; ty = m01; tz = m02;
	m00 = tx * m.m00 + ty * m.m10 + tz * m.m20;
	m01 = tx * m.m01 + ty * m.m11 + tz * m.m21;
	m02 = tx * m.m02 + ty * m.m12 + tz * m.m22;
	m03 = tx * m.m03 + ty * m.m13 + tz * m.m23 + m03;
	tx = m10; ty = m11; tz = m12;
	m10 = tx * m.m00 + ty * m.m10 + tz * m.m20;
	m11 = tx * m.m01 + ty * m.m11 + tz * m.m21;
	m12 = tx * m.m02 + ty * m.m12 + tz * m.m22;
	m13 = tx * m.m03 + ty * m.m13 + tz * m.m23 + m13;
	tx = m20; ty = m21; tz = m22;
	m20 = tx * m.m00 + ty * m.m10 + tz * m.m20;
	m21 = tx * m.m01 + ty * m.m11 + tz * m.m21;
	m22 = tx * m.m02 + ty * m.m12 + tz * m.m22;
	m23 = tx * m.m03 + ty * m.m13 + tz * m.m23 + m23;
}

void Matrix34::multiply( Vector3* out, const Vector3& in ) const {
	//out��in��������������Ȃ��̂ŁA�o�b�N�A�b�v
	double tx = in.x; 
	double ty = in.y; 
	out->x = m00 * tx + m01 * ty + m02 * in.z + m03;
	out->y = m10 * tx + m11 * ty + m12 * in.z + m13;
	out->z = m20 * tx + m21 * ty + m22 * in.z + m23;
}

//���ʂȂ��ړ��s�����Z�B2D�̎��̂��Q�l�ɂ��Ď��͂ōl���悤�B
void Matrix34::translate( const Vector3& a ){
	m03 += m00 * a.x + m01 * a.y + m02 * a.z;
	m13 += m10 * a.x + m11 * a.y + m12 * a.z;
	m23 += m20 * a.x + m21 * a.y + m22 * a.z;
}

//���ʂȂ��g��k���s�����Z�B2D�̎��̂��Q�l�ɂ��Ď��͂ōl���悤�B
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

//���ʂȂ�X��]�s�����Z 2,3�񂵂��ς��Ȃ��B
void Matrix34::rotateX( double r ){
	double c = cos( r );
	double s = sin( r );
	double t;
	//1�s��
	t = c * m01 + s * m02;
	m02 = -s * m01 + c * m02;
	m01 = t;
	//2�s��
	t = c * m11 + s * m12;
	m12 = -s * m11 + c * m12;
	m11 = t;
	//3�s��
	t = c * m21 + s * m22;
	m22 = -s * m21 + c * m22;
	m21 = t;
}

//���ʂȂ�Y��]�s�����Z 1,3�񂵂��ς��Ȃ��B
void Matrix34::rotateY( double r ){
	double c = cos( r );
	double s = sin( r );
	double t;
	//1�s��
	t = c * m00 - s * m02;
	m02 = s * m00 + c * m02;
	m00 = t;
	//2�s��
	t = c * m10 - s * m12;
	m12 = s * m10 + c * m12;
	m10 = t;
	//3�s��
	t = c * m20 - s * m22;
	m22 = s * m20 + c * m22;
	m20 = t;
}

//���ʂȂ�Z��]�s�����Z 1,2�񂵂��ς��Ȃ��B
void Matrix34::rotateZ( double r ){
	double c = cos( r );
	double s = sin( r );
	double t;
	//1�s��
	t = c * m00 + s * m01;
	m01 = -s * m00 + c * m01;
	m00 = t;
	//2�s��
	t = c * m10 + s * m11;
	m11 = -s * m10 + c * m11;
	m10 = t;
	//1�s��
	t = c * m20 + s * m21;
	m21 = -s * m20 + c * m21;
	m20 = t;
}

