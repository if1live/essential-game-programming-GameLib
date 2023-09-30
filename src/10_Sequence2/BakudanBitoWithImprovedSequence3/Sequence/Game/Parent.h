#ifndef INCLUDED_SEQUENCE_GAME_PARENT_H
#define INCLUDED_SEQUENCE_GAME_PARENT_H

#include "Sequence/Child.h"
class State;

namespace Sequence{
class Parent;
namespace Game{
class Child;

class Parent : public Sequence::Child{
public:
	typedef Sequence::Parent GrandParent;
	enum Mode{
		MODE_1P,
		MODE_2P,
		MODE_NONE,
	};
	enum PlayerID{
		PLAYER_1,
		PLAYER_2,
		PLAYER_NONE,
	};
	Parent( GrandParent::Mode );
	~Parent();
	Base* update( GrandParent* );

	PlayerID winner() const;
	void setWinner( PlayerID );
	State* state();
	void drawState() const; //�Q�[���`��
	bool hasFinalStageCleared() const; //�ŏI�ʃN���A���܂����H
	int lifeNumber() const;
	Mode mode() const;
	void startLoading();
	void goToNextStage();
	void reduceLife();
private:
	State* mState;
	int mStageID;
	int mLife;
	PlayerID mWinner; //�������炱��ɓ���
	static const int FINAL_STAGE = 2; //����2�ʂ��ŏI��
	static const int INITIALI_LIFE_NUMBER = 2;

	Game::Child* mChild;
};

} //namespace Game
} //namespace Sequence

#endif