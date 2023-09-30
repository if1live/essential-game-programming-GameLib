#ifndef INCLUDED_MATRIX22_H
#define INCLUDED_MATRIX22_H

class Vector2;

class Matrix22{
public:
	//4�v�f�^���ăR���X�g���N�g
	Matrix22( double e00, double e01, double e10, double e11 );
	//�x�N�^�Ɗ|���Z
	void multiply( Vector2* out, const Vector2& in ) const;
private:
	double m00, m01, m10, m11;
};

#endif
