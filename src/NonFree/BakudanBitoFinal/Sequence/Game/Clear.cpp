#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"

namespace Sequence{
namespace Game{

Clear::Clear() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/clear.tga" );
}

Clear::~Clear(){
	SAFE_DELETE( mImage );
}

void Clear::update( Parent* parent ){
	if ( mCount == 60 ){ //1�b�҂� 
		if ( parent->hasFinalStageCleared() ){ //�ŏI�ʂ��N���A�����Ȃ�G���f�B���O��
			parent->moveTo( Parent::NEXT_ENDING ); 
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
