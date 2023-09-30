#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Parent.h"

namespace Sequence{
namespace Game{

Pause::Pause() : mImage( 0 ),mCursorPosistion( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Pause::~Pause(){
	SAFE_DELETE( mImage );
}

void Pause::update( Parent* parent ){
	//0:���Ȃ���
	//1:�ʃZ����
	//2:�^�C�g����ʂ�
	//3:�߂�
	if ( Pad::isTriggered( Pad::U) ){
		--mCursorPosistion;
		if ( mCursorPosistion < 0 ){ //�}�C�i�X�͍ő�l�Ƀ��[�v
 			mCursorPosistion = 1;
		}
		//�J�[�\����
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosistion;
		if ( mCursorPosistion > 1 ){ //1���z������0�Ƀ��[�v
			mCursorPosistion = 0;
		}
		//�J�[�\����
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::A ) ){
		if ( mCursorPosistion == 0 ){ //�Q�[���ɖ߂�
			parent->moveTo( Parent::NEXT_PLAY );
		}else if ( mCursorPosistion == 1 ){ //�^�C�g���֖߂�
			parent->moveTo( Parent::NEXT_TITLE );
		}
		//���艹
		SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
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
}



} //namespace Game
} //namespace Sequence