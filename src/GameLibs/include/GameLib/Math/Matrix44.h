#ifndef INCLUDED_GAMELIB_MATH_MATRIX44_H
#define INCLUDED_GAMELIB_MATH_MATRIX44_H

namespace GameLib{
namespace Math{

class Vector3;
class Vector4;
class Matrix34;

/*!
44�͕ϊ��̍ŏI�i�ɂ����g��Ȃ����Ƃ�z�肵�Ă���B������@�\�̓M���M�������Ȃ��B
34�łł��邱�Ƃ�34�ł�邱�ƁB�l�͍s��̏�Z�͏d������ł��邱�Ƃ����΂��ΖY���B
*/
class Matrix44{
public:
	Matrix44();
	void setIdentity();
	//�����ϊ��s��̐���
	void setPerspectiveTransform( 
		float fieldOfViewYInRadian,
		float windowWidth,
		float windowHeight,
		float nearClip,
		float farClip,
		bool zBufferIsLinear = false );
	//���ˉe�s��̐���
	void setOrthogonalTransform(
		float left,
		float right,
		float bottom,
		float top,
		float front,
		float back );
	//view�s�������Ă��̏�Ŋ|���Z����
	void multiplyViewTransform(
		const Vector3& eyePosition,
		const Vector3& eyeTarget,
		const Vector3& upVector );

	void mul( Vector4* out, const Vector3& in ) const;
	void operator*=( const Matrix34& );
	void setMul( const Matrix44&, const Matrix34& );
	///���S�Ɉ�v���Ă��邩�`�F�b�N
	bool operator==( const Matrix44& ) const;
	///�����ł��Ⴄ�̂����`�F�b�N
	bool operator!=( const Matrix44& ) const;

	float& operator()( int row, int col );
	float operator()( int row, int col ) const;

	float m00, m01, m02, m03;
	float m10, m11, m12, m13;
	float m20, m21, m22, m23;
	float m30, m31, m32, m33;
};

} //namespace Math
} //namespace GameLib

#endif
