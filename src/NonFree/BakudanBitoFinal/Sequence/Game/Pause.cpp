#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "SoundManager.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Parent.h"

namespace Sequence{
namespace Game{

Pause::Pause() : mImage( 0 ), mCursorImage( 0 ), mCursorPosition( 0 ){
	mImage = new Image( "data/image/pause.tga" );
	mCursorImage = new Image( "data/image/cursor.tga" );
}

Pause::~Pause(){
	SAFE_DELETE( mImage );
	SAFE_DELETE( mCursorImage );
}

void Pause::update( Parent* parent ){
	//0:���Ȃ���
	//1:�ʃZ����
	//2:�^�C�g����ʂ�
	//3:�߂�
	if ( Pad::isTriggered( Pad::U) ){
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
		if ( mCursorPosition == 0 ){ //�Q�[���ɖ߂�
			parent->moveTo( Parent::NEXT_PLAY );
		}else if ( mCursorPosition == 1 ){ //�^�C�g���֖߂�
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
	//�J�[�\����`��
	mCursorImage->draw( 180, 220 + mCursorPosition * 48, 0, 0, 32, 32 );
}



} //namespace Game
} //namespace Sequence