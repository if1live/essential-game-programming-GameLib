#include <sstream>
#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Manager.h"

#include "Robo.h"
#include "Bullet.h"
#include "Pad.h"
#include "Library/Matrix34.h"
#include "Library/GraphicsDatabase.h"
#include "Library/Tree.h"
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
const int Robo::mMaxEnergy = 100; //�ő啐��|�C���g
const int Robo::mEnergyPerBullet = 27; //����̏���G�l���M�[
const int Robo::mEnergyCharge = 1; //���t���[�����܂�G�l���M�[�̗�
const double Robo::mLockOnAngleIn = 10.0; //���b�N�I������p�x
const double Robo::mLockOnAngleOut = 30.0; //���b�N�I�����O���p�x

Robo::Robo( int id ) : 
mPosition( 0.0, 0.0, 0.0 ),
mAngleY( 0.0 ),
mId( id ),
mDatabase( 0 ),
mTree( 0 ),
mBullets( 0 ),
mBulletNumber( 100 ), //���߂���
mCameraCount( 0 ),
mCount( 0 ),
mVelocity( 0.0, 0.0, 0.0 ),
mMode( MODE_STANDING ),
mHitPoint( mMaxHitPoint ),
mEnergy( mMaxEnergy ),
mLockOn( true ){ //���������Ďn��aaaaaaaaa��̂ōŏ��̓��b�N�I�����ĂĂ������낤�B
	mDatabase = new GraphicsDatabase( "robo.txt" );
	mTree = mDatabase->createTree( "robo" );
	mTree->setAnimation( mDatabase->animation( "standing" ) ); //�����A�j���Z�b�g
	mBullets = new Bullet[ mBulletNumber ];

}

Robo::~Robo(){
	SAFE_DELETE_ARRAY( mBullets );
	SAFE_DELETE( mTree ); //�g���Ă�������g���Ă��������
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
	//AI�̎v�l�B�v���C���[�̏ꍇ�͓��͂��擾���ĕԂ�����
	bool iJump;
	bool iFire;
	bool iTurn;
	bool iLeft; 
	bool iRight; 
	bool iUp;
	bool iDown; 
	think( &iJump, &iFire, &iTurn, &iLeft, &iRight, &iUp, &iDown );
	//�ȉ�����������͂��g���čs��
	const Vector3& enemyPos = *enemy->position();
	++mCount;
	//�W�����v������Ă�H
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
			if ( !iJump ){ //�W�����v���͂��Ȃ��̂ŉ��~�ɕω�
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
			if ( !iJump ){ //�W�����v���͂��Ȃ��̂ŉ��~�ɕω�
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
		case MODE_STANDING:
		case MODE_WALKING:
			if ( iJump ){
				mMode = MODE_JUMP_UP;
				mCount = 0;
				mCameraCount = 0;

				//�G�̕��Ɍ�����B
				Vector3 dir;
				dir.setSub( enemyPos, mPosition ); //��������G��
				//Y���p�x��atan2( x, z )�B
				t = GameLib::atan2( dir.x, dir.z );
				//180�x�ȏ㍷�������+-360�x���ċt��
				if ( t - mAngleY > 180.0 ){
					t -= 360.0;
				}else if ( mAngleY - t > 180.0 ){
					t += 360.0;
				}
				mAngleVelocityY = ( t - mAngleY ) / static_cast< double >( mCameraDelayCount );
				mTree->setAnimation( mDatabase->animation( "jumping" ) ); //�W�����v
			}else if ( iTurn ){
				turn( iLeft, iRight ); //�R�[�h�������Ȃ�̂Ŋ֐��ɔ�΂�
			}else{
				move( iLeft, iRight, iUp, iDown ); //�R�[�h�������Ȃ�̂Ŋ֐��ɔ�΂�
			}
			mVelocity.y = 0.0;
			break;
	}
	//�������牺�͏Փˏ���������Ƃ��̌�ɂȂ�B
	mPosition += mVelocity;
	if ( mPosition.y < 0.0 ){
		mPosition.y = 0.0;
		mMode = MODE_STANDING;
		mTree->setAnimation( mDatabase->animation( "standing" ) ); //
	}
	//���퐶��
	if ( iFire ){
		//�㏸�A���~���͌��ĂȂ�
		if ( ( mMode != MODE_JUMP_FALL ) && ( mMode != MODE_JUMP_UP ) ){
			//�G�l���M�[����Ă�H
			if ( mEnergy >= mEnergyPerBullet ){
				//�󂫕����T��
				for ( int i = 0; i < mBulletNumber; ++i ){
					if ( mBullets[ i ].isEmpty() ){
						Vector3 c = ( mId == 0 ) ? Vector3( 0.0, 0.35, 0.7 ) : Vector3( 0.7, 0.35, 0.0 );
						mBullets[ i ].create(
							mDatabase, 
							"bullet", 
							mPosition,
							15.0,
							mAngleY,
							mLockOn,
							c );
						mEnergy -= mEnergyPerBullet;
						break;
					}
				}
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
	//����G�l���M�[�`���[�W
	mEnergy += mEnergyCharge;
	if ( mEnergy > mMaxEnergy ){
		mEnergy = mMaxEnergy;
	}
	//���b�N�I������
	//�܂��͊p�x�𑪂��Ă݂悤�B
	//�p�x�͉��ő��邩�H�����A���ς��B
	Vector3 toEnemy;
	toEnemy.setSub( enemyPos, mPosition );
	Vector3 myDir( 0.0, 0.0, -1.0 );
	Matrix34 m;
	m.setRotationY( mAngleY + 180.0 );
	m.multiply( &myDir, myDir );
	toEnemy *= 1.0 / toEnemy.length(); //������1��
	double dotProduct = toEnemy.dot( myDir );
	//�p�x�ɒ����ƁA
	double angle = GameLib::acos( dotProduct );
	if ( mLockOn ){
		//���b�N�I�����Ă�Ȃ�O��邩�ǂ������ׂ�
		if ( angle > mLockOnAngleOut ){
			mLockOn = false;
		}
	}else{
		//���邩�ǂ������ׂ�
		if ( angle < mLockOnAngleIn ){
			mLockOn = true;
		}
	}
	mTree->update(); //�A�j���[�V�����X�V
}

void Robo::think(
bool* jump,
bool* fire,
bool* turn,
bool* left,
bool* right,
bool* up,
bool* down ) const {
	Pad* pad = Pad::instance();
	if ( mId == 0 ){ //�v���C���[
		*jump = pad->isOn( Pad::JUMP, mId );
		*fire = pad->isTriggered( Pad::FIRE, mId );
		*turn = pad->isOn( Pad::TURN, mId );
		*left = pad->isOn( Pad::LEFT, mId );
		*right = pad->isOn( Pad::RIGHT, mId );
		*up = pad->isOn( Pad::UP, mId );
		*down = pad->isOn( Pad::DOWN, mId );
	}else{ //AI
		//���n��
		//���b�N�I�����ĂȂ���΃W�����v����
		*jump = !mLockOn;
		//�e�͌��Ă邾�����B
		*fire = true;
		//����͂��Ȃ�
		*turn = false;
		//�ړ��͕ϐ������̂��ʓ|�Ȃ̂ł��Ȃ��B
		*left = *right = *up = *down = false;
	}
}

void Robo::turn( bool left, bool right ){
	if ( left ){
		mAngleY += mTurnSpeed;
		if ( mAngleY > 180.0 ){ //-PI����PI�ɂ����߂�
			mAngleY -= 360.0;
		}
	}
	if ( right ){
		mAngleY -= mTurnSpeed;
		if ( mAngleY < -180.0 ){ //-PI����PI�ɂ����߂�
			mAngleY += 360.0;
		}
	}
}

void Robo::move( bool left, bool right, bool up, bool down ){
	//�ړ������B�܂����_���l�����Ȃ������x���o��
	Vector3 move( 0.0, 0.0, 0.0 );
	if ( up ){
		move.z = -1.0;
	}
	if ( down ){
		move.z = 1.0;
	}
	if ( left ){
		move.x = -1.0;
	}
	if ( right ){
		move.x = 1.0;
	}
	//�A�j���[�V�����ύX
	if ( move.x != 0.0 || move.z != 0.0 ){
		if ( mMode == MODE_STANDING ){
			mMode = MODE_WALKING;
			mTree->setAnimation( mDatabase->animation( "walking" ) ); //�����o����
		}
	}else{
		if ( mMode == MODE_WALKING ){
			mMode = MODE_STANDING;
			mTree->setAnimation( mDatabase->animation( "standing" ) ); //�Î~
		}
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

void Robo::draw( 
const Matrix44& pvm,
const Vector3& lightVector,
const Vector3& lightColor,
const Vector3& ambient ) const {
	//�c���[�Ɉʒu�����Z�b�g
	mTree->setAngle( Vector3( 0.0, mAngleY, 0.0 ) );
	mTree->setPosition( mPosition );
	//�`��
	mTree->draw( pvm, lightVector, lightColor, ambient );
	//����
	for ( int i = 0; i < mBulletNumber; ++i ){
		if ( !mBullets[ i ].isEmpty() ){
			mBullets[ i ].draw( pvm, lightVector, lightColor, ambient );
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

int Robo::energy() const {
	return mEnergy;
}

bool Robo::isLockOn() const {
	return mLockOn;
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
