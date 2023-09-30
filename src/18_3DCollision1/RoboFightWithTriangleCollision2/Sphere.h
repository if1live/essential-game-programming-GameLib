#ifndef INCLUDED_SPHERE_H
#define INCLUDED_SPHERE_H

#include "Library/Vector3.h"

class Sphere{
public:
	bool isIntersect( const Sphere& ) const;

	void setPosition( const Vector3& );
	void setRadius( double );
	//�󂯎�����ړ��x�N�^�̐����̂�������ɓ�����Ȃ��œ����鐬���������c��
	void restrictMove( Vector3* moveVectorInOut, const Sphere& ) const;
private:
	Vector3 mPosition;
	double mRadius;
};

#endif

