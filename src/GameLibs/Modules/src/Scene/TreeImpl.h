#ifndef INCLUDED_GAMELIB_SCENE_TREEIMPL_H
#define INCLUDED_GAMELIB_SCENE_TREEIMPL_H

#include "GameLib/Base/Impl/ReferenceType.h"
#include "GameLib/Scene/Batch.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Matrix34.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Math/Functions.h"
#include "GameLib/Base/Impl/ReferenceType.h"
#include "GameLib/Base/Pool.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Scene/Animation.h"

#include "Scene/BatchImpl.h"
#include "Scene/Node.h"
#include "Scene/TreeTemplate.h"
#include "Scene/ContainerImpl.h"
#include "Scene/AnimationImpl.h"

namespace GameLib{
using namespace GameLib::Graphics;
namespace Scene{

class Tree::Impl : public ReferenceType{
public:
	Impl( TreeTemplate* tmpl ) : 
	mTemplate( tmpl ),
	mPosition( 0.f, 0.f, 0.f ),
	mAngle( 0.f, 0.f, 0.f ),
	mScale( 1.f, 1.f, 1.f ),
	mColor( 1.f, 1.f, 1.f ),
	mTransparency( 1.f ),
	mTime( 0.f ),
	mSpeed( 1.f ),
	mNodes( 0 ),
	mNodeNumber( 0 ){
		mTemplate->container()->refer(); //���L�R���e�i�̎Q�ƃJ�E���g�𑝂₷

		//�\�z�J�n
		mNodeNumber = tmpl->nodeNumber();
		mNodes = static_cast< Node* >( OPERATOR_NEW( sizeof( Node ) * mNodeNumber ) );
		for ( int i = 0; i < mNodeNumber; ++i ){
			const NodeTemplate* src = tmpl->node( i ); 
			Node* dst = &mNodes[ i ];
			//�R���X�g���N�g
			new ( dst ) Node( src );

			//�q�̐��𐔂���
			int child = src->mChild;
			int childNumber = 0;
			while ( child >= 0 ){
				++childNumber;
				child = tmpl->node( child )->mBrother;
			}
			Node** children = mChildrenPool.allocate( childNumber );
			//�q���[�U����
			child = src->mChild;
			int j = 0;
			while ( child >= 0 ){
				children[ j ] = mNodes + child;
				child = tmpl->node( child )->mBrother;
				++j;
			}
			//�m�[�h�ɗ^����
			dst->setChildren( children, childNumber );
		}
	}
	~Impl(){
		for ( int i = 0; i < mNodeNumber; ++i ){
			mNodes[ i ].~Node();
		}
		OPERATOR_DELETE( mNodes );

		//�e�R���e�i�̉���`�F�b�N
		Container::Impl* c = mTemplate->container(); 
		c->release();
		if ( c->referenceCount() == 0 ){ //�������Ō�̎Q�Ƃ������Ă���B�j���B
			SAFE_DELETE( c );
		}
		mTemplate = 0;
	}
	void draw(){
		//���[���h�s������
		Matrix34 w;
		w.setTranslation( mPosition );
		w.rotateY( mAngle.y );
		w.rotateX( mAngle.x );
		w.rotateZ( mAngle.z );
		w.scale( mScale );
		//�`��
		mNodes[ 0 ].draw( w, mColor, mTransparency );
	}
	void setAnimation( Animation::Impl* a ){
		if ( a ){ //���m�������
			for ( int i = 0; i < mNodeNumber; ++i ){
				mNodes[ i ].setAnimation( a->node( mNodes[ i ].name() ) );
			}
		}else{ //���m���Ȃ����
			for ( int i = 0; i < mNodeNumber; ++i ){
				mNodes[ i ].removeAnimation(); //��Ԃ�������
			}
		}
		mTime = 0.f;
	}
	void setAnimationSpeed( float s ){
		mSpeed = s;
	}
	void updateAnimation( float t ){
		if ( t >= 0.f ){
			mTime = t;
		}
		for ( int i = 0; i < mNodeNumber; ++i ){
			mNodes[ i ].updateAnimation( mTime );
		}
		mTime += mSpeed;
	}
	TreeTemplate* mTemplate;
	Vector3 mPosition;
	Vector3 mAngle;
	Vector3 mScale;
	Vector3 mColor;
	float mTransparency;
	float mTime; //�A�j������
	float mSpeed; //�A�j�����x

	Node* mNodes; //0�Ԃ����ƌ��܂��Ă���
	int mNodeNumber;

	Pool< Node* > mChildrenPool; //�e�m�[�h�����q�z��������ɏW��
};

} //namespace Scene
} //namespace GameLib

#endif