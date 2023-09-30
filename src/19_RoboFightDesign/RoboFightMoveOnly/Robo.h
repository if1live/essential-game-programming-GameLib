#ifndef INCLUDED_ROBO_H
#define INCLUDED_ROBO_H

#include "Library/Vector3.h"

class Matrix34;
class Matrix44;
class GraphicsDatabase;
class Model;

class Robo{
public:
	Robo( int id ); //�ԍ����炤�B���������Ԃ��m�肽���̂ŁB
	~Robo();
	void draw( const Matrix44& perspectiveViewMatrix ) const;
	void update( const Vector3& enemyPosition );
	void setPosition( const Vector3& );
	void setAngleY( double );
	const Vector3* position() const;
	void getViewMatrix( Matrix34* ) const;
private:
	void move();
	void turn();

	Vector3 mPosition;
	double mAngleY;
	int mId;
	GraphicsDatabase* mDatabase;
	Model* mModel;
	
	int mCameraCount;
	int mCount; //�ړ��J�n�㉽�t���[���o�����H
	Vector3 mVelocity; //���݂̕��ʑ��x
	double mAngleVelocityY; //�U��������x

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

	enum Mode{
		MODE_JUMP_UP, //�W�����v�㏸��
		MODE_JUMP_STAY, //�W�����v���Œ�~��
		MODE_JUMP_FALL, //�W�����v�~����
		MODE_ON_LAND, //���n���Ă�
	};
	Mode mMode;

};

#endif
