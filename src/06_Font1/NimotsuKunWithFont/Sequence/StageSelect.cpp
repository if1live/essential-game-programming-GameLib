#include "GameLib/GameLib.h"
using namespace GameLib;

#include "GameLib/Framework.h"
#include "Sequence/StageSelect.h"
#include "Sequence/Parent.h"
#include "Image.h"
#include "StringRenderer.h"
#include "sstream"
using namespace std;

namespace Sequence{

StageSelect::StageSelect() : mImage( 0 ), mCursorPosition( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

StageSelect::~StageSelect(){
	SAFE_DELETE( mImage );
}

void StageSelect::update( Parent* parent ){
	//���͎擾
	Framework f = Framework::instance();
	if ( f.isKeyTriggered( 'w' ) ){
		--mCursorPosition;
		if ( mCursorPosition < 1-1 ){ //�}�C�i�X�͍ő�l�Ƀ��[�v
 			mCursorPosition = 9-1;
		}
	}else if ( f.isKeyTriggered( 'z' ) ){
		++mCursorPosition;
		if ( mCursorPosition > 9-1 ){ //9�ʂ��z������1�ʂɃ��[�v
			mCursorPosition = 1-1;
		}
	}else if ( f.isKeyTriggered( ' ' ) ){
		parent->setStageID( mCursorPosition + 1 ); //1�������̂��X�e�[�W�ԍ���
		parent->moveTo( Parent::SEQ_GAME );
	}
	//�`��
	mImage->draw();
	//��������
	StringRenderer* sr = StringRenderer::instance();
	sr->draw( 0, 0, "STAGE SELECTION" );
	ostringstream oss;
	for ( int i = 1; i <= 9; ++i ){
		oss << i;
		sr->draw( 1, i, oss.str().c_str() );
		oss.str( "" );
	}
	//�J�[�\��������
	sr->draw( 0, mCursorPosition + 1, ">" );
}

} //namespace Sequence