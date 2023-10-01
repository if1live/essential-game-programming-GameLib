#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Sequence/Game/Ready.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"

namespace Sequence{
namespace Game{

Ready::Ready() : 
mImageReady( 0 ), mImageGo( 0 ), mCount( 0 ), mStarted( false ){
	mImageReady = new Image( "data/image/ready.tga" );
	mImageGo = new Image( "data/image/go.tga" );
}

Ready::~Ready(){
	SAFE_DELETE( mImageReady );
	SAFE_DELETE( mImageGo );
}

void Ready::update( Parent* parent ){
	if ( !mStarted ){
		parent->startLoading();
		mStarted = true;
	}
	//描画
	parent->drawState();
	if ( mCount >= 120 ){ //2秒経ったらPlayへ
		parent->moveTo( Parent::NEXT_PLAY );
	}else if ( mCount >= 60 ){ //Goを表示
		mImageGo->draw();
	}else{ //Ready...を表示
		mImageReady->draw();
	}

	++mCount;
}

} //namespace Game
} //namespace Sequence