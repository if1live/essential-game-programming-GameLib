#ifndef INCLUDED_VECTOR3_H
#define INCLUDED_VECTOR3_H

class Vector3{
public:
	//�������Ȃ��R���X�g���N�^
	Vector3();
	//double����R���X�g���N�^
	Vector3( double x, double y, double z );
	//Vector2����R���X�g���N�^
	Vector3( const Vector3& );
	//�ォ�琔�����Z�b�g
	void set( double x, double y, double z );
	//�����Z
	void operator+=( const Vector3& );
	//�����Z
	void operator-=( const Vector3& );
	//�R�s�[
	void operator=( const Vector3& );
	//�����Z���Z�b�g
	void setAdd( const Vector3&, const Vector3& );
	//�����Z���Z�b�g
	void setSub( const Vector3&, const Vector3& );

	double x, y, z;
};

#endif
