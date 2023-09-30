#include "GameLib/Framework.h"
#include "GameLib/DebugScreen.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Vector4.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Functions.h"
#include "GameLib/Scene/PrimitiveRenderer.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/Graphics/IndexBuffer.h"
#include "GameLib/Graphics/VertexBuffer.h"
#include "GameLib/Graphics/Texture.h"
using namespace GameLib;

#include <limits>
using namespace std;

#include "Pad.h"
#include "Robo.h"
#include "Stage.h"

Robo* gRobo[ 2 ];
Stage* gStage;
int gTime;
bool gPlaying = false;
const int TIME_LIMIT = 90; //90�b

Vector3 gAmbient( 0.0f, 0.2f, 0.4f );
GameLib::Scene::PrimitiveRenderer gPrimitiveRenderer;

void drawRect( Vector2* p, unsigned c1, unsigned c2 ){
	Framework f = Framework::instance();
	Vector4 p4[ 4 ];
	for ( int i = 0; i < 4; ++i ){
		p4[ i ].x = p[ i ].x;
		p4[ i ].y = p[ i ].y;
		p4[ i ].z = 0.0; //z��0�Œ�
		p4[ i ].w = 1.0; //w��1�Œ�
	}
	gPrimitiveRenderer.addTransformedTriangle( p4[ 0 ], p4[ 1 ], p4[ 2 ], c1, c1, c2 );
	gPrimitiveRenderer.addTransformedTriangle( p4[ 3 ], p4[ 1 ], p4[ 2 ], c2, c1, c2 );
}


//���[�U�����֐��B���g��mainLoop()��
namespace GameLib{
	void Framework::configure( Configuration* c ){
		c->enableVSync( true );
		c->setArchiveNumber( 1 );
		c->setArchiveName( 0, "data.bin" );
		c->setLoadMode( LOAD_ARCHIVE_ONLY ); 
	}
	void Framework::update(){
		if ( !gStage ){
			gPrimitiveRenderer = Scene::PrimitiveRenderer::create( 1000, 100 );
			Pad::create();
			gRobo[ 0 ] = new Robo( 0 );
			gRobo[ 1 ] = new Robo( 1 );
			gRobo[ 0 ]->setPosition( Vector3( 0.0, 0.0, 50.0 ) );
			gRobo[ 0 ]->setAngleY( 180.0 );
			gStage = new Stage();
			gPlaying = true;
		}
		if ( gPlaying ){
			gRobo[ 0 ]->update( gRobo[ 1 ] );
			gRobo[ 1 ]->update( gRobo[ 0 ] );
			++gTime;
		}

		//0�ԃ��{����J�����s��Q�b�g
		Matrix44 pvm;
		pvm.setPerspectiveTransform( 45.f, 
			static_cast< float >( width() ),
			static_cast< float >( height() ),
			1.f,
			10000.f );
		Matrix34 vm;
		Vector3 eyePosition;
		gRobo[ 0 ]->getView( &vm, &eyePosition ); //�r���[�s��̓��{�ɍ���Ă��炤
		pvm *= vm;

		//�`��
		//�O���[�o���ȃ��C�e�B���O�ݒ�
		Graphics::Manager gm = Graphics::Manager::instance();
		gm.setProjectionViewMatrix( pvm );
		gm.setEyePosition( eyePosition );
		gm.setLightingMode( Graphics::LIGHTING_PER_PIXEL );
		gm.setAmbientColor( gAmbient );
		gm.setLightColor( 0, Vector3( 1.0f, 0.8f, 0.6f ) );
		gm.enableDepthTest( true );
		gm.enableDepthWrite( true );
		Vector3 lightPos[ 4 ];
		lightPos[ 0 ].set( 100000.f, 100000.f, 100000.f );
		float lightIntensities[ 4 ];
		lightIntensities[ 0 ] = lightPos[ 0 ].length(); //���������x�ɂ���΁A�����������̋�����1�ɂȂ�
		//�c��O�̃��C�g�͒e�̂Ƃ���ɒu�������B
		//�J�����ɋ߂�����3�I�ڂ��B
		float nearestDistance[ 3 ];
		int nearestIndex[ 3 ];
		for ( int i = 0; i < 3; ++i ){
			nearestDistance[ i ] = numeric_limits< float >::max();
			nearestIndex[ i ] = -1;
		}
		//�����𑪂�Ȃ���ŏ����O�擾
		//100�̓��{������̒e�̐��̍ő吔�B
		for ( int i = 0; i < 2; ++i ){
			for ( int j = 0; j < 100; ++j ){
				const Vector3* p = gRobo[ i ]->getBulletPosition( j );
				if ( p ){
					Vector3 t;
					t.setSub( *p, eyePosition );
					float d = t.squareLength();
					if ( d < nearestDistance[ 0 ] ){
						nearestDistance[ 2 ] = nearestDistance[ 1 ];
						nearestDistance[ 1 ] = nearestDistance[ 0 ];
						nearestDistance[ 0 ] = d;
						nearestIndex[ 2 ] = nearestIndex[ 1 ];
						nearestIndex[ 1 ] = nearestIndex[ 0 ];
						nearestIndex[ 0 ] = i * 100 + j;
					}else if ( d < nearestDistance[ 1 ] ){
						nearestDistance[ 2 ] = nearestDistance[ 1 ];
						nearestDistance[ 1 ] = d;
						nearestIndex[ 2 ] = nearestIndex[ 1 ];
						nearestIndex[ 1 ] = i * 100 + j;
					}else if ( d < nearestDistance[ 2 ] ){
						nearestDistance[ 2 ] = d;
						nearestIndex[ 2 ] = i * 100 + j;
					}
				}
			}
		}
		for ( int i = 0; i < 3; ++i ){
			if ( nearestIndex[ i ] != -1 ){
				int robo = nearestIndex[ i ] / 100;
				int bullet = nearestIndex[ i ] % 100;
				const Vector3* p = gRobo[ robo ]->getBulletPosition( bullet );
				lightPos[ i + 1 ] = *p;
				if ( robo == 1 ){
					gm.setLightColor( i + 1, Vector3( 1.f, 0.2f, 0.4f ) );
				}else{
					gm.setLightColor( i + 1, Vector3( 0.2f, 0.4f, 1.f ) );
				}
				lightIntensities[ i + 1 ] = 10.f;
			}else{
				lightIntensities[ i + 1 ] = 0.f;
			}
		}
		gm.setCullMode( Graphics::CULL_BACK );
		//���C�g�ݒ�
		for ( int i = 0; i < 4; ++i ){
			gm.setLightPosition( i, lightPos[ i ] );
			gm.setLightIntensity( i, lightIntensities[ i ] );
		}
		gStage->draw();
		gRobo[ 0 ]->draw();
		gRobo[ 1 ]->draw();

		//�ȉ��t�����g�G���h�`��
		//���f���`x��ł�����ꂽ���̂����ɖ߂�
		gm.setDiffuseColor( Vector3( 1.f, 1.f, 1.f ) );
		gm.setTransparency( 1.f );
		gm.setLightingMode( Graphics::LIGHTING_NONE );

		//�K�v�ȏ��𔲂���
		int hp0 = gRobo[ 0 ]->getHitPoint();
		int hp1 = gRobo[ 1 ]->getHitPoint();
		int e0 = gRobo[ 0 ]->getEnergy();
		bool lockOn0 = gRobo[ 0 ]->getLockOn();

		//Z�e�X�g���p�B�A���t�@�u�����hON
		gPrimitiveRenderer.enableDepthTest( false );
		gPrimitiveRenderer.enableDepthWrite( false );
		gPrimitiveRenderer.setBlendMode( Graphics::BLEND_LINEAR );

		Vector2 p[ 4 ];
		unsigned c1;
		unsigned c2;
		//�̗̓o�[�w�i
		p[ 0 ].set( -0.9f, 0.95f );
		p[ 1 ].set( -0.9f, 0.87f );
		p[ 2 ].set( 0.1f, 0.95f );
		p[ 3 ].set( 0.1f, 0.87f );
		c1 = c2 = 0xff406080;
		drawRect( p, c1, c2 );
		p[ 0 ].set( -0.9f, 0.85f );
		p[ 1 ].set( -0.9f, 0.82f );
		p[ 2 ].set( 0.1f, 0.85f );
		p[ 3 ].set( 0.1f, 0.82f );
		c1 = c2 = 0xff806040;
		drawRect( p, c1, c2 );
		//�̗̓o�[�{��
		float length = static_cast< float >( hp0 ) / static_cast< float >( Robo::mMaxHitPoint );
		p[ 0 ].set( -0.9f, 0.95f );
		p[ 1 ].set( -0.9f, 0.87f );
		p[ 2 ].set( -0.9f + length, 0.95f );
		p[ 3 ].set( -0.9f + length, 0.87f );
		c1 = 0xff882244;
		c2 = 0xff88ccff;
		drawRect( p, c1, c2 );
		length = static_cast< float >( hp1 ) / static_cast< float >( Robo::mMaxHitPoint );
		p[ 0 ].set( -0.9f, 0.85f );
		p[ 1 ].set( -0.9f, 0.82f );
		p[ 2 ].set( -0.9f + length, 0.85f );
		p[ 3 ].set( -0.9f + length, 0.82f );
		c1 = 0xffff4422;
		c2 = 0xffffcc88;
		drawRect( p, c1, c2 );
		//����G�l���M�[
		//�w�i
		p[ 0 ].set( -0.1f, -0.7f );
		p[ 1 ].set( -0.1f, -0.8f );
		p[ 2 ].set( 0.1f, -0.7f );
		p[ 3 ].set( 0.1f, -0.8f );
		c1 = c2 = 0x80404040;
		drawRect( p, c1, c2 );
		//�{��
		gPrimitiveRenderer.setBlendMode( Graphics::BLEND_ADDITIVE ); //���Z�ɂ��Ă݂悤����
		length = 0.2f * static_cast< float >( e0 ) / static_cast< float >( Robo::mMaxEnergy );
		p[ 0 ].set( -0.1f, -0.7f );
		p[ 1 ].set( -0.1f, -0.8f );
		p[ 2 ].set( -0.1f + length, -0.7f );
		p[ 3 ].set( -0.1f + length, -0.8f );
		c1 = 0x80ff0000;
		c2 = 0x80ffff00;
		drawRect( p, c1, c2 );

		//���[�_�[
		gPrimitiveRenderer.setBlendMode( Graphics::BLEND_LINEAR );
		//�w�i
		p[ 0 ].set( 0.7f, 0.7f );
		p[ 1 ].set( 0.7f, 0.9f );
		p[ 2 ].set( 0.9f, 0.7f );
		p[ 3 ].set( 0.9f, 0.9f );
		c1 = c2 = 0x80404040;
		drawRect( p, c1, c2 );
		gPrimitiveRenderer.setBlendMode( Graphics::BLEND_ADDITIVE ); //���Z�ɂ��Ă݂悤��
		//����
		Vector2 t;
		t.set( gRobo[ 0 ]->position()->x, gRobo[ 0 ]->position()->z ); //x.z
		t *= 0.002f; //+-50�̃X�e�[�W��0.2�̑傫���ɏk������񂾂���A0.2/100��0.002
		t += Vector2( 0.8f, 0.8f ); //(0.8,0.8)�����S�Ȃ̂�����A����𑫂�
		p[ 0 ].set( t.x - 0.005f, t.y - 0.005f );
		p[ 1 ].set( t.x - 0.005f, t.y + 0.005f );
		p[ 2 ].set( t.x + 0.005f, t.y - 0.005f );
		p[ 3 ].set( t.x + 0.005f, t.y + 0.005f );
		c1 = c2 = 0xcc0080ff;
		drawRect( p, c1, c2 );
		//�G
		t.set( gRobo[ 1 ]->position()->x, gRobo[ 1 ]->position()->z ); //x.z
		t *= 0.002f; //+-50�̃X�e�[�W��0.2�̑傫���ɏk������񂾂���A0.2/100��0.002
		t += Vector2( 0.8f, 0.8f ); //(0.8,0.8)�����S�Ȃ̂�����A����𑫂�
		p[ 0 ].set( t.x - 0.005f, t.y - 0.005f );
		p[ 1 ].set( t.x - 0.005f, t.y + 0.005f );
		p[ 2 ].set( t.x + 0.005f, t.y - 0.005f );
		p[ 3 ].set( t.x + 0.005f, t.y + 0.005f );
		c1 = c2 = 0xccff8000;
		drawRect( p, c1, c2 );

		//���b�N�I���}�[�N
		if ( lockOn0 ){
			gPrimitiveRenderer.setBlendMode( Graphics::BLEND_ADDITIVE ); //���Z�ɂ��Ă݂悤����
			//�G���{�����W�ϊ����ăX�N���[�����W�ɕϊ�
			Vector3 t = *gRobo[ 1 ]->position();
			Vector4 p4;
			pvm.mul( &p4, t );
			//XY���W��w�Ŋ���Ώo��B
			float x = p4[ 0 ] / p4[ 3 ];
			float y = p4[ 1 ] / p4[ 3 ];
			//�F�͐Ԃ��Ȃ�
			c1 = c2 = 0x80ff0000;
			//��4�{�ł�낤�B
			p[ 0 ].set( x - 0.01f, y + 0.2f );
			p[ 1 ].set( x - 0.01f, y + 0.1f );
			p[ 2 ].set( x + 0.01f, y + 0.2f );
			p[ 3 ].set( x + 0.01f, y + 0.1f );
			drawRect( p, c1, c2 );
			p[ 0 ].set( x - 0.01f, y - 0.2f );
			p[ 1 ].set( x - 0.01f, y - 0.1f );
			p[ 2 ].set( x + 0.01f, y - 0.2f );
			p[ 3 ].set( x + 0.01f, y - 0.1f );
			drawRect( p, c1, c2 );
			p[ 0 ].set( x - 0.2f, y - 0.01f );
			p[ 1 ].set( x - 0.2f, y + 0.01f );
			p[ 2 ].set( x - 0.1f, y - 0.01f );
			p[ 3 ].set( x - 0.1f, y + 0.01f );
			drawRect( p, c1, c2 );
			p[ 0 ].set( x + 0.2f, y - 0.01f );
			p[ 1 ].set( x + 0.2f, y + 0.01f );
			p[ 2 ].set( x + 0.1f, y - 0.01f );
			p[ 3 ].set( x + 0.1f, y + 0.01f );
			drawRect( p, c1, c2 );
		}
		//���Ԑ���
		gPrimitiveRenderer.setBlendMode( Graphics::BLEND_LINEAR ); //���ǂ���
		length = 1.9f * static_cast< float >( 60 * TIME_LIMIT - gTime ) / static_cast< float >( 60 * TIME_LIMIT );
		p[ 0 ].set( -0.95f, -0.95f );
		p[ 1 ].set( -0.95f, -0.9f );
		p[ 2 ].set( 0.95f, -0.95f );
		p[ 3 ].set( 0.95f, -0.9f );
		c1 = c2 = 0x80404040;
		drawRect( p, c1, c2 );
		p[ 0 ].set( -0.95f, -0.95f );
		p[ 1 ].set( -0.95f, -0.9f );
		p[ 2 ].set( -0.95f + length, -0.95f );
		p[ 3 ].set( -0.95f + length, -0.9f );
		gPrimitiveRenderer.setBlendMode( Graphics::BLEND_ADDITIVE ); //���Z?
		c1 = 0x80ff8888;
		c2 = 0x8088ffff;
		drawRect( p, c1, c2 );

		if ( Input::Manager().keyboard().isTriggered( ' ' ) ){
			gPlaying = !gPlaying;
		}

		if ( hp0 <= 0 || hp1 <= 0 || gTime >= 60 * TIME_LIMIT ){
			gPlaying = false;
			DebugScreen::instance().draw( 20, 5, "Game Over. press Fire" );
			if ( Pad::instance()->isTriggered( Pad::FIRE, 0 ) ){
				//�Z�b�g���Ă�����̂͌�n��
				gm.setVertexBuffer( 0 );
				gm.setIndexBuffer( 0 );
				gm.setTexture( 0 );

				SAFE_DELETE( gRobo[ 0 ] );
				SAFE_DELETE( gRobo[ 1 ] );
				gRobo[ 0 ] = new Robo( 0 );
				gRobo[ 1 ] = new Robo( 1 );
				gRobo[ 0 ]->setPosition( Vector3( 0.f, 0.f, 50.f ) );
				gRobo[ 0 ]->setAngleY( 180.0 );
				gPlaying = true;
				gTime = 0;
			}
		}
		gPrimitiveRenderer.draw(); //�����I�������`��
		//�I������(�}�E�X�Ł~�{�^�����@���ꂽ��)
		if ( isEndRequested() ){
			if ( gStage ){
				Pad::destroy();
				SAFE_DELETE( gRobo[ 0 ] );
				SAFE_DELETE( gRobo[ 1 ] );
				SAFE_DELETE( gStage );
				//�O���[�o���ɂ�����̂͏���ɂ͏����Ȃ��̂ŊJ��
				gPrimitiveRenderer.release(); 
			}
		}
	}
}
