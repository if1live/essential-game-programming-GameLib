#ifndef INCLUDED_GAME_STATE_H
#define INCLUDED_GAME_STATE_H

#include "Array2D.h"

class Image; //�錾�������Ă����΂����B�C���N���[�h���Ȃ��Ă����B
class StaticObject;

class State{
public:
	//�X�e�[�W�ԍ���^���ăR���X�g���N�g�B0���Ɠ�l�p
	static const int STAGE_ID_2PLAYERS = 0;
	State( int stageID );
	~State();
	//���͂͒��łƂ�
	void update();
	void draw() const;
	//�O��������
	bool hasCleared() const;
	bool isAlive1P() const; //1P�����Ă�H
	bool isAlive2P() const; //2P�����Ă�H
private:
	//�����Ȃ��I�u�W�F�N�g
	Array2D< StaticObject > mStaticObjects;
	int mStageID;

	Image* mImage; //�摜
};

#endif
