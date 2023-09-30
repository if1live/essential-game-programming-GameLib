#include "GameLib/GameLib.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"

#include "Library/Tree.h"
#include "Library/TreeTemplate.h"
#include "Library/NodeTemplate.h"
#include "Library/Node.h"
#include "Library/Animation.h"
#include "Library/AnimationNode.h"
#include "Library/Matrix44.h"
#include "Library/Matrix34.h"
#include "Library/Batch.h"

using namespace GameLib;
using namespace GameLib::PseudoXml;

Tree::Tree( const TreeTemplate& tmpl ) :
mPosition( 0.0, 0.0, 0.0 ),
mAngle( 0.0, 0.0, 0.0 ),
mScale( 1.0, 1.0, 1.0 ),
mNodes( 0 ),
mNodeNumber( 0 ),
mTime( 0.0 ){
	mNodeNumber = tmpl.nodeNumber();
	mNodes = new Node[ mNodeNumber ];
	for ( int i = 0; i < mNodeNumber; ++i ){
		Node& dst = mNodes[ i ];
		const NodeTemplate* src = tmpl.node( i );
		//�p�����[�^���ڂ�
		dst.setTranslation( *src->translation() );
		dst.setRotation( *src->rotation() );
		dst.setScale( *src->scale() );
		dst.setName( src->name()->c_str() );
		dst.setBatch( src->batch() );

		//[���j-�Z��`������q�z��`���ւ̕ϊ�]
		//�q�̐��𐔂���
		int child = src->child();
		int childNumber = 0;
		while ( child >= 0 ){
			++childNumber;
			child = tmpl.node( child )->brother();
		}
		//�q���A���P�[�g
		dst.setChildNumber( childNumber );
		//�q���[�U����
		child = src->child();
		int j = 0;
		while ( child >= 0 ){
			dst.setChild( j, mNodes + child );
			child = tmpl.node( child )->brother();
			++j;
		}
	}
}

Tree::~Tree(){
	SAFE_DELETE_ARRAY( mNodes );
}

void Tree::draw(
const Matrix44& pvm,
const Vector3& lightVector,
const Vector3& lightColor,
const Vector3& ambient ) const {
	Matrix34 wm;
	wm.setTranslation( mPosition );
	wm.rotateY( mAngle.y );
	wm.rotateX( mAngle.x );
	wm.rotateZ( mAngle.z );
	wm.scale( mScale );

	//���m�[�h�֓n��
	if ( mNodes ){
		mNodes[ 0 ].draw(
			pvm,
			wm,
			lightVector,
			lightColor,
			ambient );
	}
}


void Tree::setPosition( const Vector3& a ){
	mPosition = a;
}

void Tree::setAngle( const Vector3& a ){
	mAngle = a;
}

void Tree::setScale( const Vector3& a ){
	mScale = a;
}

const Vector3* Tree::position() const {
	return &mPosition;
}

const Vector3* Tree::angle() const {
	return &mAngle;
}

const Vector3* Tree::scale() const {
	return &mScale;
}

Node* Tree::node( const char* name ){
	for ( int i = 0; i < mNodeNumber; ++i ){
		if ( *( mNodes[ i ].name() ) == name ){
			return &mNodes[ i ];
		}
	}
	return 0;
}

const Node* Tree::node( const char* name ) const {
	for ( int i = 0; i < mNodeNumber; ++i ){
		if ( *( mNodes[ i ].name() ) == name ){
			return &mNodes[ i ];
		}
	}
	return 0;
}

//�A�j���[�V�������Z�b�g����
void Tree::setAnimation( const Animation* anim ){
	for ( int i = 0; i < mNodeNumber; ++i ){
		const AnimationNode* an = anim->node( mNodes[ i ].name()->c_str() );
		mNodes[ i ].setAnimation( an ); //0�ł��C�ɂ����Z�b�g�B�ނ��낵�Ȃ��Ƃ܂����B
	}
	mTime = 0.0; //���������߂�
}

//�A�j���[�V������Ԃ��X�V���A������1�i�߂�B
void Tree::update(){
	for ( int i = 0; i < mNodeNumber; ++i ){
		mNodes[ i ].update( mTime );
	}
	mTime += 1.0;
}
