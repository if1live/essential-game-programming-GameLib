#ifndef INCLUDED_ROBO_H
#define INCLUDED_ROBO_H

#include "GameLib/Math/Vector3.h"

namespace GameLib{
	namespace Math{
		class Matrix34;
		class Matrix44;
	}
}
using namespace GameLib::Math;

class GraphicsDatabase;
class Model;
class Bullet;

class Robo{
public:
	Robo( int id ); //�ԍ����炤�B���������Ԃ��m�肽���̂ŁB
	~Robo();
	void draw() const;
	void update( Robo* robo );
	void setPosition( const Vector3& );
	void setAngleY( float );
	const Vector3* getPosition() const;
	void getView( Matrix34* matrix, Vector3* position ) const;
	void setDamage( int damage );
	int getHitPoint() const;
	int getEnergy() const;
	bool getLockOn() const;
	const Vector3* getBulletPosition( int i ) const;

	static const int mJumpUpTime; //�㏸���Ă�������
	static const int mJumpStayTime; //�㏸�㉺�~�܂ł̎���
	static const int mJumpFallTime; //���~�ɂ����鎞��
	static const int mMoveAccelEndCount; //�����n�߂ĉ������I������܂ł̎���
	static const float mMaxMoveSpeed; //�ő�ړ����x
	static const float mJumpHeight; //�ő卂�x
	static const int mCameraDelayCount; //�W�����v�J�n�㉽�t���[���œG�̕���������
	static const float mCameraDistanceZ; //�����[�g����납��ʂ��H
	static const float mCameraDistanceY; //�����낵�
	static const float mCameraTargetDistanceZ; //�����_�͉����[�g����H
	static const float mTurnSpeed; //���񑬓x
	static const int mMaxHitPoint; //�ő�q�b�g�|�C���gz
	static const int mMaxEnergy; //����|�C���g�ő�l
	static const int mEnergyPerBullet; //�ꔭ������̏���G�l���M�[
	static const int mEnergyCharge; //���t���[�����܂�G�l���M�[
	static const float mLockOnAngleIn; //���b�N�I������p�x
	static const float mLockOnAngleOut; //���b�N�I������p�x
private:
	void move( bool left, bool right, bool up, bool down );
	void turn( bool left, bool right );
	//�v�l���[�`���B�{�^�����͂�Ԃ��B�v���C���[����L�����Ȃ炽�����͂���邾��
	void think( bool* jump, bool* fire, bool* turn, bool* left, bool* right, bool* up, bool* down ) const;

	Vector3 mPosition;
	float mAngleY;
	int mId;
	AutoPtr< GraphicsDatabase > mDatabase;
	AutoPtr< Model > mModel;
	
	Array< Bullet > mBullets;
	int mBulletNumber;

	int mCameraCount;
	int mCount; //�ړ��J�n�㉽�t���[���o�����H
	Vector3 mVelocity; //���݂̕��ʑ��x
	float mAngleVelocityY; //�U��������x


	enum Mode{
		MODE_JUMP_UP, //�W�����v�㏸��
		MODE_JUMP_STAY, //�W�����v���Œ�~��
		MODE_JUMP_FALL, //�W�����v�~����
		MODE_ON_LAND, //���n���Ă�
	};
	Mode mMode;

	int mHitPoint; //�̗�
	int mEnergy; //��������̂ɕK�v�ȃG�l���M�[
	bool mLockOn; //���b�N�I�����Ă܂����H
};

#endif
