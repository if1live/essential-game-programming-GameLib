#include "GameLib/Framework.h"

#include "Library/GraphicsDatabase.h"
#include "Library/Node.h"
#include "Library/Vector3.h"
#include "Library/Matrix44.h"
#include "Library/Matrix34.h"
#include "GameLib/Math.h"
using namespace GameLib;

GraphicsDatabase* gDatabase;
Node* gSun;
Node* gEarth;
Node* gMoon;
Node* gEarthTranslation; //�n���ړ�(�_�~�[)
Node* gMoonRevolution; //�����](�_�~�[)

int gCount;
Vector3 gLightVector;
Vector3 gLightColor( 0.8, 0.7, 0.6 );
Vector3 gAmbient( 0.2, 0.3, 0.4 );
Vector3 gEyePosition( 0.0, 10.0, 20.0 );

namespace GameLib{
	void Framework::update(){
		if ( !gDatabase ){
			setFrameRate( 60 );
			gDatabase = new GraphicsDatabase( "cube.txt" );
			/* �e�q�֌W�}�B�R���X�g���N�^�������͎q�̐����B
			                        +- earth
			sun - earthTranslation -+
			                        +- moonRevolution - moon
			*/
			gSun = new Node( gDatabase->batch( "cube" ), 1 );
			gEarthTranslation = new Node( 0, 2 );
			gEarth = new Node( gDatabase->batch( "cube" ), 0 );
			gMoonRevolution = new Node( 0, 1 );
			gMoon = new Node( gDatabase->batch( "cube" ), 0 );
			//�e�q�֌W�����Ă��
			gSun->setChild( 0, gEarthTranslation );
			gEarthTranslation->setChild( 0, gEarth );
			gEarthTranslation->setChild( 1, gMoonRevolution );
			gMoonRevolution->setChild( 0, gMoon );
		}
		//�J�����ƃ��C�g�𓮂����Ă݂�
		gLightVector.x = sin( gCount ) * cos( gCount );
		gLightVector.y = sin( gCount ) * sin( gCount );
		gLightVector.z = cos( gCount );
		//�r���[�s��A�����ϊ��s��
		Matrix44 pm;
		pm.setPerspectiveTransform( 45.0, width(), height(), 1.0, 10000.0 );
		Matrix34 vm;
		vm.setViewTransform( gEyePosition, Vector3( 0.0, 0.0, 0.0 ) );
		Matrix44 pvm;
		pvm.setMul( pm, vm );

		//�e�m�[�h�ɕϊ��Z�b�g
		gEarthTranslation->setTranslation( Vector3( 10.0, 0.0, 0.0 ) );
		gSun->setRotation( Vector3( 0.0, gCount * 100.0 / 365.0, 0.0 ) );
		gEarth->setRotation( Vector3( 0.0, gCount * 100.0 / 3.0, 0.0 ) );
		gMoonRevolution->setRotation( Vector3( 0.0, gCount * 100.0 / 30.0, 0.0 ) );
		gMoon->setTranslation( Vector3( 4.0, 0.0, 0.0 ) );

		Matrix34 wm;
		wm.setIdentity();
		gSun->draw(
			pvm,
			wm,
			gLightVector,
			gLightColor,
			gAmbient );

		++gCount;
		if ( isEndRequested() ){
			SAFE_DELETE( gSun );
			SAFE_DELETE( gEarth );
			SAFE_DELETE( gMoon );
			SAFE_DELETE( gEarthTranslation );
			SAFE_DELETE( gMoonRevolution );
			SAFE_DELETE( gDatabase );
		}
	}
}
