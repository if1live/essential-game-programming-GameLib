#include "GameLib/Framework.h"

#include "Library/GraphicsDatabase.h"
#include "Library/Model.h"
#include "Library/Vector3.h"
#include "Library/Matrix44.h"
#include "Library/Matrix34.h"
#include "GameLib/Math.h"
using namespace GameLib;
using namespace GameLib;

GraphicsDatabase* gDatabase;
Model* gSun;
Model* gEarth;
Model* gMoon;
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
			gSun = gDatabase->createModel( "cube" );
			gEarth = gDatabase->createModel( "cube" );
			gMoon = gDatabase->createModel( "cube" );
		}
		//カメラとライトを動かしてみる
		gLightVector.x = sin( gCount ) * cos( gCount );
		gLightVector.y = sin( gCount ) * sin( gCount );
		gLightVector.z = cos( gCount );
		//ビュー行列、透視変換行列
		Matrix44 pm;
		pm.setPerspectiveTransform( 45.0, width(), height(), 1.0, 10000.0 );
		Matrix34 vm;
		vm.setViewTransform( gEyePosition, Vector3( 0.0, 0.0, 0.0 ) );
		pm *= vm;

		//行列生成開始
		Matrix34 w;
		w.setTranslation( Vector3( 0.0, 0.0, 0.0 ) ); //太陽系の中心
		w.rotateY( gCount * 100.0 / 365.0 ); //太陽系全体の回転=地球公転
		Matrix34 l1a;
		l1a.setTranslation( Vector3( 10.0, 0.0, 0.0 ) ); //太陽->地球
		Matrix34 l1b0;
		l1b0.setRotationY( gCount * 100.0 / 30.0 ); //月公転
		Matrix34 l1b1;
		l1b1.setRotationY( gCount * 100.0 / 3.0 ); //地球自転
		Matrix34 l0;
		l0.setTranslation( Vector3( 4.0, 0.0, 0.0 ) ); //地球->月

		//太陽
		Matrix44 pvwm;
		pvwm.setMul( pm, w );
		gSun->draw( pvwm, gLightVector, gLightColor, gAmbient );

		Matrix34 wL1a = w;
		wL1a *= l1a;

		//地球
		Matrix34 t = wL1a;
		t *= l1b1;
		pvwm.setMul( pm, t );
		gEarth->draw( pvwm, gLightVector, gLightColor, gAmbient );
		
		//月
		t = wL1a;
		t *= l1b0;
		t *= l0;
		pvwm.setMul( pm, t );
		gMoon->draw( pvwm, gLightVector, gLightColor, gAmbient );

		++gCount;
		if ( isEndRequested() ){
			SAFE_DELETE( gSun );
			SAFE_DELETE( gEarth );
			SAFE_DELETE( gMoon );
			SAFE_DELETE( gDatabase );
		}
	}
}

