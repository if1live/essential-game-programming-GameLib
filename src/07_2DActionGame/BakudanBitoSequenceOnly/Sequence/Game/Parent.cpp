#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Ready.h"
#include "Sequence/Game/Pause.h"
#include "Sequence/Game/Play.h"
#include "Sequence/Game/Failure.h"
#include "Sequence/Game/Judge.h"
#include "State.h"
#include "File.h"
#include <sstream>

namespace Sequence{
namespace Game{

Parent::Parent() : 
mState( 0 ),
mStageID( 1 ), //最初は1面
mLife( INITIALI_LIFE_NUMBER ),
mNextSequence( NEXT_NONE ),
mClear( 0 ),
mReady( 0 ),
mPause( 0 ),
mPlay( 0 ),
mFailure( 0 ),
mJudge( 0 ){
	//最初はReady
	mReady = new Ready();
}

Parent::~Parent(){
	SAFE_DELETE( mState );
	SAFE_DELETE( mClear );
	SAFE_DELETE( mReady );
	SAFE_DELETE( mPause );
	SAFE_DELETE( mPlay );
	SAFE_DELETE( mFailure );
	SAFE_DELETE( mJudge );
}

void Parent::update( GrandParent* parent ){
	if ( mClear ){
		mClear->update( this );
	}else if ( mReady ){
		mReady->update( this );
	}else if ( mPause ){
		mPause->update( this );
	}else if ( mPlay ){
		mPlay->update( this );
	}else if ( mFailure ){
		mFailure->update( this );
	}else if ( mJudge ){
		mJudge->update( this );
	}else{
		HALT( "bakana!" ); //ありえない
	}
	//遷移判定
	switch ( mNextSequence ){
		case NEXT_CLEAR:
			ASSERT( !mClear && !mReady && !mPause && mPlay && !mFailure && !mJudge );
			SAFE_DELETE( mPlay );
			mClear = new Clear();
			++mStageID; //次のステージへ
			break;
		case NEXT_READY:
			ASSERT( !mReady && !mPause && !mPlay && ( mFailure || mClear ||  mJudge ) );
			SAFE_DELETE( mFailure );
			SAFE_DELETE( mClear );
			SAFE_DELETE( mJudge );
			mReady = new Ready();
			break;
		case NEXT_PAUSE:
			ASSERT( !mClear && !mReady && !mPause && mPlay && !mFailure && !mJudge );
			SAFE_DELETE( mPlay );
			mPause = new Pause();
			break;
		case NEXT_PLAY:
			ASSERT( !mClear && ( mReady || mPause ) && !mPlay && !mFailure && !mJudge );
			SAFE_DELETE( mReady );
			SAFE_DELETE( mPause );
			mPlay = new Play();
			break;
		case NEXT_FAILURE:
			ASSERT( !mClear && !mReady && !mPause && mPlay && !mFailure && !mJudge );
			SAFE_DELETE( mPlay );
			mFailure = new Failure();
			--mLife;
			break;
		case NEXT_JUDGE:
			ASSERT( !mClear && !mReady && !mPause && mPlay && !mFailure && !mJudge );
			SAFE_DELETE( mPlay );
			mJudge = new Judge();
			break;
		case NEXT_ENDING:
			ASSERT( mClear && !mReady && !mPause && !mPlay && !mFailure && !mJudge );
			SAFE_DELETE( mClear );
			parent->moveTo( GrandParent::NEXT_ENDING );
			break;
		case NEXT_GAME_OVER:
			ASSERT( !mClear && !mReady && !mPause && !mPlay && mFailure && !mJudge );
			SAFE_DELETE( mFailure );
			parent->moveTo( GrandParent::NEXT_GAME_OVER );
			break;
		case NEXT_TITLE:
			ASSERT( !mClear && !mReady && ( mPause || mJudge ) && !mPlay && !mFailure );
			SAFE_DELETE( mPause );
			SAFE_DELETE( mJudge );
			parent->moveTo( GrandParent::NEXT_TITLE );
			break;
	}
	mNextSequence = NEXT_NONE;
}

void Parent::moveTo( NextSequence next ){
	ASSERT( mNextSequence == NEXT_NONE );
	mNextSequence = next;
}

State* Parent::state(){
	return mState;
}

bool Parent::hasFinalStageCleared() const {
	return ( mStageID > FINAL_STAGE );
}

int Parent::lifeNumber() const {
	return mLife;
}

//Parent::ModeをParent::Modeに変換。下流シーケンスにParentを見せない。
Parent::Mode Parent::mode() const {
	Mode r = MODE_NONE;
	switch ( GrandParent::instance()->mode() ){
		case GrandParent::MODE_1P: r = MODE_1P; break;
		case GrandParent::MODE_2P: r = MODE_2P; break;
		default: ASSERT( false ); break;
	}
	return r;
}

void Parent::startLoading(){
	SAFE_DELETE( mState );
	//ロードします
	std::ostringstream oss;
	if ( mode() == MODE_1P ){
		oss << "data/stageData/" << mStageID << ".txt";
	}else{
		oss << "data/stageData/9.txt"; //二人用ステージのかわりに9面をロード 
	}
	File file( oss.str().c_str() ); //これでconst char*が取れる
	mState = new State( file.data(), file.size() );
}

} //namespace Game
} //namespace Sequence