#include <sstream>
#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Manager.h"

#include "Robo.h"
#include "Bullet.h"
#include "Pad.h"
#include "Library/Matrix34.h"
#include "Library/GraphicsDatabase.h"
#include "Library/Model.h"
#include "GameLib/Math.h"
using namespace GameLib;

Robo::Robo( int id ) : 
mPosition( 0.0, 0.0, 0.0 ),
mAngleY( 0.0 ),
mId( id ),
mDatabase( 0 ),
mModel( 0 ),
mBullets( 0 ),
mBulletNumber( 100 ){ //���߂���
	mDatabase = new GraphicsDatabase( "robo.txt" );
	mModel = mDatabase->createModel( "robo" );
	mBullets = new Bullet[ mBulletNumber ];
}

Robo::~Robo(){
	SAFE_DELETE_ARRAY( mBullets );
	SAFE_DELETE( mModel ); //�g���Ă�������g���Ă��������
	SAFE_DELETE( mDatabase );
}

void Robo::setPosition( const Vector3& a ){
	mPosition = a;
}

void Robo::setAngleY( double a ){
	mAngleY = a;
}

void Robo::update( const Vector3& enemyPos ){
	Pad* pad = Pad::instance();
	if ( pad->isOn( Pad::TURN, mId ) ){
		if ( pad->isOn( Pad::LEFT, mId ) ){
			mAngleY += 1.0;
			if ( mAngleY > 180.0 ){ //-PI����PI�ɂ����߂�
				mAngleY -= 360.0;
			}
		}
		if ( pad->isOn( Pad::RIGHT, mId ) ){
			mAngleY -= 1.0;
			if ( mAngleY < -180.0 ){ //-PI����PI�ɂ����߂�
				mAngleY += 360.0;
			}
		}
	}else{
		//�ړ������B�܂����_���l�����Ȃ������x���o��
		Vector3 move( 0.0, 0.0, 0.0 );
		if ( pad->isOn( Pad::UP, mId ) ){
			move.z = -1.0;
		}
		if ( pad->isOn( Pad::DOWN, mId ) ){
			move.z = 1.0;
		}
		if ( pad->isOn( Pad::LEFT, mId ) ){
			move.x = -1.0;
		}
		if ( pad->isOn( Pad::RIGHT, mId ) ){
			move.x = 1.0;
		}
		//�����������������ĉ�]
		Matrix34 m;
		m.setRotationY( mAngleY + 180.0 );
		m.multiply( &move, move );
		mPosition += move;
	}

	//���퐶��
	if ( pad->isOn( Pad::FIRE, mId ) ){
		//�󂫕����T��
		for ( int i = 0; i < mBulletNumber; ++i ){
			if ( mBullets[ i ].isEmpty() ){
				mBullets[ i ].create(
					mDatabase, 
					"bullet", 
					mPosition,
					45.0,
					mAngleY );
				break;
			}
		}
	}
	//����X�V
	for ( int i = 0; i < mBulletNumber; ++i ){
		if ( !mBullets[ i ].isEmpty() ){
			mBullets[ i ].update( enemyPos );
		}
	}
}

void Robo::draw( const Matrix44& pvm ) const {
	//���f���Ɉʒu�����Z�b�g
	mModel->setAngle( Vector3( 0.0, mAngleY, 0.0 ) );
	mModel->setPosition( mPosition );
	//�`��
	mModel->draw( pvm );
	//����
	for ( int i = 0; i < mBulletNumber; ++i ){
		if ( !mBullets[ i ].isEmpty() ){
			mBullets[ i ].draw( pvm );
		}
	}
}

const Vector3* Robo::position() const {
	return &mPosition;
}

void Robo::getViewMatrix( Matrix34* vm ) const {
	//�܂����ʕ����x�N�^���쐬
	Vector3 d( 0.0, 0.0, 1.0 );
	Matrix34 m;
	m.setRotationY( mAngleY );
	m.multiply( &d, d );
	//������O����mCameraTargetDistanceZ�����L�΂�
	Vector3 t;
	t.setMul( d, 20.0 );
	//���{�������Ƃ���ɂ���Ȃ炿����Ɖ������Ă��B����̓p�����[�^�ɂȂ����̏�H�v�B
	t.y -= mPosition.y * 0.12; //���̂ւ�̒������K��
	//�����������mCameraDistacneZ�����L�΂�
	Vector3 p;
	p.setMul( d, -20.0 );
	//Y��mCameraDistanceY���v���X
	p.y += 20.0;
	//���{�������Ƃ���ɂ���Ȃ炿����ƍ��ڂɂ��ĉ������Ă��B����̓p�����[�^�ɂȂ����̏�H�v�B
	p.y += mPosition.y * 0.12; //���̂ւ�̒������K��
	//���{���݈ʒu���v���X
	t += mPosition;
	p += mPosition;
	//�r���[�s��쐬
	vm->setViewTransform( p, t );
}