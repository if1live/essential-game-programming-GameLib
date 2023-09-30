#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Loading.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"
#include "StringRenderer.h"

namespace Sequence{
namespace Game{

//���[�h���n�߂�̂�update�����炱���ł̓��[�h�͊J�n���Ȃ����A
//�u���[�h���v�Ƃ����摜�͂����Ń��[�h����B
Loading::Loading() : 
mImage( 0 ),
mCount( 0 ),
mStarted( false ){
	mImage = new Image( "data/image/dummy.dds" );
}

Loading::~Loading(){
	SAFE_DELETE( mImage );
}

void Loading::update( Parent* parent ){
	if ( !mStarted ){
		//���ǃ��[�h����̂͐e�B�Ȃ��Ȃ�State�������Ă���̂͐e������B
		parent->startLoading();
		mStarted = true;
	}else if ( mCount == 60 ){ //1�b�҂�
		parent->moveTo( Parent::SEQ_PLAY ); //Play�Ɉړ�
	}

	//�`��
	mImage->draw();
	//�����o��
	StringRenderer::instance()->draw( 0, 0, "NOW LOADING..." );

	++mCount;
}

} //namespace Game
} //namespace Sequence