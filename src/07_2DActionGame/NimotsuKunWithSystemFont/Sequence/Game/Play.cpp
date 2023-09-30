#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"

#include "Sequence/Game/Play.h"
#include "Sequence/Game/Parent.h"
#include "State.h"

using namespace GameLib;

namespace Sequence{
namespace Game{

Play::Play(){ //�������Ȃ�
}

Play::~Play(){ //�������Ȃ�
}

//�Q�[���{��
void Play::update( Parent* parent ){
	State* state = parent->state();

	bool cleared = false;
	//���C�����[�v
	//�N���A�`�F�b�N
	if ( state->hasCleared() ){
		cleared = true;
	}
	//���͎擾
	//SPACE�����ꂽ�烁�j���[�֍s��
	Framework f = Framework::instance();
	if ( f.isKeyTriggered( ' ' ) ){
		parent->moveTo( Parent::SEQ_MENU );
	}else{ //�ʏ�X�V
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

		//�N���A�����Ȃ��֕�
		if ( cleared ){
			parent->moveTo( Parent::SEQ_CLEAR );
		}
	}
	//�`��
	state->draw();
}

} //namespace Game
} //namespace Sequence
