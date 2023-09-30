#ifndef INCLUDED_TRIANGLE_H
#define INCLUDED_TRIANGLE_H

#include "Library/Vector3.h"

class Triangle{
public:
	//�����鎞�������߂�
	double getIntersectionTime( const Vector3& lineBegin, const Vector3& lineVector ) const;
	//�n�_�Ɛ����x�N�^
	bool isIntersect( const Vector3& lineBegin, const Vector3& lineVector ) const;
	//�󂯎�����ړ��x�N�^�̐����̂�������ɓ�����Ȃ��œ����鐬���������c��
	bool restrictMove( Vector3* moveVectorInOut, const Vector3& position ) const;
	//�󂯎�����ړ��x�N�^����@������������
	void modifyMove( Vector3* moveVectorInOut ) const;
	//�ʒu�ݒ�
	void setPosition( const Vector3& p0, const Vector3& p1, const Vector3& p2 );
private:
	Vector3 mPosition[ 3 ]; //3���_
};

#endif

