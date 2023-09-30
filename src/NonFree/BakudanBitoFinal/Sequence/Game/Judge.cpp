#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "Sequence/Game/Judge.h"
#include "Sequence/Game/Parent.h"

namespace Sequence{
namespace Game{

Judge::Judge() : 
mImage1P( 0 ), 
mImage2P( 0 ), 
mImageDraw( 0 ), 
mCursorPosition( 0 ), 
mImageCursor( 0 ){
	mImage1P = new Image( "data/image/judge1P.tga" );
	mImage2P = new Image( "data/image/judge2P.tga" );
	mImageDraw = new Image( "data/image/judgeDraw.tga" );
	mImageMenu = new Image( "data/image/judgeMenu.tga" );
	mImageCursor = new Image( "data/image/cursor.tga" );
}

Judge::~Judge(){
	SAFE_DELETE( mImage1P );
	SAFE_DELETE( mImage2P );
	SAFE_DELETE( mImageDraw );
	SAFE_DELETE( mImageMenu );
	SAFE_DELETE( mImageCursor );
}

void Judge::update( Parent* parent ){
	if ( Pad::isTriggered( Pad::U ) ){
		--mCursorPosition;
		if ( mCursorPosition < 0 ){ //�}�C�i�X�͍ő�l�Ƀ��[�v
 			mCursorPosition = 1;
		}
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosition;
		if ( mCursorPosition > 1 ){ //1���z������0�Ƀ��[�v
			mCursorPosition = 0;
		}
	}else if ( Pad::isTriggered( Pad::A ) ){
		if ( mCursorPosition == 0 ){
			//�����ď���
			parent->moveTo( Parent::NEXT_READY );
		}else if ( mCursorPosition == 1 ){
			//�^�C�g����
			parent->moveTo( Parent::NEXT_TITLE );
		}
	}
	//�`��
	//�܂��Q�[�����
	parent->drawState();
	//��ɏd�˂�
	mImageMenu->draw();

	Parent::PlayerID winner = parent->getWinner();
	if ( winner == Parent::PLAYER_1 ){
		mImage1P->draw();
	}else if ( winner == Parent::PLAYER_2 ){
		mImage2P->draw();
	}else{
		mImageDraw->draw();
	}
	mImageMenu->draw();

	mImageCursor->draw( 180, 276 + mCursorPosition * 48, 0, 0, 32, 32 );
}



} //namespace Game
} //namespace Sequence