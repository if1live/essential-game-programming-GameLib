#ifndef INCLUDED_SEQUENCE_GAME_PARENT_H
#define INCLUDED_SEQUENCE_GAME_PARENT_H

class State;

namespace Sequence{
class Parent;
namespace Game{

class Clear;
class Ready;
class Pause;
class Play;
class Failure;
class Judge;

class Parent{
public:
	typedef Sequence::Parent GrandParent;
	enum NextSequence{
		NEXT_CLEAR,
		NEXT_READY,
		NEXT_PAUSE,
		NEXT_PLAY,
		NEXT_FAILURE,
		NEXT_JUDGE,
		NEXT_ENDING,
		NEXT_GAME_OVER,
		NEXT_TITLE,

		NEXT_NONE,
	};
	enum Mode{
		MODE_1P,
		MODE_2P,
		MODE_NONE,
	};
	Parent();
	~Parent();
	void update( GrandParent* );
	void moveTo( NextSequence );

	State* state();
	bool hasFinalStageCleared() const; //�ŏI�ʃN���A���܂����H
	int lifeNumber() const;
	Mode mode() const;
	void startLoading();
private:
	State* mState;
	int mStageID;
	int mLife;
	static const int FINAL_STAGE = 2; //����2�ʂ��ŏI��
	static const int INITIALI_LIFE_NUMBER = 2;

	NextSequence mNextSequence;

	Clear* mClear;
	Ready* mReady;
	Pause* mPause;
	Play* mPlay;
	Failure* mFailure;
	Judge* mJudge;
};

} //namespace Game
} //namespace Sequence

#endif