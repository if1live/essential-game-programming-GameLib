#include "GameLib/GameLib.h"
using namespace GameLib;

#include "GameLib/Framework.h"
#include "Sequence/StageSelect.h"
#include "Sequence/Parent.h"
#include "Image.h"
#include "sstream"
using namespace std;

namespace Sequence{

StageSelect::StageSelect() : mImage( 0 ), mCursorPosition( 0 ){
	mImage = new Image( "data/image/stageSelect.dds" );
}

StageSelect::~StageSelect(){
	SAFE_DELETE( mImage );
}

void StageSelect::update( Parent* parent ){
	//入力取得
	int stage = 0;
	//添え字がずれると厄介なので0も入れておく
	char numberChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	for ( int i = 0; i < 10; ++i ){
		if ( Framework::instance().isKeyTriggered( numberChars[ i ] ) ){
			stage = i;
		}
	}
	if ( stage != 0 ){ //これで次へ行く
		parent->setStageID( stage );
		parent->moveTo( Parent::SEQ_GAME );
	}
	//描画
	mImage->draw();
}

} //namespace Sequence