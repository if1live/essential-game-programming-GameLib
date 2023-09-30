#include "GameLib/GameLib.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Functions.h"

#include "Bullet.h"

using namespace GameLib;
using namespace GameLib::Scene;

using namespace std;

//�p�����[�^�Q
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
Container container, 
const char* batchName,
const Vector3& p,
float angleX,
float angleY,
bool homing ){
	mModel = container.createModel( batchName );
	mModel.setPosition( p );
	mModel.setAngle( Vector3( angleX, angleY, 0.f ) );
	mHoming = homing;
	mCount = 0;
}

const Vector3* Bullet::position() const {
	return mModel.position();
}

void Bullet::update( const Vector3& enemyPos ){
	//�G�̕��Ɍ�����B
	Vector3 dir;
	Vector3 p = *mModel.position();
	Vector3 a = *mModel.angle();
	if ( mHoming ){
		dir.setSub( enemyPos, *mModel.position() ); //��������G��
		//Y���p�x��atan2( x, z )�B
		float ty = atan2( dir.x, dir.z ); 
		//180�x�ȏ㍷�������+-360�x���ċt��
		if ( ty - a.y > 180.f ){
			ty -= 2.f * 180.f;
		}else if ( a.y - ty > 180.f ){
			ty += 2.f * 180.f;
		}
		//X���p�x��Y/(X,Z)�B
		float zxLength = sqrt( dir.x * dir.x + dir.z * dir.z );
		float tx = atan2( dir.y, zxLength );
		//X���p�x�͂��������͈͂�(-90,90)��180�x�ȏ㗣��邱�Ƃ͂Ȃ��B���̂܂܂ŗǂ��B
		float hx = mHomingX; 
		float hy = mHomingY; 
		//�z�[�~���O�͈͓��Ȃ炻�̂��̂�
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
	//�����낢�̂�z��]���Ƃ���
	a.z += 10.0;
	//�p�x�X�V
	mModel.setAngle( a );
	//�ʒu�͂��̕����̉�]�s���(0,0,1)��ϊ����đ����Ă��
	Vector3 v( 0.0, 0.0, mSpeed );
	Matrix34 m;
	m.setRotationY( a.y );
	m.rotateX( -a.x );
	m.mul( &v, v );
	p += v;
	mModel.setPosition( p );
	++mCount;
	if ( mCount >= mLife ){
		mCount = -1;
	}
}

void Bullet::draw() const {
	mModel.draw();
}
