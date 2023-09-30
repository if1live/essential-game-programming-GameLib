#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Title.h"
#include "Sequence/Parent.h"
#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"

namespace Sequence{

Title::Title() : mImage( 0 ), mCursorPosistion( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Title::~Title(){
	SAFE_DELETE( mImage );
}

void Title::update( Parent* parent ){
	//���͎擾
	if ( Pad::isTriggered( Pad::U ) ){
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
		parent->moveTo( Parent::NEXT_GAME );
		if ( mCursorPosistion == 0 ){
			parent->setMode( Parent::MODE_1P );
		}else if ( mCursorPosistion == 1 ){
			parent->setMode( Parent::MODE_2P );
		}else{
			HALT( "arienai" );
		}
		//���艹
		SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
	}
	//�`��
	mImage->draw();
	//�����o��
	Framework f = Framework::instance();
	f.drawDebugString( 0, 0, "[����] : �޸������" );
	f.drawDebugString( 1, 2, "��� �� ����" );
	f.drawDebugString( 1, 3, "��� �� �ۼ��" );
	//�J�[�\��������
	f.drawDebugString( 0, mCursorPosistion + 2, ">" );
}

} //namespace Sequence