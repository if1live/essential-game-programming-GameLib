#include "GameLib/GameLib.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
using namespace GameLib;

#include "Sequence/Game/Play.h"
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
void Play::update( Parent* parent ){
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
			parent->moveTo( Parent::NEXT_CLEAR );
		}else if ( die1P ){
			parent->moveTo( Parent::NEXT_FAILURE );
		}
	}else{ //��l�p
		if ( die1P || die2P ){ //�ǂ���������ł�Ώ��s����
			parent->moveTo( Parent::NEXT_JUDGE );
			if ( die1P && die2P ){
				parent->setWinner( Parent::PLAYER_NONE ); //�������S
			}else if ( die1P ){
				parent->setWinner( Parent::PLAYER_2 );
			}else{
				parent->setWinner( Parent::PLAYER_1 );
			}
		}
	}
	//B�{�^���Ń|�[�Y�֍s��
	if ( Pad::isTriggered( Pad::B ) ){
		parent->moveTo( Parent::NEXT_PAUSE );
	}
	//�X�V
	state->update();
	//�`��
	state->draw();
}

} //namespace Game
} //namespace Sequence
