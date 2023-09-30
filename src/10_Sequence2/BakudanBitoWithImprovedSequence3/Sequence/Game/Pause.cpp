#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Play.h"
#include "Sequence/Title.h"

namespace Sequence{
namespace Game{

Pause::Pause() : mImage( 0 ),mCursorPosistion( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Pause::~Pause(){
	SAFE_DELETE( mImage );
}

Base* Pause::update( Parent* parent ){
	Base* next = this;
	//0:���Ȃ���
	//1:�ʃZ����
	//2:�^�C�g����ʂ�
	//3:�߂�
	if ( Pad::isTriggered( Pad::U) ){
		--mCursorPosistion;
		if ( mCursorPosistion < 0 ){ //�}�C�i�X�͍ő�l�Ƀ��[�v
 			mCursorPosistion = 1;
		}
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosistion;
		if ( mCursorPosistion > 1 ){ //1���z������0�Ƀ��[�v
			mCursorPosistion = 0;
		}
	}else if ( Pad::isTriggered( Pad::A ) ){
		if ( mCursorPosistion == 0 ){ //�Q�[���ɖ߂�
			next = new Play;
		}else if ( mCursorPosistion == 1 ){ //�^�C�g���֖߂�
			next = new Title;
		}
	}
	//�`��
	//�܂��Q�[�����
	parent->drawState();
	//��ɏd�˂�
	mImage->draw();
	//����`��
	Framework f = Framework::instance();
	f.drawDebugString( 0, 0, "[�߰��]" );
	f.drawDebugString( 1, 2, "�ްѦ��޹�" );
	f.drawDebugString( 1, 3, "���������" );
	//�J�[�\��������
	f.drawDebugString( 0, mCursorPosistion + 2, ">" );

	return next;
}



} //namespace Game
} //namespace Sequence