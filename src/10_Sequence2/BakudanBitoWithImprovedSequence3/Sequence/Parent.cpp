#include "GameLib/GameLib.h"
#include "Sequence/Parent.h"
#include "Sequence/Title.h"

namespace Sequence{

Parent* Parent::mInstance = 0;

void Parent::create(){
	ASSERT( !mInstance );
	mInstance = new Parent();
}

void Parent::destroy(){
	ASSERT( mInstance );
	SAFE_DELETE( mInstance );
}

Parent* Parent::instance(){
	return mInstance;
}

Parent::Parent() : 
mMode( MODE_NONE ),
mChild( 0 ){
	//�ŏ��ɍ��̂̓^�C�g��
	mChild = new Title();
}

Parent::~Parent(){
	//�c���Ă���Ζ��E
	SAFE_DELETE( mChild );
}

void Parent::update(){
	Base* nextChild = mChild->update( this );
	if ( nextChild != mChild ){
		//�J�ڔ���
		Child* casted = dynamic_cast< Child* >( nextChild );
		ASSERT( casted ); //���s�͂��肦�Ȃ��B
		SAFE_DELETE( mChild );
		mChild = casted;
	}
	nextChild = 0; //�O�̂���
}

void Parent::setMode( Mode mode ){
	mMode = mode;
}

Parent::Mode Parent::mode() const {
	return mMode;
}

} //namespace Sequence
