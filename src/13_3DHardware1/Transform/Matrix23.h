#ifndef INCLUDED_MATRIX23_H
#define INCLUDED_MATRIX23_H

class Vector2;

class Matrix23{
public:
	Matrix23(); //�f�t�H���g�ŃR���X�g���N�g
	//�ړ��s�񐶐�
	void setTranslation( const Vector2& );
	//��]�s�񐶐�
	void setRotation( double radian );
	//�g��k���s�񐶐�
	void setScaling( const Vector2& );
	//�x�N�^�Ɗ|���Z
	void multiply( Vector2* out, const Vector2& in ) const;
	//�s��Ɗ|���Z
	void operator*=( const Matrix23& );
	//�ړ��s�������ĉE���炩����
	void translate( const Vector2& );
	//��]�s�������ĉE���炩����
	void rotate( double radian );
	//�g��k���s�������ĉE���炩����
	void scale( const Vector2& );
private:
	double m00, m01, m02;
	double m10, m11, m12;
};

#endif
