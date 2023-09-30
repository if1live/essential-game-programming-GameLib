#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Game/Clear.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Ready.h"
#include "Sequence/Ending.h"
#include "Image.h"

namespace Sequence{
namespace Game{

Clear::Clear() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Clear::~Clear(){
	SAFE_DELETE( mImage );
}

Base* Clear::update( Parent* parent ){
	Base* next = this;
	if ( mCount == 60 ){ //1�b�҂� 
		if ( parent->hasFinalStageCleared() ){ //�ŏI�ʂ��N���A�����Ȃ�G���f�B���O��
			next = new Ending;
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
