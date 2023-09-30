#ifndef INCLUDED_GAMELIB_SCENE_TREETEMPLATE_H
#define INCLUDED_GAMELIB_SCENE_TREETEMPLATE_H

#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"

#include "Scene/BatchImpl.h"
#include "Scene/NodeTemplate.h"
#include "Scene/Node.h"

namespace GameLib{

namespace Math{
	class Vector3;
}
namespace PseudoXml{
	class Element;
}

namespace Scene{
using namespace Math;

//�c���[�̌��^�B���C�u�����̊O����͌����Ȃ��̂ł�����Impl�͂Ȃ��B
class TreeTemplate{
public:
	TreeTemplate( PseudoXml::ConstElement&, Container::Impl& ); //�����cpp���B
	~TreeTemplate(){
		SAFE_DELETE_ARRAY( mNodes );
		mName = 0; //�R���e�i�������Ă��镶����Ȃ̂ł����ł͎Q�Ƃ��͂�������
		mContainer = 0;
	}
	int nodeNumber() const { 
		return mNodeNumber; 
	}
	const NodeTemplate* node( int i ) const { 
		return &mNodes[ i ]; 
	}
	const char* name() const { 
		return mName; 
	}
	Container::Impl* container(){
		return mContainer;
	}
private:
	int countNode( GameLib::PseudoXml::ConstElement&, int n );
	int build( PseudoXml::ConstElement&, Container::Impl&, int nodePos );

	NodeTemplate* mNodes; //0�Ԃ����ƌ��܂��Ă���̂ō����w���|�C���^�͕s�v
	int mNodeNumber;
	const char* mName;
	Container::Impl* mContainer;
};

} //namespace Scene
} //namespace GameLib

#endif
