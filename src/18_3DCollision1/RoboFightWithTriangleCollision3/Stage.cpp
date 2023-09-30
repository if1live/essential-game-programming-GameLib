#include "GameLib/Framework.h"

#include "Stage.h"
#include "Triangle.h"
#include "Library/GraphicsDatabase.h"
#include "Library/Batch.h"
#include "Library/IndexBuffer.h"
#include "Library/VertexBuffer.h"
#include "Library/Model.h"
#include "Library/Matrix44.h"
#include "Library/Vector3.h"
#include <limits>
using namespace std;

Stage::Stage() : mDatabase( 0 ), mModel( 0 ), mTriangles( 0 ){
	mDatabase = new GraphicsDatabase( "stage.txt" );
	mModel = mDatabase->createModel( "draw" );

	//�Փˏ����p�f�[�^�쐬
	const Batch* batch = mDatabase->batch( "collision" );
	const IndexBuffer* ib = batch->indexBuffer();
	const VertexBuffer* vb = batch->vertexBuffer();

	mTriangleNumber = ib->size() / 3;
	mTriangles = new Triangle[ mTriangleNumber ];
	for ( int i = 0; i < mTriangleNumber; ++i ){
		unsigned i0 = ib->index( i * 3 + 0 );
		unsigned i1 = ib->index( i * 3 + 1 );
		unsigned i2 = ib->index( i * 3 + 2 );
		mTriangles[ i ].setPosition( 
			*vb->position( i0 ),
			*vb->position( i1 ),
			*vb->position( i2 ) );
	}
}

Stage::~Stage(){
	SAFE_DELETE( mModel );
	SAFE_DELETE( mDatabase );
	SAFE_DELETE_ARRAY( mTriangles );
}

void Stage::draw( const Matrix44& pvm ) const {
	mModel->draw( pvm );
}

void Stage::restrictMove( Vector3* v, const Vector3& p ) const {
	Vector3 vCopy = *v;
	//��ꃋ�[�v
	double t = numeric_limits< double >::max();
	int minI = -1;
	for ( int i = 0; i < mTriangleNumber; ++i ){
		double tt = mTriangles[ i ].getIntersectionTime( p, vCopy );
		if ( tt < t ){
			t = tt;
			minI = i;
		}
	}
	if ( t >= 0.f && t <= 1.f ){ //��������
		mTriangles[ minI ].restrictMove( &vCopy, p );
	}
	//��񃋁[�v
	t = numeric_limits< double >::max();
	minI = -1;
	for ( int i = 0; i < mTriangleNumber; ++i ){
		double tt = mTriangles[ i ].getIntersectionTime( p, vCopy );
		if ( tt < t ){
			t = tt;
			minI = i;
		}
	}
	if ( t >= 0.f && t <= 1.f ){ //��������
		mTriangles[ minI ].restrictMove( &vCopy, p );
	}
	//��O���[�v�B������悤�Ȃ�Ȃ��������Ƃɂ��ďI���B
	t = numeric_limits< double >::max();
	minI = -1;
	for ( int i = 0; i < mTriangleNumber; ++i ){
		if ( mTriangles[ i ].isIntersect( p, vCopy ) ){
			v->set( 0.0, 0.0, 0.0 );
			return;
		}
	}
	*v = vCopy;
}
