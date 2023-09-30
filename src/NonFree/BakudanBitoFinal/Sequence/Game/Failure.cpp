#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Sequence/Game/Failure.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"

namespace Sequence{
namespace Game{

Failure::Failure() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/failure.tga" );
}

Failure::~Failure(){
	SAFE_DELETE( mImage );
}

void Failure::update( Parent* parent ){
	if ( mCount == 60 ){ //1�b�҂� 
		if ( parent->getLifeNumber() == 0 ){
			parent->moveTo( Parent::NEXT_GAME_OVER ); 
		}else{
			parent->moveTo( Parent::NEXT_READY );
		}
	}
	//�`��
	//�܂��Q�[����ʂ�`��
	parent->drawState();
	//��ɏj���̃��b�Z�[�W��`��
	mImage->draw();

	++mCount;
}

} //namespace Game
} //namespace Sequence
