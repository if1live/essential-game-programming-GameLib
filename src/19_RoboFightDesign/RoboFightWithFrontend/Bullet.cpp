#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Library/Vector3.h"
#include "Library/Matrix34.h"
#include "Library/GraphicsDatabase.h"
#include "Library/Model.h"
#include "GameLib/Math.h"
using namespace std;

#include "Bullet.h"

//�p�����[�^�Q
const int Bullet::mLife = 120;
const float Bullet::mHomingX = 1.0;
const float Bullet::mHomingY = 1.0;
const float Bullet::mSpeed = 1.0;

Bullet::Bullet() : mModel( 0 ), mCount( -1 ){
}

Bullet::~Bullet(){
	SAFE_DELETE( mModel );
}

bool Bullet::isEmpty() const {
	return ( mCount < 0 );
}

void Bullet::die(){
	mCount = -1;
}

void Bullet::create(
GraphicsDatabase* db, 
const char* batchName,
const Vector3& p,
double angleX,
double angleY,
bool homing,
unsigned color ){
	//���f��������Ȃ�����Ƃ�
	SAFE_DELETE( mModel );

	mModel = db->createModel( batchName );
	mModel->setPosition( p );
	mModel->setAngle( Vector3( angleX, angleY, 0.0 ) );
	mModel->setColor( color );
	mHoming = homing;
	mCount = 0;
}

const Vector3* Bullet::position() const {
	return mModel->position();
}

void Bullet::update( const Vector3& enemyPos ){
	//�G�̕��Ɍ�����B
	Vector3 dir;
	Vector3 p = *mModel->position();
	Vector3 a = *mModel->angle();
	if ( mHoming ){
		dir.setSub( enemyPos, *mModel->position() ); //��������G��
		//Y���p�x��atan2( x, z )�B
		double ty = atan2( dir.x, dir.z );
		//180�x�ȏ㍷�������+-360�x���ċt��
		if ( ty - a.y > 180.0 ){
			ty -= 360.0;
		}else if ( a.y - ty > 180.0 ){
			ty += 360.0;
		}
		//X���p�x��Y/(X,Z)�B
		double zxLength = sqrt( dir.x * dir.x + dir.z * dir.z );
		double tx = atan2( dir.y, zxLength );
		//X���p�x�͂��������͈͂�(-90,90)��180�x�ȏ㗣��邱�Ƃ͂Ȃ��B���̂܂܂ŗǂ��B
		double hx = mHomingX; //�ʖ�
		double hy = mHomingY; 
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
	a.z += 2.0;
	//�p�x�X�V
	mModel->setAngle( a );
	//�ʒu�͂��̕����̉�]�s���(0,0,1)��ϊ����đ����Ă��
	Vector3 v( 0.0, 0.0, mSpeed );
	Matrix34 m;
	m.setRotationY( a.y );
	m.rotateX( -a.x );
	m.multiply( &v, v );
	p += v;
	mModel->setPosition( p );
	++mCount;
	if ( mCount >= mLife ){
		mCount = -1;
	}
}

void Bullet::draw( const Matrix44& pvm ) const {
	mModel->draw( pvm );
}
