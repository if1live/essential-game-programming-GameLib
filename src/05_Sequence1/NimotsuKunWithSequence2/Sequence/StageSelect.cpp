#include "GameLib/GameLib.h"
using namespace GameLib;

#include "GameLib/Framework.h"
#include "Sequence/StageSelect.h"
#include "Sequence/Parent.h"
#include "Image.h"

namespace Sequence{

StageSelect::StageSelect() : mImage( 0 ){
	mImage = new Image( "data/image/stageSelect.dds" );
}

StageSelect::~StageSelect(){
	SAFE_DELETE( mImage );
}

void StageSelect::update( Parent* parent ){
	//���͎擾
	int stage = 0;
	//�Y�����������Ɩ��Ȃ̂�0������Ă���
	char numberChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
	for ( int i = 0; i < 10; ++i ){
		if ( Framework::instance().isKeyTriggered( numberChars[ i ] ) ){
			stage = i;
		}
	}
	if ( stage != 0 ){ //����Ŏ��֍s��
		parent->setStageID( stage );
		parent->moveTo( Parent::SEQ_GAME );
	}
	//�`��
	mImage->draw();
}

} //namespace Sequence