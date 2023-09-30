#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Parent.h"
#include "Image.h"
#include "State.h"

namespace Sequence{
namespace Game{

Clear::Clear() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
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
	parent->state()->draw();
	//��ɏj���̃��b�Z�[�W��`��
	mImage->draw();
	//�N���A�[�Ǝ���`��
	Framework::instance().drawDebugString( 0, 0, "�ر�!" );

	++mCount;
}

} //namespace Game
} //namespace Sequence
