#ifndef INCLUDED_GAMELIB_SCENE_ANIMATIONIMPL_H
#define INCLUDED_GAMELIB_SCENE_ANIMATIONIMPL_H

#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
#include "GameLib/Base/Impl/ReferenceType.h"
#include "GameLib/Base/HashMap.h"
#include "GameLib/Scene/Container.h"
#include "GameLib/Scene/Animation.h"

#include "Scene/AnimationNode.h"

namespace GameLib{
namespace Scene{

class Animation::Impl : public ReferenceType{
public:
	//���̊֐��̓R���e�i���g���̂ŁA�C���N���[�h���[�v������邽�߂�Animation.cpp�ɂ����Ă���B
	Impl( PseudoXml::ConstElement&, Container::Impl& );
	~Impl(){
		mName = 0; //�R���e�i�������Ă��镶����Ȃ̂ł����ł͎Q�Ƃ��͂�������
		for ( int i = 0; i < mNodeNumber; ++i ){
			mNodes[ i ].~AnimationNode();
		}
		OPERATOR_DELETE( mNodes );
		mContainer = 0;
	}
	const char* name() const {
		return mName;
	}
	const AnimationNode* node( const char* name ) const {
		int p = mNodeMap.find( name );
		if ( mNodeMap.isEnd( p ) ){
			return 0;
		}else{
			int idx = *mNodeMap.value( p );
			return &mNodes[ idx ];
		}
	}
	Container::Impl* container(){
		return mContainer;
	}
private:
	HashMap< const char*, int > mNodeMap;
	AnimationNode* mNodes;
	int mNodeNumber;
	const char* mName;
	Container::Impl* mContainer;
};

} //namespace Scene
} //namespace GameLib

#endif

