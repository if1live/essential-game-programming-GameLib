#include "Sphere.h"
#include "Library/Vector3.h"


void Sphere::setPosition( const Vector3& a ){
	mPosition = a;
}

void Sphere::setRadius( double a ){
	mRadius = a;
}

bool Sphere::isIntersect( const Sphere& b ) const {
	Vector3 difference;
	difference.setSub( mPosition, b.mPosition );
	double rSum = mRadius + b.mRadius;
	double squareDistance = difference.squareLength();
	rSum *= rSum;
	return ( squareDistance < rSum );
}

void Sphere::restrictMove( Vector3* v, const Sphere& a ) const {
	//�R�s�[����
	Sphere t = *this;
	t.mPosition += *v; //�ړ�
	//�ړ���Ŕ��肵�āA�������ĂȂ���΂��������B
	bool r = t.isIntersect( a );
	if ( !r ){
		return;
	}
	//�������Ă���Ȃ�x�N�^���C������B
	//b = (c0-c1)/|c0-c1|
	//a = v - dot( b, v ) * b
	//b������1�̏Փː��������x�N�^�Ba�͌��ʂƂ��ė~�������������x�N�^�B
	//�����ŁA��̎������̎��ɂ��̂܂ܓ�����
	//a = v - dot( (c0-c1)/|c0-c1|, v ) / |c0-c1| * b
	//�ƂȂ�Bc0-c1��d�Ƃ���΁A
	//a = v - dot( d, v ) * d / |d|^2
	//�ƂȂ�A���v�Z���ȒP�ɂȂ�B
	Vector3 d;
	d.setSub( mPosition, a.mPosition ); //c0-c1 = d
	double dotDV = d.dot( *v ); //dot(d, v)
	double dLength2 = d.squareLength(); //|d|^2
	d *= ( dotDV / dLength2 ); //dot(b,v) / |d|^2
	//�Ō�̈����Z
	*v -= d;
}
