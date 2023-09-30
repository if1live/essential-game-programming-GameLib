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
	if ( mCount == 60 ){ //60�t���[���ňړ�
		parent->moveTo( Parent::SEQ_STAGE_SELECT );
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
