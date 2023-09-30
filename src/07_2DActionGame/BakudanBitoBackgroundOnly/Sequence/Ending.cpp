#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
using namespace GameLib;

#include "Sequence/Ending.h"
#include "Sequence/Parent.h"
#include "Image.h"

namespace Sequence{

Ending::Ending() : mImage( 0 ), mCount( 0 ){
	mImage = new Image( "data/image/dummy.dds" );
}

Ending::~Ending(){
	SAFE_DELETE( mImage );
}

void Ending::update( Parent* parent ){
	if ( mCount == 120 ){ //2�b�҂� 
		parent->moveTo( Parent::NEXT_TITLE );
	}
	//�`��
	mImage->draw();
	//�����o��
	Framework f = Framework::instance();
	f.drawDebugString( 0, 0, "����ĳ! ���� �ɹްѦ ��ʼϼ�" );
	f.drawDebugString( 0, 1, "¸���� �������" );

	++mCount;
}

} //namespace Sequence