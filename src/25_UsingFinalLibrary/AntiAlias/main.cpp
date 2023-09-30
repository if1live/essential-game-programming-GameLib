//--------�}���`�T���v���A���`�G�C���A�X�X�C�b�`---
bool gMultiSampleAntiAlias = true; //��������������Ă݂悤

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
#include "GameLib/Graphics/Vertex.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/WindowCreator/WindowCreator.h"
using namespace GameLib::Math;
using namespace GameLib::Graphics;

VertexBuffer gVertexBuffer;
Texture gTexture;
int gCount;
GameLib::Graphics::TextureFilter gTextureFilter = GameLib::Graphics::TEXTURE_FILTER_POINT;

namespace GameLib{
	//Configuration�N���X���g���ď����ݒ肵�Ă�낤�B
	void Framework::configure( Configuration* conf ){
		conf->enableAntiAlias( gMultiSampleAntiAlias ); //�W���M�[���y�����鏈���̗L��
		conf->enableVSync( true );
	}
	void Framework::update(){
		using namespace Graphics;
		//���x�ƂȂ��o�Ă���̂�m�ꕶ���ł��邱�Ƃ������ė~����
		Graphics::Manager m = Graphics::Manager::instance();
		if ( gCount == 0 ){ 
			//--------------���_�o�b�t�@�����-----------------------
			gVertexBuffer = VertexBuffer::create( 3 ); //3���_�̒��_�o�b�t�@
			Vertex* v = gVertexBuffer.lock(); //�������݃|�C���^�𓾂�
			//�ʒu������
			v[ 0 ].mPosition.set( -1.f, -1.f, 0.f );
			v[ 1 ].mPosition.set( 0.f, 1.f, 0.f );
			v[ 2 ].mPosition.set( 1.f, -0.8f, 0.f );
			for ( int i = 0; i < 3; ++i ){
				v[ i ].mColor = 0xffffffff;
			}
			//�e�N�X�`�����W������(�K��)
			v[ 0 ].mUv.set( -2.f, 2.f );
			v[ 1 ].mUv.set( 0.f, -2.f );
			v[ 2 ].mUv.set( 2.f, -2.f );
			//�����I����������
			gVertexBuffer.unlock( &v );

			//-------------�e�N�X�`�������---------------------
			gTexture = Texture::create( "robo.tga" );
			while ( !gTexture.isReady() ){
				; //���[�h�҂�
			}
		}
		//�g���f�[�^���w�肷��
		m.setVertexBuffer( gVertexBuffer );
		m.setTexture( gTexture );

		//�s��͒P�ʍs��ł���(�����ϊ��r���[�s��ƃ��[���h�̓���K�v)
		Matrix44 pvm;
		pvm.setIdentity();
		m.setProjectionViewMatrix( pvm );
		Matrix34 wm;
		wm.setIdentity();
		m.setWorldMatrix( wm );
		//�`��
		m.draw( 0, 1, PRIMITIVE_TRIANGLE );

		//�ȉ��̓X�y�[�X�L�[�Ńt�B���^��؂�ւ���R�[�h
		if ( Input::Manager::instance().keyboard().isTriggered( ' ' ) ){
			if ( gTextureFilter == TEXTURE_FILTER_POINT ){
				gTextureFilter = TEXTURE_FILTER_LINEAR;
			}else if ( gTextureFilter == TEXTURE_FILTER_LINEAR ){
				gTextureFilter = TEXTURE_FILTER_POINT;
			}
		}
		//�e�N�X�`���t�B���^�Z�b�g
		m.setTextureFilter( gTextureFilter );

		DebugScreen ds = DebugScreen::instance();
		if ( gTextureFilter == TEXTURE_FILTER_POINT ){
			ds << "TextureFilter : POINT" << endl;
		}else if ( gTextureFilter == TEXTURE_FILTER_LINEAR ){
			ds <<"TextureFilter : LINEAR" << endl;
		}
		if ( gMultiSampleAntiAlias ){
			ds << "MultiSampleAntiAlias : TRUE" << endl;
		}else{
			ds << "MultiSampleAntiAlias : FALSE" << endl;
		}
		++gCount;

		//��Еt�� 
		if ( isEndRequested() ){
			//�O���[�o���ɒu���Ə���ɂ͏����Ȃ��̂ŉ��
			gTexture.release();
			gVertexBuffer.release();
		}
	}
}
