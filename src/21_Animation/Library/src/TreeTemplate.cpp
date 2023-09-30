#include "GameLib/GameLib.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"

#include "Library/TreeTemplate.h"
#include "Library/NodeTemplate.h"
#include "Library/GraphicsDatabase.h"
#include "Library/Matrix44.h"
#include "Library/Matrix34.h"
#include "Library/Batch.h"

using namespace GameLib;
using namespace GameLib::PseudoXml;

TreeTemplate::TreeTemplate( Element& e, const GraphicsDatabase& db ) :
mNodes( 0 ),
mNodeNumber( 0 ){
	ASSERT( string( "Tree" ) == e.name() ); //tree����ˁH
	ASSERT( e.childNumber() == 1 ); //���[�g�m�[�h�͈�����Ȃ���ˁH�����A��͂����ˁH
	//���O�擾
	int an = e.attributeNumber();
	const string nameStr( "name" );
	for ( int i = 0; i < an; ++i ){
		Attribute a = e.attribute( i );
		if ( nameStr == a.name() ){
			mName = a.value();
		}
	}
	//�܂��m�[�h�𐔂��悤
	Element c = e.child( 0 );
	mNodeNumber = countNode( c, 0 );
	mNodes = new NodeTemplate[ mNodeNumber ];
	//�K�w���\�z
	build( c, db, 0 );
}

TreeTemplate::~TreeTemplate(){
	SAFE_DELETE_ARRAY( mNodes );
}

//�ċA
int TreeTemplate::countNode( Element& e, int n ){
	++n; //�����̕��v���X
	int cn = e.childNumber();
	for ( int i = 0; i < cn; ++i ){
		Element c = e.child( i );
		ASSERT( string( "Node" ) == c.name() ); //Node����ˁH
		n = countNode( c, n );
	}
	return n;
}

int TreeTemplate::build( Element& e, const GraphicsDatabase& db, int nodePos ){
	//�����̏������܂�
	int an = e.attributeNumber();
	NodeTemplate& node = mNodes[ nodePos ];
	for ( int i = 0; i < an; ++i ){
		Attribute a = e.attribute( i );
		string name = a.name();
		if ( name == "name" ){
			node.mName = a.value();
		}else if ( name == "batch" ){
			node.mBatch = db.batch( a.value() );
		}else if ( name == "translation" ){
			a.getDoubleValues( &node.mTranslation.x, 3 );
		}else if ( name == "rotation" ){
			a.getDoubleValues( &node.mRotation.x, 3 );
		}else if ( name == "scale" ){
			a.getDoubleValues( &node.mScale.x, 3 );
		}
	}
	++nodePos;
	ASSERT( nodePos <= mNodeNumber );  //���肦�˂�
	//�q���̏������܂�
	int cn = e.childNumber();
	int elder = -1;
	for ( int i = 0; i < cn; ++i ){
		Element c = e.child( i );
		if ( i == 0 ){ //0�ŏ��̎q�Ȃ玩���ɃZ�b�g
			node.mChild = nodePos;
		}else{ //����ȊO�Ȃ�O�̎q��
			mNodes[ elder ].mBrother = nodePos;
		}
		elder = nodePos;
		nodePos = build( c, db, nodePos );
	}
	return nodePos;
}

const string* TreeTemplate::name() const {
	return &mName;
}

const NodeTemplate* TreeTemplate::node( int i ) const {
	return &mNodes[ i ];
}

int TreeTemplate::nodeNumber() const {
	return mNodeNumber;
}

