#ifndef INCLUDED_GAMELIB_GRAPHICS_MANAGER_H
#define INCLUDED_GAMELIB_GRAPHICS_MANAGER_H

#include "GameLib/Graphics/Enum.h"

namespace GameLib{

namespace Math{
	class Matrix44;
	class Matrix34;
	class Vector2;
	class Vector3;
}
using namespace Math;
	
namespace Graphics{

class Texture;
class VertexBuffer;
class IndexBuffer;

class Manager{
public:
	Manager();
	static Manager instance();

	void setVertexBuffer( VertexBuffer );
	void setIndexBuffer( IndexBuffer );
	void setTexture( Texture );

	void enableDepthTest( bool );
	void enableDepthWrite( bool );
	void setBlendMode( BlendMode );
	void setCullMode( CullMode );
	void setTextureFilter( TextureFilter );
	///�����ϊ��r���[�ϊ��s��Z�b�g�B
	void setProjectionViewMatrix( const Matrix44& );
	///���[���h�ϊ��s��Z�b�g
	void setWorldMatrix( const Matrix34& );
	///�r���[�|�[�g�ݒ�
	void setViewport( int x, int y, int width, int height );
	///�r���[�|�[�g�擾(0�łȂ����̂ɂ��������̂ŗ~�������̂����|�C���^���Z�b�g)
	void getViewport( int* xOut, int* yOut, int* widthOut, int* heightOut );

	///�ݒ肵���e�N�X�`���A���_�o�b�t�@�A�C���f�N�X�o�b�t�@�ŕ`��
	void drawIndexed(
		int offset, 
		int primitiveNumber, 
		PrimitiveType = PRIMITIVE_TRIANGLE );
	///�ݒ肵���e�N�X�`���A���_�o�b�t�@�ŕ`��
	void draw( 
		int offset, 
		int primitiveNumber, 
		PrimitiveType = PRIMITIVE_TRIANGLE );
	///�S��ʂɃe�N�X�`����\��t��
	void blendToScreen( Texture );
	///�t���X�N���[����
	void enableFullScreen( bool );
	///�`��o�b�t�@����Ԃ�
	int width() const;
	///�`��o�b�t�@������Ԃ�
	int height() const;
	///�t���X�N���[�����̃E�B���h�E����Ԃ�
	int fullScreenWindowWidth() const;
	///�t���X�N���[�����̃E�B���h�E������Ԃ�
	int fullScreenWindowHeight() const;
	///�t���[���ԍ���Ԃ�
	unsigned frameId() const;
	///�X�N���[���V���b�g�����(�g���q��DDS,TGA��������)�B�ۑ��^�C�~���O�͕`��I����B
	void captureScreen( const char* filename );

	//���C�e�B���O�֌W
	///�s�N�Z���V�F�[�_��L��������
	void setLightingMode( LightingMode );
	///���C�g�ʒu
	void setLightPosition( int index, const Vector3& position );
	///���C�g���x(100�Ȃ�100m�̋�����1�̖��邳�ɂȂ�B���xI�Ƃ��āA���邳�́uI/�����v�Ō��܂�)
	void setLightIntensity( int index, float intensity );
	///���C�g�F
	void setLightColor( int index, const Vector3& );
	///���_�ʒu
	void setEyePosition( const Vector3& position );
	///�f�B�t���[�Y�J���[
	void setDiffuseColor( const Vector3& );
	///�X�y�L�����F
	void setSpecularColor( const Vector3& );
	///�����F
	void setEmissionColor( const Vector3& );
	///�A���r�G���g�F
	void setAmbientColor( const Vector3& );
	///�����x�ݒ�
	void setTransparency( float );
	///�X�y�L�����̉s��(�傫���قǉs���B�����̈Ӗ��͕ʂ̖{��ǂ���)
	void setSpecularSharpness( float );

	//�ȉ��m��Ȃ��Ă�������
	static void create( 
		void* windowHandle,
		int width,
		int height,
		bool fullScreen,
		bool vSync,
		bool antiAlias );
	static void destroy();
	void beginDraw();
	void endDraw();
	bool canRender() const; //�f�o�C�X���X�g���Ă����false���Ԃ�B
	void restore(); //�f�o�C�X���X�g���畜�A����
	void getPointerModifier( float* scale, Vector2* offset ) const;
};

} //namespace Graphics
} //namespace GameLib

#endif
