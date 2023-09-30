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

//�{���̓t�@�C���ɏo���񂾂�H
const int Robo::mJumpUpTime = 20; //�㏸���Ă�������
const int Robo::mJumpStayTime = 60; //�㏸�㉺�~�܂ł̎���
const int Robo::mJumpFallTime = 40; //���~�ɂ����鎞��
const int Robo::mMoveAccelEndCount = 30; //�����n�߂ĉ������I������܂ł̎���
const double Robo::mMaxMoveSpeed = 0.5; //�ő�ړ����x
const double Robo::mJumpHeight = 20.0; //�ő卂�x
const int Robo::mCameraDelayCount = 10; //�W�����v�J�n�㉽�t���[���œG�̕���������
const double Robo::mCameraDistanceZ = 10.0; //�����[�g����납��ʂ��H
const double Robo::mCameraDistanceY = 4.0; //�����낵�
const double Robo::mCameraTargetDistanceZ = 20.0; //�����_�͉����[�g����H
const double Robo::mTurnSpeed = 1.0; //���񑬓x
const int Robo::mMaxHitPoint = 100; //�ő�q�b�g�b�|�C���g

Robo::Robo( int id ) : 
mPosition( 0.0, 0.0, 0.0 ),
mAngleY( 0.0 ),
mId( id ),
mDatabase( 0 ),
mModel( 0 ),
mBullets( 0 ),
mBulletNumber( 100 ), //���߂���
mCameraCount( 0 ),
mCount( 0 ),
mVelocity( 0.0, 0.0, 0.0 ),
mMode( MODE_ON_LAND ),
mHitPoint( mMaxHitPoint ){
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

void Robo::update( Robo* enemy ){
	//����ł�
	if ( mHitPoint <= 0 ){
		return;
	}
	//�p�x�͈͕␳
	if ( mAngleY > 180.0 ){
		mAngleY -= 360.0;
	}else if ( mAngleY < -180.0 ){
		mAngleY += 360.0;
	}

	const Vector3& enemyPos = *enemy->position();
	++mCount;
	//�W�����v������Ă�H
	Pad* pad = Pad::instance();
	bool inputJump = pad->isOn( Pad::JUMP, mId );
	double t;
	//�����͂Ɏ��������������Ă݂悤�B�R�[�h�̏d���������邪�u���b�N�P�ʂŌ���΃V���v���ɂȂ�B
	//���ʂ̏������Ƃǂ��炪�ǂ�����ׂĂ݂悤�B
	switch ( mMode ){
		case MODE_JUMP_UP:
			//�J��������肫���Ă��Ȃ��Ȃ�J������]�p��
			if ( mCameraCount < mCameraDelayCount ){
				mAngleY += mAngleVelocityY;
				++mCameraCount;
			}
			//�㏸
			t = mJumpHeight / static_cast< double >( mJumpUpTime );
			mVelocity.y = t;
			if ( !inputJump ){ //�W�����v���͂��Ȃ��̂ŉ��~�ɕω�
				mMode = MODE_JUMP_FALL;
				mCount = 0;
			}else if ( mCount >= mJumpUpTime ){ //�㏸�I��
				mMode = MODE_JUMP_STAY; 
				mCount = 0;
			}
			mVelocity.x = mVelocity.z = 0.0; //X,Z�ړ��𖕎E
			break;
		case MODE_JUMP_STAY:
			//�J��������肫���Ă��Ȃ��Ȃ�J������]�p��
			if ( mCameraCount < mCameraDelayCount ){
				mAngleY += mAngleVelocityY;
				++mCameraCount;
			}
			mVelocity.y = 0.0;
			if ( !inputJump ){ //�W�����v���͂��Ȃ��̂ŉ��~�ɕω�
				mMode = MODE_JUMP_FALL;
				mCount = 0;
			}else if ( mCount >= mJumpStayTime ){ //���~��
				mMode = MODE_JUMP_FALL;
				mCount = 0;
			}
			break;
		case MODE_JUMP_FALL:
			//�J��������肫���Ă��Ȃ��Ȃ�J������]�p��
			if ( mCameraCount < mCameraDelayCount ){
				mAngleY += mAngleVelocityY;
				++mCameraCount;
			}
			//���~
			t = mJumpHeight / static_cast< double >( mJumpFallTime );
			mVelocity.y = -t;
			//�ڒn����͍ŏI�I�ɂ͏Փˏ����ł��̂ł����ł͂��Ȃ��B
			break;
		case MODE_ON_LAND:
			if ( inputJump ){
				mMode = MODE_JUMP_UP;
				mCount = 0;
				mCameraCount = 0;

				//�G�̕��Ɍ�����B
				Vector3 dir;
				dir.setSub( enemyPos, mPosition ); //��������G��
				//Y���p�x��atan2( x, z )
				t = GameLib::atan2( dir.x, dir.z );
				//180�x�ȏ㍷�������+-360�x���ċt��
				if ( t - mAngleY > 180.0 ){
					t -= 360.0;
				}else if ( mAngleY - t > 180.0 ){
					t += 360.0;
				}
				mAngleVelocityY = ( t - mAngleY ) / static_cast< double >( mCameraDelayCount );
			}else if ( pad->isOn( Pad::TURN, mId ) ){
				turn(); //�R�[�h�������Ȃ�̂Ŋ֐��ɔ�΂�
			}else{
				move(); //�R�[�h�������Ȃ�̂Ŋ֐��ɔ�΂�
			}
			mVelocity.y = 0.0;
			break;
	}
	//�������牺�͏Փˏ���������Ƃ��̌�ɂȂ�B
	mPosition += mVelocity;
	if ( mPosition.y < 0.0 ){
		mPosition.y = 0.0;
		mMode = MODE_ON_LAND;
	}
	//���퐶��
	if ( pad->isTriggered( Pad::FIRE, mId ) ){
		//�󂫕����T��
		for ( int i = 0; i < mBulletNumber; ++i ){
			if ( mBullets[ i ].isEmpty() ){
				mBullets[ i ].create(
					mDatabase, 
					"bullet", 
					mPosition,
					15.0,
					mAngleY );
				break;
			}
		}
	}
	//����X�V
	for ( int i = 0; i < mBulletNumber; ++i ){
		if ( !mBullets[ i ].isEmpty() ){
			mBullets[ i ].update( enemyPos );
			//�Փˏ���
			Vector3 t;
			t.setSub( *mBullets[ i ].position(), enemyPos );
			if ( t.squareLength() < 4.0 ){
				enemy->setDamage( 1 ); //1�_���炵�Ă݂��B
				mBullets[ i ].die(); //�e�����܂��B
			}
		}
	}
}

void Robo::turn(){
	Pad* pad = Pad::instance();
	if ( pad->isOn( Pad::LEFT, mId ) ){
		mAngleY += mTurnSpeed;
		if ( mAngleY > 180.0 ){ //-PI����PI�ɂ����߂�
			mAngleY -= 360.0;
		}
	}
	if ( pad->isOn( Pad::RIGHT, mId ) ){
		mAngleY -= mTurnSpeed;
		if ( mAngleY < -180.0 ){ //-PI����PI�ɂ����߂�
			mAngleY += 360.0;
		}
	}
}

void Robo::move(){
	//�ړ������B�܂����_���l�����Ȃ������x���o��
	Vector3 move( 0.0, 0.0, 0.0 );
	Pad* pad = Pad::instance();
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

	//���~�܂��Ă���Ȃ�b�͑����B�K���ɉ������Ă�邾����
	if ( mVelocity.x == 0.0 && mVelocity.z == 0.0 ){
		//�����ɂ����鎞�Ԃōő呬�x�������1�t���[��������̉����x���o��B
		double accel = mMaxMoveSpeed / static_cast< double >( mMoveAccelEndCount );
		mVelocity.setMul( move, accel );
	}else{ //���łɓ����Ă���ꍇ
		if ( move.x == 0.0 && move.z == 0.0 ){ //�ړ����[��
			mVelocity.set( 0.0, 0.0, 0.0 ); //�ړ��͂Ƃ܂�B
		}else{
			//���łɓ����Ă���ꍇ���Ȃ�ʓ|�ł���B
			//45�x���������]���������Ƀ[������������Ȃ����Ƃ����̂̓X�g���X���B
			//������A�u���̑��x�ƕ���������Ȃ������������[�������蒼���v�ɂ���B

			//90�x�ȏ�̃^�[���Ȃ��U���x��0�ɂ���B
			//�������������������Q�[�������邪�A���т��ѓ����������Ȃ犵���͎ז����낤�B
			//90�x�ȏ�̃^�[���Ȃ猻���x�Ɖ����̓��ς��}�C�i�X�̂͂���
			double dp = mVelocity.dot( move );
			if ( dp <= 0.0 ){
				mVelocity.set( 0.0, 0.0, 0.0 );
			}else{ //90�x����
				//���݂̈ړ����x�Ɛ����Ȑ����̂ݎ��o��
				//���������́A�ړ������P�ʃx�N�^�Ƃ̓��ς��A�ړ������P�ʃx�N�^�ɂ�����΂����B
				//�ړ��P�ʃx�N�^E�A�����x�x�N�^V�Ƃ��āA�V�������x�x�N�^V'���Ȃ킿���s������
				//V' = dot(V,E) * E
				//���̎��AE�͈ړ��x�N�^M���g����E=M/|M|�Ə����邩��A
				//V' = dot(V,M) * M / ( |M|^2 )
				//�����A�P�ʃx�N�^�����ۂ̕�������������B|M|���|M|^2�̕����v�Z�͑����̂��B
				double moveSquareLength = move.x * move.x + move.z * move.z;
				double dp = mVelocity.dot( move );
				mVelocity.setMul( move, dp / moveSquareLength );
			}
			//������������B
			//�ړ����x�͍ő呬�x/�������Ԃł���B
			double accel = mMaxMoveSpeed / static_cast< double >( mMoveAccelEndCount );
			mVelocity.madd( move, accel );
			//�ő呬�x�ŃX�g�b�v
			double speed = mVelocity.length();
			if ( speed > mMaxMoveSpeed ){
				mVelocity *= mMaxMoveSpeed / speed;
			}
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

void Robo::setDamage( int d ){
	mHitPoint -= d;
}

int Robo::hitPoint() const {
	return mHitPoint;
}

void Robo::getViewMatrix( Matrix34* vm ) const {
	//�܂����ʕ����x�N�^���쐬
	Vector3 d( 0.0, 0.0, 1.0 );
	Matrix34 m;
	m.setRotationY( mAngleY );
	m.multiply( &d, d );
	//������O����mCameraTargetDistanceZ�����L�΂�
	Vector3 t;
	t.setMul( d, mCameraTargetDistanceZ );
	//���{�������Ƃ���ɂ���Ȃ炿����Ɖ������Ă��B����̓p�����[�^�ɂȂ����̏�H�v�B
	t.y -= mPosition.y * 0.12; //���̂ւ�̒������K��
	//�����������mCameraDistacneZ�����L�΂�
	Vector3 p;
	p.setMul( d, -mCameraDistanceZ );
	//Y��mCameraDistanceY���v���X
	p.y += mCameraDistanceY;
	//���{�������Ƃ���ɂ���Ȃ炿����ƍ��ڂɂ��ĉ������Ă��B����̓p�����[�^�ɂȂ����̏�H�v�B
	p.y += mPosition.y * 0.12; //���̂ւ�̒������K��
	//���{���݈ʒu���v���X
	t += mPosition;
	p += mPosition;
	//�r���[�s��쐬
	vm->setViewTransform( p, t );
}