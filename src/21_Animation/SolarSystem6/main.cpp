#include "GameLib/Framework.h"

#include "Library/GraphicsDatabase.h"
#include "Library/Tree.h"
#include "Library/Vector3.h"
#include "Library/Matrix44.h"
#include "Library/Matrix34.h"
#include "GameLib/Math.h"
using namespace GameLib;

GraphicsDatabase* gDatabase;
Tree* gSolarSystem;

int gCount;
Vector3 gLightVector;
Vector3 gLightColor( 0.8, 0.7, 0.6 );
Vector3 gAmbient( 0.2, 0.3, 0.4 );
Vector3 gEyePosition( 0.0, 20.0, 40.0 );

namespace GameLib{
	void Framework::update(){
		if ( !gDatabase ){
			setFrameRate( 60 );
			gDatabase = new GraphicsDatabase( "solarSystem.txt" );
			gSolarSystem = gDatabase->createTree( "solarSystem" );
			gSolarSystem->setAnimation( gDatabase->animation( "solarSystem" ) );
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
		pm *= vm;

		//�e�m�[�h�ɕϊ��Z�b�g
		gSolarSystem->update(); //��邱�Ƃ͂��ꂾ���ɂȂ��Ă��܂����B
		gSolarSystem->draw(
			pm,
			gLightVector,
			gLightColor,
			gAmbient );

		++gCount;
		if ( isEndRequested() ){
			SAFE_DELETE( gSolarSystem );
			SAFE_DELETE( gDatabase );
		}
	}
}
