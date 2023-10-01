#include "GameLib/GameLib.h"
using namespace GameLib;

#include "GameLib/Framework.h"
#include "Sequence/Title.h"
#include "Sequence/Parent.h"
#include "Image.h"

namespace Sequence{

Title::Title() : mImage( 0 ){
	mImage = new Image( "data/image/title.dds" );
}

Title::~Title(){
	SAFE_DELETE( mImage );
}

void Title::update( Parent* parent ){
	//入力取得
	if ( Framework::instance().isKeyTriggered( ' ' ) ){
		parent->moveTo( Parent::SEQ_STAGE_SELECT );
	}
	//描画
	mImage->draw();
}

} //namespace Sequence