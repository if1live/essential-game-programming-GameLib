#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Play.h"
#include "Sequence/Game/Parent.h"
#include "Game/State.h"

namespace Sequence{
namespace Game{

Play::Play(){ //�������Ȃ�
}

Play::~Play(){ //�������Ȃ�
}

//�Q�[���{��
void Play::update( Parent* parent ){
	Framework f = Framework::instance();;

	State* state = parent->state();

	bool cleared = state->hasCleared();
	bool die1P = !state->isAlive( 0 );
	bool die2P = !state->isAlive( 1 );
	//�Ƃ肠�����f�o�O�R�}���h�Ńe�X�g����B
	if ( f.isKeyTriggered( '1' ) ){ //1P�E��
		die2P = true;
	}else if ( f.isKeyTriggered( '2' ) || f.isKeyTriggered( 'x' ) ){ //2P�E��
		die1P = true;
	}else if ( f.isKeyTriggered( 'c' ) ){
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
	if ( f.isKeyTriggered( ' ' ) ){
		parent->moveTo( Parent::NEXT_PAUSE );
	}
	//�X�V
	state->update();
	//�`��
	state->draw();
}

} //namespace Game
} //namespace Sequence
