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
	//xyz��a�{
	void operator*=( double );
	//�|���Z���Z�b�g
	void setMul( const Vector3&, double );
	//�����̓��Q�b�g
	double squareLength() const;
	//���όv�Z
	double dot( const Vector3& ) const;
	///this = crossProduct( a, b )
	void setCross( const Vector3& a, const Vector3& b );
	/// this = a * b + c
	void setMadd( const Vector3& a, double b, const Vector3& c );

	double x, y, z;
};

#endif
