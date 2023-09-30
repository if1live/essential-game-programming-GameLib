#ifndef INCLUDED_ROBO_H
#define INCLUDED_ROBO_H

#include "Library/Vector3.h"

class Matrix34;
class Matrix44;
class GraphicsDatabase;
class Model;
class Bullet;

class Robo{
public:
	Robo( int id ); //�ԍ����炤�B���������Ԃ��m�肽���̂ŁB
	~Robo();
	void draw( 
		const Matrix44& perspectiveViewMatrix,
		const Vector3& lightVector,
		const Vector3& lightColor,
		const Vector3& ambient ) const;
	void update( Robo* robo );
	void setPosition( const Vector3& );
	void setAngleY( double );
	const Vector3* position() const;
	void getViewMatrix( Matrix34* ) const;
	void setDamage( int damage );
	int hitPoint() const;
	int energy() const;
	bool isLockOn() const;

	static const int mJumpUpTime; //�㏸���Ă�������
	static const int mJumpStayTime; //�㏸�㉺�~�܂ł̎���
	static const int mJumpFallTime; //���~�ɂ����鎞��
	static const int mMoveAccelEndCount; //�����n�߂ĉ������I������܂ł̎���
	static const double mMaxMoveSpeed; //�ő�ړ����x
	static const double mJumpHeight; //�ő卂�x
	static const int mCameraDelayCount; //�W�����v�J�n�㉽�t���[���œG�̕���������
	static const double mCameraDistanceZ; //�����[�g����납��ʂ��H
	static const double mCameraDistanceY; //�����낵�
	static const double mCameraTargetDistanceZ; //�����_�͉����[�g����H
	static const double mTurnSpeed; //���񑬓x
	static const int mMaxHitPoint; //�ő�q�b�g�|�C���gz
	static const int mMaxEnergy; //����|�C���g�ő�l
	static const int mEnergyPerBullet; //�ꔭ������̏���G�l���M�[
	static const int mEnergyCharge; //���t���[�����܂�G�l���M�[
	static const double mLockOnAngleIn; //���b�N�I������p�x
	static const double mLockOnAngleOut; //���b�N�I������p�x
private:
	void move( bool left, bool right, bool up, bool down );
	void turn( bool left, bool right );
	//�v�l���[�`���B�{�^�����͂�Ԃ��B�v���C���[����L�����Ȃ炽�����͂���邾��
	void think( bool* jump, bool* fire, bool* turn, bool* left, bool* right, bool* up, bool* down ) const;

	Vector3 mPosition;
	double mAngleY;
	int mId;
	GraphicsDatabase* mDatabase;
	Model* mModel;
	
	Bullet* mBullets;
	int mBulletNumber;

	int mCameraCount;
	int mCount; //�ړ��J�n�㉽�t���[���o�����H
	Vector3 mVelocity; //���݂̕��ʑ��x
	double mAngleVelocityY; //�U��������x


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
