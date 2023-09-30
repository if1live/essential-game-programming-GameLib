#include "GameLib/Framework.h"
#include "GameLib/Scene/Container.h"
#include "GameLib/Scene/Model.h"
#include "GameLib/Scene/Tree.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Math/Functions.h"
#include "GameLib/Graphics/Manager.h"

using namespace GameLib::Scene;
using namespace GameLib::Graphics;
using namespace GameLib;

Container gContainer;
Model gModel0;
Model gModel1;
int gCount;

namespace GameLib{
	void Framework::configure( Configuration* c ){
		c->enableVSync();
	}
	void Framework::update(){
		if ( !gContainer ){
			gContainer = Container::create( "cube.txt" );
			while ( !gContainer.isReady() ){
				; //���[�h�҂�
			}
			gModel0 = gContainer.createModel( 0 );
			gModel1 = gContainer.createModel( 0 );
			gModel1.setScale( Vector3( 0.5f ) ); //�����͔���
			gModel0.setColor( Vector3( 1.f, 1.f, 0.5f ) ); //���F
			gModel1.setColor( Vector3( 0.5f, 1.f, 1.f ) ); //���F
			gModel0.setTransparency( 0.5f );
			gModel1.setTransparency( 0.5f );
		}
		//�r���[�s�����낤
		Vector3 eye( 0.f, 0.f, 5.f );
		Matrix34 vm;
		vm.setViewTransform( 
			eye,
			Vector3( 0.f, 0.f, 0.f ), 
			Vector3( 0.f, 1.f, 0.f ) );
		//�����ϊ��s��
		Matrix44 pm;
		pm.setPerspectiveTransform( 
			60.f, 
			static_cast< float >( width() ),
			static_cast< float >( height() ),
			1.f, 10000.f );
		//����PV�����
		pm *= vm;

		//���C�g�ł�����������
		Graphics::Manager gm = Graphics::Manager::instance(); 
		gm.setProjectionViewMatrix( pm );
		gm.setLightingMode( LIGHTING_PER_VERTEX );
		gm.setLightColor( 0, Vector3( 1.f, 1.f, 1.f ) ); //��
		gm.setLightColor( 1, Vector3( 1.f, 0.5f, 0.5f ) ); //��
		gm.setLightColor( 2, Vector3( 0.5f, 1.f, 0.5f ) ); //��
		gm.setLightColor( 3, Vector3( 0.5f, 0.5f, 1.f ) ); //��
		gm.setAmbientColor( Vector3( 0.2f, 0.2f, 0.2f ) );
		gm.setEyePosition( eye );
		for ( int i = 0; i < 4; ++i ){
			float t = static_cast< float >( gCount * ( i + 1 ) ) * 0.1f;
			Vector3 p;
			p.set( sin( t )*cos( t ),  sin( t )*sin( t ), cos( t ) );
			p *= 100.f;
			gm.setLightPosition( i, p );
			gm.setLightIntensity( i, 50.f );
		}
		float fc = static_cast< float >( gCount ) * 0.5f;
		gModel0.setAngle( Vector3( fc, fc * 2.f, fc * 3.f ) );
		gModel1.setAngle( Vector3( fc * 2.f, fc * 3.f, fc ) );
		//Z�o�b�t�@��������Off
		gm.enableDepthWrite( true );
		//���ʏ�������
		gm.setCullMode( Graphics::CULL_FRONT );
		gModel0.draw();
		gModel1.draw();
		//�\�ʏ�������
		gm.setCullMode( Graphics::CULL_BACK );
		gModel1.draw(); //������
		gModel0.draw();

		if ( isEndRequested() ){
			gModel0.release();
			gModel1.release();
			gContainer.release();
		}
		++gCount;
	}
}
