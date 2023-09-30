#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "Sequence/Game/Judge.h"
#include "Sequence/Game/Parent.h"
#include "Sequence/Game/Ready.h"

namespace Sequence{
namespace Game{

Judge::Judge() : mImage( 0 ), mCursorPosistion( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Judge::~Judge(){
	SAFE_DELETE( mImage );
}

Child* Judge::update( Parent* parent ){
	Child* next = this;
	if ( Pad::isTriggered( Pad::U ) ){
		--mCursorPosistion;
		if ( mCursorPosistion < 0 ){ //�}�C�i�X�͍ő�l�Ƀ��[�v
 			mCursorPosistion = 1;
		}
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosistion;
		if ( mCursorPosistion > 1 ){ //1���z������0�Ƀ��[�v
			mCursorPosistion = 0;
		}
	}else if ( Pad::isTriggered( Pad::A ) ){
		if ( mCursorPosistion == 0 ){
			//�����ď���
			next = new Ready;
		}else if ( mCursorPosistion == 1 ){
			//�^�C�g����
			parent->moveTo( Parent::NEXT_TITLE );
		}
	}
	//�`��
	//�܂��Q�[�����
	parent->drawState();
	//��ɏd�˂�
	mImage->draw();

	//�܂��ǂ������������̂��\��
	Framework f = Framework::instance();
	f.drawDebugString( 0, 0, "[���ʲ��ò]" );

	Parent::PlayerID winner = parent->winner();
	if ( winner == Parent::PLAYER_1 ){
		f.drawDebugString( 0, 1, "1P� ��!" );
	}else if ( winner == Parent::PLAYER_2 ){
		f.drawDebugString( 0, 1, "2P� ��!" );
	}else{
		f.drawDebugString( 0, 1, "�����ż! ˷ܹ!" );
	}
	//�߂ʁ[
	f.drawDebugString( 1, 3, "��� �ۼ��" );
	f.drawDebugString( 1, 4, "��� ���� ���� �" );
	//�J�[�\��������
	f.drawDebugString( 0, mCursorPosistion + 3, ">" );

	return next;
}



} //namespace Game
} //namespace Sequence