#ifndef INCLUDED_GAMELIB_SCENE_NODETEMPLATE_H
#define INCLUDED_GAMELIB_SCENE_NODETEMPLATE_H

#include "Scene/BatchImpl.h"
#include "GameLib/Math/Matrix34.h"

namespace GameLib{

namespace Math{
	class Vector3;
}
namespace Scene{
using namespace Math;

//�m�[�h�̌��^�B���C�u�����̊O����͌����Ȃ��̂ł�����Impl�͂Ȃ��B
class NodeTemplate{
public:
	NodeTemplate() :
	mChild( -1 ),
	mBrother( -1 ),
	mBatch( 0 ),
	mName( 0 ){
		mTransform.setIdentity();
	}
	~NodeTemplate(){
		mBatch = 0; //���L���͎����Ă��Ȃ��̂ŒP�Ȃ�Q�Ƃ͂���
		mName = 0; //����
	}
	int mChild;
	int mBrother;
	const Batch::Impl* mBatch;
	Matrix34 mTransform;
	const char* mName;
};

} //namespace Scene
} //namespace GameLib

#endif
