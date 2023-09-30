#ifndef INCLUDED_VECTOR2_H
#define INCLUDED_VECTOR2_H

class Vector2{
public:
	//�������Ȃ��R���X�g���N�^
	Vector2();
	//��������R���X�g���N�^
	Vector2( int x, int y );
	//double����R���X�g���N�^
	Vector2( double x, double y );
	//Vector2����R���X�g���N�^
	Vector2( const Vector2& );
	//�����Z
	void operator+=( const Vector2& );
	//�����Z
	void operator-=( const Vector2& );
	//�R�s�[
	void operator=( const Vector2& );
	//�����Z���Z�b�g
	void setAdd( const Vector2&, const Vector2& );
	//�����Z���Z�b�g
	void setSub( const Vector2&, const Vector2& );
	//��Ԃ��Z�b�g( this = a + u*ab + v*ac )
	void setInterporation( 
		const Vector2& a, 
		const Vector2& ab, 
		const Vector2& ac, 
		double u, 
		double v );

	double x;
	double y;
};

#endif
