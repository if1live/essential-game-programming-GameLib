#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/GameOver.h"
#include "Sequence/Parent.h"
#include "Sequence/Title.h"
#include "Image.h"

namespace Sequence{

GameOver::GameOver() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

GameOver::~GameOver(){
	SAFE_DELETE( mImage );
}

Child* GameOver::update( Parent* ){
	Child* next = this;
	if ( mCount == 60 ){ //1�b�҂� 
		next = new Title;
	}
	//�`��
	mImage->draw();
	//�����o��
	Framework::instance().drawDebugString( 0, 0, "�ްѵ��ް ������޼��" );

	++mCount;

	return next;
}

} //namespace Sequence