#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/GameOver.h"
#include "Sequence/Ending.h"
#include "Sequence/Title.h"

namespace Sequence{

Parent* Parent::mInstance = 0;

void Parent::create(){
	STRONG_ASSERT( !mInstance );
	mInstance = new Parent();
}

void Parent::destroy(){
	STRONG_ASSERT( mInstance );
	SAFE_DELETE( mInstance );
}

Parent* Parent::instance(){
	return mInstance;
}

Parent::Parent() : 
mNextSequence( NEXT_NONE ),
mMode( MODE_NONE ),
mTitle( 0 ),
mGame( 0 ),
mGameOver( 0 ),
mEnding( 0 ){
	//�ŏ��ɍ��̂̓^�C�g��
	mTitle = new Title();
}

Parent::~Parent(){
	//�c���Ă���Ζ��E
	SAFE_DELETE( mTitle );
	SAFE_DELETE( mGame );
	SAFE_DELETE( mGameOver );
	SAFE_DELETE( mEnding );
}

void Parent::update(){
	if ( mTitle ){
		mTitle->update( this );
	}else if ( mGame ){
		mGame->update( this );
	}else if ( mGameOver ){
		mGameOver->update( this );
	}else if ( mEnding ){
		mEnding->update( this );
	}else{
		HALT( "bakana!" ); //���肦�Ȃ�
	}
	//�J�ڔ���
	//�ʃZ����
	switch ( mNextSequence ){
		case NEXT_TITLE:
			STRONG_ASSERT( !mTitle && ( mGame || mGameOver || mEnding ) ); //�`�F�b�N
			SAFE_DELETE( mGame );
			SAFE_DELETE( mGameOver );
			SAFE_DELETE( mEnding );
			mTitle = new Title();
			break;
		case NEXT_GAME:
			STRONG_ASSERT( mTitle && !mGameOver && !mGame && !mEnding ); //�`�F�b�N
			SAFE_DELETE( mTitle );
			mGame = new Game::Parent( mMode );
			break;
		case NEXT_GAME_OVER:
			STRONG_ASSERT( mGame && !mGameOver && !mEnding && !mTitle ); //�`�F�b�N
			SAFE_DELETE( mGame );
			mGameOver = new GameOver();
			break;
		case NEXT_ENDING:
			STRONG_ASSERT( mGame && !mGameOver && !mEnding && !mTitle ); //�`�F�b�N
			SAFE_DELETE( mGame );
			mEnding = new Ending();
	}
	mNextSequence = NEXT_NONE; //�߂�
}

void Parent::moveTo( NextSequence next ){
	STRONG_ASSERT( mNextSequence == NEXT_NONE ); //����ȊO���肦�Ȃ�
	mNextSequence = next;
}

void Parent::setMode( Mode mode ){
	mMode = mode;
}

Parent::Mode Parent::mode() const {
	return mMode;
}

} //namespace Sequence
