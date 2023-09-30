#include "GameLib/GameLib.h"
using namespace GameLib;

#include "GameLib/Framework.h"
#include "Image.h"
#include "State.h"
#include "Sequence/Game/Menu.h"
#include "Sequence/Game/Parent.h"
#include "StringRenderer.h"

namespace Sequence{
namespace Game{

Menu::Menu() : mImage( 0 ), mMenuCursor( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Menu::~Menu(){
	SAFE_DELETE( mImage );
}

void Menu::update( Parent* parent ){
	Framework f = Framework::instance();
	//0:���Ȃ���
	//1:�ʃZ����
	//2:�^�C�g����ʂ�
	//3:�߂�
	if ( f.isKeyTriggered( 'w' ) ){
		--mMenuCursor;
		if ( mMenuCursor < 0 ){ //�}�C�i�X�͍ő�l�Ƀ��[�v
 			mMenuCursor = 3;
		}
	}else if ( f.isKeyTriggered( 'z' ) ){
		++mMenuCursor;
		if ( mMenuCursor > 3 ){ //3���z������0�Ƀ��[�v
			mMenuCursor = 0;
		}
	}else if ( f.isKeyTriggered( ' ' ) ){
		switch ( mMenuCursor ){
			case 0: //���Ȃ���
				parent->state()->reset();
				parent->moveTo( Parent::SEQ_PLAY );
				break;
			case 1: //�ʃZ����
				parent->moveTo( Parent::SEQ_STAGE_SELECT );
				break;
			case 2: //�^�C�g����
				parent->moveTo( Parent::SEQ_TITLE );
				break;
			case 3: //���̂܂ܖ߂�
				parent->moveTo( Parent::SEQ_PLAY );
				break;
			default: //���̑��͖���
				break;
		}
	}
	//�`��
	//�܂��Q�[�����
	parent->state()->draw();
	//��ɏd�˂�
	mImage->draw();
	//����`��
	StringRenderer* sr = StringRenderer::instance();
	sr->draw( 0, 0, "[MENU]" );
	sr->draw( 1, 1, "RETRY" );
	sr->draw( 1, 2, "GO TO STAGE SELECTION" );
	sr->draw( 1, 3, "GO TO TITLE" );
	sr->draw( 1, 4, "CONTINUE" );
	//�J�[�\��������
	sr->draw( 0, mMenuCursor + 1, ">" );
}



} //namespace Game
} //namespace Sequence