#ifndef INCLUDED_GAME_STATE_H
#define INCLUDED_GAME_STATE_H

#include "Array2D.h"

class Image; //�錾�������Ă����΂����B�C���N���[�h���Ȃ��Ă����B
class StaticObject;
class DynamicObject;

class State{
public:
	//�X�e�[�W�ԍ���^���ăR���X�g���N�g�B0���Ɠ�l�p
	static const int STAGE_ID_2PLAYERS = 0;
	State( int stageID );
	~State();
	void update();
	void draw() const;
	//�O��������
	bool hasCleared() const;
	bool isAlive( int playerID ) const; //�����Ă�H
private:
	//���ݒu�B���W�͔��e�̍��W
	void setFire( int x, int y );

	//�����Ȃ��I�u�W�F�N�g
	Array2D< StaticObject > mStaticObjects;
	//�����I�u�W�F�N�g
	DynamicObject* mDynamicObjects;
	int mDynamicObjectNumber;

	int mStageID;

	Image* mImage; //�摜
};

#endif
