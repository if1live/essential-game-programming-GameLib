#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Ready.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"
#include "State.h"

namespace Sequence{
namespace Game{

Ready::Ready() : 
mImage( 0 ), mCount( 0 ), mStarted( false ){
	mImage = new Image( "data/image/dummy.dds" );
}

Ready::~Ready(){
	SAFE_DELETE( mImage );
}

void Ready::update( Parent* parent ){
	if ( !mStarted ){
		parent->startLoading();
		mStarted = true;
	}
	if ( mCount >= 120 ){ //2�b�o������Play��
		parent->moveTo( Parent::NEXT_PLAY );
	}else if ( mCount >= 60 ){ //Go��\��
		Framework::instance().drawDebugString( 0, 1, "GO!" );
	}else{ //Ready...��\��
		Framework::instance().drawDebugString( 0, 1, "Ready..." );
	}
	//�`��
	parent->state()->draw();
	mImage->draw();
	//�����o��
	Framework::instance().drawDebugString( 0, 0, "[READY] : " );

	++mCount;
}

} //namespace Game
} //namespace Sequence