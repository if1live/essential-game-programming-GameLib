#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Sequence/Title.h"
#include "Sequence/Parent.h"
#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"

namespace Sequence{

Title::Title() : mImage( 0 ), mCursorImage( 0 ), mCursorPosition( 0 ){
	mImage = new Image( "data/image/title.tga" );
	mCursorImage = new Image( "data/image/cursor.tga" );
}

Title::~Title(){
	SAFE_DELETE( mImage );
	SAFE_DELETE( mCursorImage );
}

void Title::update( Parent* parent ){
	//���͎擾
	if ( Pad::isTriggered( Pad::U ) ){
		--mCursorPosition;
		if ( mCursorPosition < 0 ){ //�}�C�i�X�͍ő�l�Ƀ��[�v
 			mCursorPosition = 1;
		}
		//�J�[�\����
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosition;
		if ( mCursorPosition > 1 ){ //1���z������0�Ƀ��[�v
			mCursorPosition = 0;
		}
		//�J�[�\����
		SoundManager::instance()->playSe( SoundManager::SE_CURSOR_MOVE );
	}else if ( Pad::isTriggered( Pad::A ) ){
		parent->moveTo( Parent::NEXT_GAME );
		if ( mCursorPosition == 0 ){
			parent->setMode( Parent::MODE_1P );
		}else if ( mCursorPosition == 1 ){
			parent->setMode( Parent::MODE_2P );
		}else{
			HALT( "arienai" );
		}
		//���艹
		SoundManager::instance()->playSe( SoundManager::SE_SELECTION );
	}
	//�`��
	mImage->draw();
	//�J�[�\��
	mCursorImage->draw( 224, 392 + mCursorPosition * 38, 0, 0, 32, 32 );
}

} //namespace Sequence