#include "GameLib/GameLib.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Functions.h"
using namespace GameLib;

#include "Library/GraphicsDatabase.h"
#include "Library/Model.h"
using namespace std;

#include "Bullet.h"

//パラメータ群
const int Bullet::mLife = 120;
const float Bullet::mHomingX = 1.0;
const float Bullet::mHomingY = 1.0;
const float Bullet::mSpeed = 1.0;

Bullet::Bullet() : mCount( -1 ){
}

Bullet::~Bullet(){
}

bool Bullet::isEmpty() const {
	return ( mCount < 0 );
}

void Bullet::die(){
	mCount = -1;
}

void Bullet::create(
AutoPtr< GraphicsDatabase >& db,
const char* batchName,
const Vector3& p,
float angleX,
float angleY,
bool homing ){
	//モデルがあるなら消しとく
	if ( mModel ){
		mModel = 0; //0代入で手動で消せる。
	}
	mModel = db->createModel( batchName );
	mModel->setPosition( p );
	mModel->setAngle( Vector3( angleX, angleY, 0.f ) );
	mHoming = homing;
	mCount = 0;
}

const Vector3* Bullet::getPosition() const {
	return mModel->getPosition();
}

void Bullet::update( const Vector3& enemyPos ){
	//敵の方に向ける。
	Vector3 dir;
	Vector3 p = *mModel->getPosition();
	Vector3 a = *mModel->getAngle();
	if ( mHoming ){
		dir.setSub( enemyPos, *mModel->getPosition() ); //自分から敵へ
		//Y軸角度はatan2( x, z )。
		float ty = atan2( dir.x, dir.z );
		//180度以上差があれば+-360度して逆回し
		if ( ty - a.y > 180.f ){
			ty -= 2.f * 180.f;
		}else if ( a.y - ty > 180.f ){
			ty += 2.f * 180.f;
		}
		//X軸角度はY/(X,Z)。
		float zxLength = sqrt( dir.x * dir.x + dir.z * dir.z );
		float tx = atan2( dir.y, zxLength );
		//X軸角度はそもそも範囲が(-90,90)で180度以上離れることはない。そのままで良い。
		float hx = mHomingX;
		float hy = mHomingY; 
		//ホーミング範囲内ならそのものに
		if ( tx - a.x < hx && a.x - tx < hx ){
			a.x = tx;
		}else if ( tx < a.x ){
			a.x -= hx;
		}else{
			a.x += hx;
		}
		if ( ty - a.y < hy && a.y - ty < hy ){
			a.y = ty;
		}else if ( ty < a.y ){
			a.y -= hy;
		}else{
			a.y += hy;
		}
	}
	//おもろいのでz回転つけとくか
	a.z += 20.0;
	//角度更新
	mModel->setAngle( a );
	//位置はこの方向の回転行列で(0,0,1)を変換して足してやる
	Vector3 v( 0.0, 0.0, mSpeed );
	Matrix34 m;
	m.setRotationY( a.y );
	m.rotateX( -a.x );
	m.mul( &v, v );
	p += v;
	mModel->setPosition( p );
	++mCount;
	if ( mCount >= mLife ){
		mCount = -1;
	}
}

void Bullet::draw() const {
	mModel->draw();
}
