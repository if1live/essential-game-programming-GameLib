#ifndef INCLUDED_GAMELIB_SCENE_PRIMITIVERENDERER_H
#define INCLUDED_GAMELIB_SCENE_PRIMITIVERENDERER_H

#include "GameLib/Graphics/Enum.h"

namespace GameLib{

namespace Graphics{
	class Texture;
}

namespace Math{
	class Matrix44;
	class Vector4;
	class Vector3;
	class Vector2;
}
using namespace Math;

namespace Scene{

///���ڎO�p�`�A�����A�_�A2D�����`��`�悷�邽�߂֗̕��N���X�B���x�I�ɂ͂��Ȃ薳�ʂ������B
class PrimitiveRenderer{
public:
	///���_���ƁA���̑����߂̐����w�肵�č\�z
	static PrimitiveRenderer create( 
		int vertexCapacity, 
		int commandCapacity );

	void setTexture( Graphics::Texture );
	void enableDepthTest( bool );
	void enableDepthWrite( bool );
	void setBlendMode( Graphics::BlendMode );
	void setCullMode( Graphics::CullMode );
	void setTransform( const Matrix44& );

	///�O�p�`��ϊ��ςݒ��_�ŏ����B�܂�p��4�v�f�x�N�^�B
	void addTransformedTriangle(
		const Vector4& pos0,
		const Vector4& pos1,
		const Vector4& pos2,
		const Vector2& uv0,
		const Vector2& uv1,
		const Vector2& uv2,
		unsigned color0 = 0xffffffff,
		unsigned color1 = 0xffffffff,
		unsigned color2 = 0xffffffff);
	///�O�p�`��ϊ��ςݒ��_�ŏ����B�e�N�X�`���Ȃ��B
	void addTransformedTriangle(
		const Vector4& pos0,
		const Vector4& pos1,
		const Vector4& pos2,
		unsigned color0 = 0xffffffff,
		unsigned color1 = 0xffffffff,
		unsigned color2 = 0xffffffff);
	///�O�p�`��`��
	void addTriangle(
		const Vector3& pos0,
		const Vector3& pos1,
		const Vector3& pos2,
		const Vector2& uv0,
		const Vector2& uv1,
		const Vector2& uv2,
		unsigned color0 = 0xffffffff,
		unsigned color1 = 0xffffffff,
		unsigned color2 = 0xffffffff );
	///�O�p�`��`���B�e�N�X�`���Ȃ�
	void addTriangle(
		const Vector3& pos0,
		const Vector3& pos1,
		const Vector3& pos2,
		unsigned color0 = 0xffffffff,
		unsigned color1 = 0xffffffff,
		unsigned color2 = 0xffffffff );
	///������`���B
	void addLine(
		const Vector3& pos0,
		const Vector3& pos1,
		const Vector2& uv0,
		const Vector2& uv1,
		unsigned color0 = 0xffffffff,
		unsigned color1 = 0xffffffff );
	///������`���B�e�N�X�`���Ȃ��B
	void addLine(
		const Vector3& pos0,
		const Vector3& pos1,
		unsigned color0 = 0xffffffff,
		unsigned color1 = 0xffffffff );
	///�_��`���B
	void addPoint(
		const Vector3& pos,
		const Vector2& uv,
		unsigned color = 0xffffffff );
	///�_��`���B�e�N�X�`���Ȃ�
	void addPoint(
		const Vector3& pos,
		unsigned color = 0xffffffff );
	///Z=0���ʂɒ����`��`���B���W�n�̓X�N���[�����W�B
	void addRectangle(
		const Vector2& pos0,
		const Vector2& pos1,
		const Vector2& uv0,
		const Vector2& uv1,
		unsigned color = 0xffffffff,
		float depth = 0.f );
	///Z=0���ʂɒ����`��`���B���W�n�̓X�N���[�����W�B�e�N�X�`���Ȃ��B
	void addRectangle(
		const Vector2& pos0,
		const Vector2& pos1,
		unsigned color = 0xffffffff,
		float depth = 0.f );
	///�`��
	void draw();

	//�ȉ����[�U�͈ӎ����Ȃ�
	PrimitiveRenderer();
	PrimitiveRenderer( const PrimitiveRenderer& );
	~PrimitiveRenderer();
	operator void*() const;
	PrimitiveRenderer& operator=( const PrimitiveRenderer& );
	void release();
	bool operator==( const PrimitiveRenderer& ) const;
	bool operator!=( const PrimitiveRenderer& ) const;
private:
	class Impl;
	Impl* mImpl;
};

} //namespace Scene

} //namespace GameLib


#endif
