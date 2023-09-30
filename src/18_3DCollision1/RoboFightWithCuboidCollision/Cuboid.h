#ifndef INCLUDED_CUBOID_H
#define INCLUDED_CUBOID_H

class Vector3;

class Cuboid{
public:
	bool isIntersect( const Cuboid& ) const;

	void setPosition( const Vector3& );
	void setHalfSize( const Vector3& );
	//�󂯎�����ړ��x�N�^�̐����̂�������ɓ�����Ȃ��œ����鐬���������c��
	void restrictMove( Vector3* moveVectorInOut, const Cuboid& ) const;
private:
	int mX;
	int mY;
	int mZ;
	int mHalfSizeX;
	int mHalfSizeY;
	int mHalfSizeZ;
};

#endif

