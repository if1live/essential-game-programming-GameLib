#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Image.h"
#include "Pad.h"
#include "Sequence/Game/Judge.h"
#include "Sequence/Game/Parent.h"

namespace Sequence{
namespace Game{

Judge::Judge() : mImage( 0 ), mCursorPosition( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Judge::~Judge(){
	SAFE_DELETE( mImage );
}

void Judge::update( Parent* parent ){
	if ( Pad::isTriggered( Pad::U ) ){
		--mCursorPosition;
		if ( mCursorPosition < 0 ){ //�}�C�i�X�͍ő�l�Ƀ��[�v
 			mCursorPosition = 1;
		}
	}else if ( Pad::isTriggered( Pad::D ) ){
		++mCursorPosition;
		if ( mCursorPosition > 1 ){ //1���z������0�Ƀ��[�v
			mCursorPosition = 0;
		}
	}else if ( Pad::isTriggered( Pad::A ) ){
		if ( mCursorPosition == 0 ){
			//�����ď���
			parent->moveTo( Parent::NEXT_READY );
		}else if ( mCursorPosition == 1 ){
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
	f.drawDebugString( 0, mCursorPosition + 3, ">" );
}



} //namespace Game
} //namespace Sequence