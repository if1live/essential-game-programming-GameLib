#include "GameLib/Framework.h"
#include "GameLib/DebugScreen.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Math/Functions.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Graphics/VertexBuffer.h"
#include "GameLib/Graphics/IndexBuffer.h"
#include "GameLib/Graphics/Vertex.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
using namespace GameLib::Math;
using namespace GameLib::Graphics;

VertexBuffer gVertexBuffer;
IndexBuffer gIndexBuffer;
Texture gTexture;
int gCount;
GameLib::Graphics::LightingMode gLightingMode = GameLib::Graphics::LIGHTING_NONE;

namespace GameLib{
	//Configuration�N���X���g���ď����ݒ肵�Ă�낤�B
	void Framework::configure( Configuration* conf ){
		conf->setWidth( 640 );
		conf->setHeight( 480 );
		conf->forbidFullScreen( true );
		conf->enableFullScreen( false ); //�t���X�N���[�����[�h�N��
		conf->enableVSync( false ); //�n�[�h�E�F�A�ɂ��t���[�����[�g��艻
		conf->enableAntiAlias( false ); //�W���M�[���y�����鏈���̗L��
		conf->setTitle( "DrawCube Sample" ); //�^�C�g���o�[�̕�����
	}
	void Framework::update(){
		using namespace Graphics;
		//���x�ƂȂ��o�Ă���̂�m�ꕶ���ł��邱�Ƃ������ė~����
		Graphics::Manager m = Graphics::Manager::instance();
		if ( gCount == 0 ){ 
			m.setTextureFilter( TEXTURE_FILTER_POINT );
			//--------------���_�o�b�t�@�����-----------------------
			gVertexBuffer = VertexBuffer::create( 8 ); //8���_�̒��_�o�b�t�@
			Vertex* v = gVertexBuffer.lock(); //�������݃|�C���^�𓾂�
			//�ʒu������
			v[ 0 ].mPosition.set( -1.f, -1.f, -1.f );
			v[ 1 ].mPosition.set( -1.f, -1.f, 1.f );
			v[ 2 ].mPosition.set( -1.f, 1.f, -1.f );
			v[ 3 ].mPosition.set( -1.f, 1.f, 1.f );
			v[ 4 ].mPosition.set( 1.f, -1.f, -1.f );
			v[ 5 ].mPosition.set( 1.f, -1.f, 1.f );
			v[ 6 ].mPosition.set( 1.f, 1.f, -1.f );
			v[ 7 ].mPosition.set( 1.f, 1.f, 1.f );
			//�@��������B�΂߂ɏo�Ă���@���ɂ��Ă݂�B�����͂�������1
			v[ 0 ].mNormal.set( -0.57f, -0.57f, -0.57f );
			v[ 1 ].mNormal.set( -0.57f, -0.57f, 0.57f );
			v[ 2 ].mNormal.set( -0.57f, 0.57f, -0.57f );
			v[ 3 ].mNormal.set( -0.57f, 0.57f, 0.57f );
			v[ 4 ].mNormal.set( 0.57f, -0.57f, -0.57f );
			v[ 5 ].mNormal.set( 0.57f, -0.57f, 0.57f );
			v[ 6 ].mNormal.set( 0.57f, 0.57f, -0.57f );
			v[ 7 ].mNormal.set( 0.57f, 0.57f, 0.57f );
			//���_�J���[������(���C�e�B���O�̌��ʂ��������̂Ő^����
			for ( int i = 0; i < 8; ++i ){
				v[ i ].mColor = 0xffffffff;
			}
			//�e�N�X�`�����W������(�K��)
			v[ 0 ].mUv.set( 0.f, 0.f );
			v[ 1 ].mUv.set( 0.f, 1.f );
			v[ 2 ].mUv.set( 1.f, 0.f );
			v[ 3 ].mUv.set( 1.f, 1.f );
			v[ 4 ].mUv.set( 0.f, 0.f );
			v[ 5 ].mUv.set( 0.f, 1.f );
			v[ 6 ].mUv.set( 1.f, 0.f );
			v[ 7 ].mUv.set( 1.f, 1.f );
			//�����I����������
			gVertexBuffer.unlock( &v );

			//-------------�C���f�N�X�o�b�t�@�����--------------------
			//3�C���f�N�X�ŎO�p�`��Ȃ̂ŁA�C���f�N�X�o�b�t�@�̃T�C�Y�͕���3�̔{���ɂȂ�
			gIndexBuffer = IndexBuffer::create( 36 ); //36���_�̃C���f�N�X�o�b�t�@
			unsigned short indices[ 36 ] = {
				0, 1, 3, 3, 2, 0,
				4, 6, 7, 7, 5, 4,
				0, 4, 5, 5, 1, 0,
				2, 3, 7, 7, 6, 2,
				7, 3, 1, 1, 5, 7,
				2, 6, 4, 4, 0, 2,
			};
			unsigned short* iBuffer = gIndexBuffer.lock();
			for ( int i = 0; i < 36; ++i ){
				iBuffer[ i ] = indices[ i ];
			}
			gIndexBuffer.unlock( &iBuffer );

			//-------------�e�N�X�`�������---------------------
			gTexture = Texture::create( "robo.tga" );
			while ( !gTexture.isReady() ){
				; //���[�h�҂�
			}
		}
		//�g���f�[�^���w�肷��
		m.setVertexBuffer( gVertexBuffer );
		m.setIndexBuffer( gIndexBuffer );
		m.setTexture( gTexture );

		//�A���t�@�u�����h�ȂǂȂǂ�ݒ�
		m.setBlendMode( BLEND_LINEAR );
		m.setCullMode( CULL_BACK ); //����������
		m.enableDepthTest( true );
		m.enableDepthWrite( true );
		m.setLightingMode( gLightingMode );

		//���[���h�s���ݒ肷��
		Matrix34 wm;
		wm.setRotationY( gCount * 0.09f );
		wm.rotateX( gCount * 0.08f );
		wm.rotateZ( gCount * 0.07f );
		//�r���[�s���ݒ肷��
		Matrix34 vm;
		Vector3 eyePosition( 
			4.f * sin( gCount * 0.01f ),
			0.f,
			4.f * cos( gCount * 0.01f ) );
		Vector3 eyeTarget( 0.f, 0.f, 0.f );
		Vector3 up( 0.f, 1.f, 0.f ); //�u��v���w�肷��x�N�^�B������������ĉ��ɌX����B
		vm.setViewTransform( eyePosition, eyeTarget, up ); 

		//���e�s���ݒ肷��
		Matrix44 pm;
		pm.setPerspectiveTransform( 
			45.f, //45�x
			static_cast< float >( width() ),
			static_cast< float >( height() ),
			1.f,
			1000.f );
		
		//�s����n�[�h�E�F�A�ɓn��
		pm *= vm; //�����ϊ��~�r���[�ϊ�
		m.setProjectionViewMatrix( pm ); //�����ϊ��r���[�s��
		m.setWorldMatrix( wm ); //���[���h�s��

		//���C�e�B���O�̐ݒ������B�K���ɔ�щ����̋��̃C���[�W��
		Vector3 lightPositions[ 4 ];
		float lightIntensities[ 4 ];
		for ( int i = 0; i < 4; ++i ){
			//�܂��͈ʒu
			float phase = 90.f * static_cast< float >( i );
			float t = gCount * 0.025f + phase;
			lightPositions[ i ].x = 10.f * sin( t ) * sin( t );
			lightPositions[ i ].y = 10.f * sin( t ) * cos( t );
			lightPositions[ i ].z = 10.f * cos( t );
			lightIntensities[ i ] = 5.f;
		}
		//���̐F�͔��A�ԁA�΁A�ԁA�ɂ��Ă�낤
		Vector3 lightColors[ 4 ];
		lightColors[ 0 ].set( 1.f, 1.f, 1.f );
		lightColors[ 1 ].set( 1.f, 0.f, 0.f );
		lightColors[ 2 ].set( 0.f, 1.f, 0.f );
		lightColors[ 3 ].set( 0.f, 0.f, 1.f );
		//�A���r�G���g�͒n����0.3���炢��
		Vector3 ambient( 0.3f, 0.3f, 0.3f );
		//��͕��̑��̓����B���܂肶���Ă��d���Ȃ��̂ŌŒ�l�ŁB���낢�낢�����Č��ʂ��ǂ��Ȃ邩���Ă�낤
		Vector3 diffuseColor( 1.f, 1.f, 1.f );
		Vector3 specularColor( 1.f, 1.f, 1.f );
		//�����ς���ƃn�C���C�g�̉s�����ς��B�������قǂ��炴�炵���\�ʂƂ����C���[�W
		float specularSharpness = 100.f;
		//�J�����ʒu�̐ݒ�
		m.setEyePosition( eyePosition );
		//�ł̓��C�g�̏����Z�b�g���Ă�낤
		m.setAmbientColor( ambient );
		m.setDiffuseColor( diffuseColor );
		m.setSpecularColor( specularColor );
		m.setSpecularSharpness( specularSharpness );
		for ( int i = 0; i < 4; ++i ){
			m.setLightPosition( i, lightPositions[ i ] );
			m.setLightColor( i, lightColors[ i ] );
			m.setLightIntensity( i, lightIntensities[ i ] );
		}
		//��͕`�悷�邾����
		//�����͏��ɃC���f�N�X�o�b�t�@�̉��Ԗڂ���g�����A�����`�����A����`�����B
		m.drawIndexed( 0, 12, PRIMITIVE_TRIANGLE );
		//��̏ꍇ�A�u0�Ԗڂ���12�̎O�p�`��`���v�ƂȂ�B�g����C���f�N�X��36���B

		//�ȉ��̓X�y�[�X�L�[�Ń��C�e�B���O��؂�ւ���R�[�h�ŁA�ǂ��ł��ǂ��B
		if ( Input::Manager::instance().keyboard().isTriggered( ' ' ) ){
			if ( gLightingMode == LIGHTING_NONE ){
				gLightingMode = LIGHTING_PER_VERTEX;
			}else if ( gLightingMode == LIGHTING_PER_VERTEX ){
				gLightingMode = LIGHTING_PER_PIXEL;
			}else if ( gLightingMode == LIGHTING_PER_PIXEL ){
				gLightingMode = LIGHTING_NONE;
			}
		}
		DebugScreen ds = DebugScreen::instance();
		if ( gLightingMode == LIGHTING_NONE ){
			ds << "Lighting : NONE ( SPACE KEY TO CHANGE )";
		}else if ( gLightingMode == LIGHTING_PER_VERTEX ){
			ds << "Lighting : PER VERTEX ( SPACE KEY TO CHANGE )";
		}else if ( gLightingMode == LIGHTING_PER_PIXEL ){
			ds << "Lighting : PER PIXEL ( SPACE KEY TO CHANGE )";
		}
		++gCount;

		//��Еt�� 
		if ( isEndRequested() ){
			//�O���[�o���ɒu���Ə���ɂ͏����Ȃ��̂ŊJ��
			gTexture.release();
			gIndexBuffer.release();
			gVertexBuffer.release();
		}
	}
}
