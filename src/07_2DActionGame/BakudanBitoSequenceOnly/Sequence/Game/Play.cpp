#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Play.h"
#include "Sequence/Game/Parent.h"
#include "State.h"

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
	bool die1P = false;
	bool die2P = false;
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
		}
	}
	if ( f.isKeyTriggered( ' ' ) ){
		parent->moveTo( Parent::NEXT_PAUSE );
	}
	//�X�V
	int dx = 0;
	int dy = 0;
	if ( f.isKeyOn( 'a' ) ){
		dx -= 1;
	}else if ( f.isKeyOn( 's' ) ){
		dx += 1;
	}else if ( f.isKeyOn( 'w' ) ){
		dy -= 1;
	}else if ( f.isKeyOn( 'z' ) ){
		dy += 1;
	}
	//�X�V
	state->update( dx, dy );
	//�`��
	state->draw();
}

} //namespace Game
} //namespace Sequence
