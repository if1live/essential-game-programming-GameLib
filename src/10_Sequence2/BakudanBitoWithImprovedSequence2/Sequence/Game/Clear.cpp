#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Ready.h"
#include "Image.h"

namespace Sequence{
namespace Game{

Clear::Clear() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Clear::~Clear(){
	SAFE_DELETE( mImage );
}

Child* Clear::update( Parent* parent ){
	Child* next = this;
	if ( mCount == 60 ){ //1�b�҂� 
		if ( parent->hasFinalStageCleared() ){ //�ŏI�ʂ��N���A�����Ȃ�G���f�B���O��
			parent->moveTo( Parent::NEXT_ENDING ); 
		}else{
			next = new Ready;
		}
	}
	//�`��
	//�܂��Q�[����ʂ�`��
	parent->drawState();
	//��ɏj���̃��b�Z�[�W��`��
	mImage->draw();
	//�N���A�[�Ǝ���`��
	Framework::instance().drawDebugString( 0, 0, "�ر�!" );

	++mCount;

	return next;
}

} //namespace Game
} //namespace Sequence
