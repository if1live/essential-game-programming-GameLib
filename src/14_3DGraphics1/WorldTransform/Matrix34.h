#ifndef INCLUDED_MATRIX34_H
#define INCLUDED_MATRIX34_H

class Vector3;

class Matrix34{
public:
	Matrix34(); //�f�t�H���g�ŃR���X�g���N�g
	//�ړ��s�񐶐�
	void setTranslation( const Vector3& );
	//�g��k���s�񐶐�
	void setScaling( const Vector3& );
	//�x�N�^�Ɗ|���Z
	void multiply( Vector3* out, const Vector3& in ) const;
	//�s��Ɗ|���Z
	void operator*=( const Matrix34& );
	//�ړ��s��Ə�Z
	void translate( const Vector3& );
	//�g��k���s��Ə�Z
	void scale( const Vector3& );
private:
	double m00, m01, m02, m03;
	double m10, m11, m12, m13;
	double m20, m21, m22, m23;
};

#endif
