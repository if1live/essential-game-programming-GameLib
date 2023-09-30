#include "GameLib/GameLib.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
using namespace GameLib;

#include "Sequence/Game/Play.h"
#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Failure.h"
#include "Sequence/Game/Judge.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Parent.h"
#include "Game/State.h"
#include "Pad.h"

namespace Sequence{
namespace Game{

Play::Play(){ //�������Ȃ�
}

Play::~Play(){ //�������Ȃ�
}

//�Q�[���{��
Child* Play::update( Parent* parent ){
	Child* next = this;
	Input::Keyboard kb = Input::Manager::instance().keyboard();

	State* state = parent->state();

	bool cleared = state->hasCleared();
	bool die1P = !state->isAlive( 0 );
	bool die2P = !state->isAlive( 1 );
	//�Ƃ肠�����f�o�O�R�}���h�Ńe�X�g����B
	if ( kb.isTriggered( '1' ) ){ //2P�E��
		die2P = true;
	}else if ( kb.isTriggered( '2' ) ){ //1P�E��
		die1P = true;
	}else if ( kb.isTriggered( 'c' ) ){
		cleared = true;
	}
	//SPACE�����ꂽ��|�[�Y�֍s��
	//�N���A�����Ȃ��֕�
	if ( parent->mode() == Parent::MODE_1P ){
		if ( cleared && !die1P ){
			parent->goToNextStage();
			next = new Clear;
		}else if ( die1P ){
			parent->reduceLife();
			next = new Failure;
		}
	}else{ //��l�p
		if ( die1P || die2P ){ //�ǂ���������ł�Ώ��s����
			next = new Judge;
			if ( die1P && die2P ){
				parent->setWinner( Parent::PLAYER_NONE ); //�������S
			}else if ( die1P ){
				parent->setWinner( Parent::PLAYER_2 );
			}else{
				parent->setWinner( Parent::PLAYER_1 );
			}
		}
	}
	if ( ( next == this ) && Pad::isTriggered( Pad::B ) ){ //B�{�^���Ń|�[�Y�֍s���B�������܂��ڂ��ĂȂ���������
		next = new Pause;
	}
	//�X�V
	state->update();
	//�`��
	state->draw();

	return next;
}

} //namespace Game
} //namespace Sequence
