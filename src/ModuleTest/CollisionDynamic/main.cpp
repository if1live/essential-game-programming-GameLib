#include "GameLib/Framework.h"
#include "GameLib/DebugScreen.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Math/Random.h"
#include "GameLib/Math/Functions.h"
#include "GameLib/Scene/Container.h"
#include "GameLib/Scene/Model.h"
#include "GameLib/Scene/CollisionDetector.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Input/Mouse.h"

using namespace std;
using namespace GameLib;
using namespace GameLib::Math;
using namespace GameLib::Scene;

class Sphere{
public:
	float mRadius;
	Model mModel;
};

Sphere* gSpheres;
const int gSphereNumber = 1000;
Container gContainer;
CollisionDetector gDetector;

float gEyeDistance = 100.f;
float gAngleY = 0.f;
float gAngleX = 0.f;

namespace GameLib{
	void Framework::configure( Configuration* c ){
		c->enableVSync( false );
	}
	void Framework::update(){
		if ( !gSpheres ){
			gDetector = CollisionDetector::create( gSphereNumber );
			gContainer = Container::create( "cube.txt" );
			while ( !gContainer.isReady() ){ ; } //���[�h�҂�
			gSpheres = new Sphere[ gSphereNumber ];
			//�����z�u
			Random ran = Random::create();
			for ( int i = 0; i < gSphereNumber; ++i ){
				Sphere& s = gSpheres[ i ];
				s.mModel = gContainer.createModel( 0 ); //���f���쐬
				Vector3 p(
					ran.getFloat( -100.f, 100.f ),
					ran.getFloat( -100.f, 100.f ),
					ran.getFloat( -100.f, 100.f ) );
				s.mModel.setPosition( p );
				s.mRadius = 5.f;
			}
		}
		//���x������
		Array< Vector3 > velocities( gSphereNumber );
		for ( int i = 0; i < gSphereNumber; ++i ){
			//���x�����_�ւ̈��͂ŏ�����
			velocities[ i ].setMul( *gSpheres[ i ].mModel.position(), -0.01f );
		}
		//�Փˌ��o
		//�o�^�B�߂�l��ID�Ȃ̂ŕۑ����Ă����B0���珇�ɕԂ�ۏ�͂Ȃ��B
		Array< int > idTable( gSphereNumber );
		for ( int i = 0; i < gSphereNumber; ++i ){
			idTable[ i ] = gDetector.add( *gSpheres[ i ].mModel.position(), gSpheres[ i ].mRadius );
		}
		//���o���s��
		Array< CollisionDetector::Pair > results;

		unsigned t0 = time(); //���Ԃ��v���Ă���
		gDetector.detect( &results );
		unsigned t1 = time();

		//�Փˌ��ʂɉ����đ��x���C��
		for ( int i = 0; i < results.size(); ++i ){
			const CollisionDetector::Pair& pair = results[ i ];
			int i0 = idTable[ pair.mId0 ];
			int i1 = idTable[ pair.mId1 ];
			Sphere& s0 = gSpheres[ i0 ];
			Sphere& s1 = gSpheres[ i1 ];
			const Vector3& p0 = *s0.mModel.position();
			const Vector3& p1 = *s1.mModel.position();
			Vector3 n; //�Փ˖@��
			n.setSub( p1, p0 );
			float distance = n.length();
			if ( distance != 0.f ){
				n *= 1.f / distance;
			}else{
				n.set( 0.f, 1.f, 0.f ); //���S�ɍ��킳������Y=1�����ɒ��˕Ԃ�
			}
			n *= 1.f; //���˕Ԃ����x
			velocities[ i0 ] -= n;
			velocities[ i1 ] += n;
		}
		//�ʒu�X�V
		for ( int i = 0; i < gSphereNumber; ++i ){
			Sphere& s = gSpheres[ i ];
			Vector3 p = *s.mModel.position();
			p += velocities[ i ];
			s.mModel.setPosition( p );
		}
		//��ʕ\��
		DebugScreen sc;
		sc << "FRAMERATE: " << frameRate() << " HIT:" << results.size() << " TIME:" << t1 - t0;
//------�ȉ��{�؂Ɋ֌W�Ȃ��R�[�h------

		//�����ϊ�
		Matrix44 pm;
		pm.setPerspectiveTransform( 
			60.f, 
			static_cast< float >( width() ),
			static_cast< float >( height() ),
			gEyeDistance * 0.01f, gEyeDistance * 10.f );
		//�r���[�s�����낤
		Matrix34 rm;
		rm.setRotationY( gAngleY );
		rm.rotateX( gAngleX );
		Vector3 tv( 0.f, 0.f, 1.f );
		rm.mul( &tv, tv );
		Vector3 eyePosition;
		eyePosition.setMul( tv, gEyeDistance );
		pm.multiplyViewTransform( eyePosition, Vector3( 0.f ), Vector3( 0.f, 1.f, 0.f ) );

		//�J�������͔��f
		Input::Mouse mouse; //���͂���Ŏg����
		float x = static_cast< float >( mouse.velocityX() );
		float y = static_cast< float >( mouse.velocityY() );
		if ( mouse.isOn( Input::Mouse::BUTTON_LEFT ) ){ //���{�^����]
				gAngleX -= 0.1f * y;
				if ( gAngleX > 89.f ){
					gAngleX = 89.f;
				}else if ( gAngleX < -89.f ){
					gAngleX = -89.f;
				}
				gAngleY -= 0.1f * x;
		}
		int w = mouse.wheel();
		if ( w < 0 ){
			gEyeDistance *= 0.9f;
		}else if ( w > 0 ){
			gEyeDistance *= 1.1f;
		}

		//�`��
		Graphics::Manager gm;
		gm.setProjectionViewMatrix( pm );
		gm.setLightingMode( Graphics::LIGHTING_NONE );
		gm.enableDepthWrite( true );
		gm.enableDepthTest( true );
		gm.setAmbientColor( Vector3( 0.0f, 0.0f, 0.0f ) );

		gm.setLightColor( 0, Vector3( 0.1f, 0.5f, 0.1f ) );
		gm.setLightPosition( 0, Vector3( 0.f, 0.f, 0.f ) );
		gm.setLightIntensity( 0, 100.f );

		gm.setLightColor( 1, Vector3( 0.6f, 0.4f, 0.2f ) );
		gm.setLightPosition( 1, Vector3( 5777.f, 5777.f, 5777.f ) );
		gm.setLightIntensity( 1, 10000.f );

		gm.setLightColor( 2, Vector3( 0.2f, 0.4f, 0.6f ) );
		gm.setLightPosition( 2, Vector3( -5777.f, -5777.f, -5777.f ) );
		gm.setLightIntensity( 2, 10000.f );

		gm.setEyePosition( eyePosition );

		for ( int i = 0;i < gSphereNumber; ++i ){
			gSpheres[ i ].mModel.draw();
		}

		if ( isEndRequested() ){
			gDetector.release();
			gContainer.release();
			SAFE_DELETE_ARRAY( gSpheres );
		}
	}
}
