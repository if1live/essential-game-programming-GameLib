#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Parent.h"

//���[�U�����֐��B���g��Sequence::Parent�Ɋۓ���
namespace GameLib{
	void Framework::update(){
		if ( !Sequence::Parent::instance() ){
			Sequence::Parent::create();
			setFrameRate( 60 );
		}
		Sequence::Parent::instance()->update();
		//�I������(q�������ꂽ���A�}�E�X�Ł~�{�^�����@���ꂽ��)
		if ( isKeyOn( 'q' ) ){
			requestEnd();
		}
		if ( isEndRequested() ){
			Sequence::Parent::destroy();
		}
	}
}


