#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
#include "Sequence/Title.h"

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
mMode( MODE_NONE ),
mChild( 0 ){
	//最初に作るのはタイトル
	mChild = new Title();
}

Parent::~Parent(){
	//残っていれば抹殺
	SAFE_DELETE( mChild );
}

void Parent::update(){
	Child* nextChild = mChild->update( this );
	//遷移判定
	if ( nextChild != mChild ){
		SAFE_DELETE( mChild );
		mChild = nextChild;
	}
	nextChild = 0; //念のため

}

void Parent::setMode( Mode mode ){
	mMode = mode;
}

Parent::Mode Parent::mode() const {
	return mMode;
}

} //namespace Sequence
