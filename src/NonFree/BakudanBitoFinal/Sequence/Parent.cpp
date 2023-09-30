#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/GameOver.h"
#include "Sequence/Ending.h"
#include "Sequence/Title.h"
#include "SoundManager.h"

namespace Sequence{

Parent* Parent::mInstance = 0;

void Parent::create(){
	ASSERT( !mInstance );
	mInstance = new Parent();
}

void Parent::destroy(){
	ASSERT( mInstance );
	SAFE_DELETE( mInstance );
}

Parent* Parent::instance(){
	return mInstance;
}

Parent::Parent() : 
mNextSequence( NEXT_NONE ),
mMode( MODE_NONE ),
mChild( 0 ){
	SoundManager::create();
	//�ŏ��ɍ��̂̓^�C�g��
	mChild = new Title();
}

Parent::~Parent(){
	//�c���Ă���Ζ��E
	SAFE_DELETE( mChild );
	SoundManager::destroy();
}

void Parent::update(){
	mChild->update( this );
	//�J�ڔ���
	switch ( mNextSequence ){
		case NEXT_TITLE:
			SAFE_DELETE( mChild );
			mChild = new Title();
			break;
		case NEXT_GAME:
			SAFE_DELETE( mChild );
			mChild = new Game::Parent( mMode );
			break;
		case NEXT_GAME_OVER:
			SAFE_DELETE( mChild );
			mChild = new GameOver();
			break;
		case NEXT_ENDING:
			SAFE_DELETE( mChild );
			mChild = new Ending();
	}
	mNextSequence = NEXT_NONE; //�߂�
}

void Parent::moveTo( NextSequence next ){
	ASSERT( mNextSequence == NEXT_NONE ); //����ȊO���肦�Ȃ�
	mNextSequence = next;
}

void Parent::setMode( Mode mode ){
	mMode = mode;
}

Parent::Mode Parent::getMode() const {
	return mMode;
}

} //namespace Sequence
