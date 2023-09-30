#include "GameLib/GameLib.h"
using namespace GameLib;

#include "GameLib/Framework.h"
#include "Image.h"
#include "State.h"
#include "Sequence/Game/Menu.h"
#include "Sequence/Game/Parent.h"

namespace Sequence{
namespace Game{

Menu::Menu() : mImage( 0 ), mMenuCursor( 0 ){
	mImage = new Image( "data/image/menu.dds" );
}

Menu::~Menu(){
	SAFE_DELETE( mImage );
}

void Menu::update( Parent* parent ){
	//1:���Ȃ���
	//2:�ʃZ����
	//3:�^�C�g����ʂ�
	//4:�߂�
	//�Y�����������Ɩʓ|�Ȃ̂�0������Ă���
	char numberChars[] = { '0', '1', '2', '3', '4' };
	int inputNumber = 0;
	for ( int i = 0; i < 5; ++i ){
		if ( Framework::instance().isKeyTriggered( numberChars[ i ] ) ){
			inputNumber = i;
		}
	}
	switch ( inputNumber ){
		case 1: //���Ȃ���
			parent->getState()->reset();
			parent->moveTo( Parent::SEQ_PLAY );
			break;
		case 2: //�ʃZ����
			parent->moveTo( Parent::SEQ_STAGE_SELECT );
			break;
		case 3: //�^�C�g����
			parent->moveTo( Parent::SEQ_TITLE );
			break;
		case 4: //���̂܂ܖ߂�
			parent->moveTo( Parent::SEQ_PLAY );
			break;
		default: //���̑��͖���
			break;
	}

	//�`��
	//�܂��Q�[�����
	parent->getState()->draw();
	//��ɏd�˂�
	mImage->draw();
}



} //namespace Game
} //namespace Sequence