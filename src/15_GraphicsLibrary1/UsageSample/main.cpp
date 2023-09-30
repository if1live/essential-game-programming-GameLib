#include "GameLib/Framework.h"
#include "Library/Vector3.h"
#include "Library/Vector2.h"
#include "Library/Matrix44.h"
#include "Library/Model.h"
#include "Library/Batch.h"
#include "Library/IndexBuffer.h"
#include "Library/VertexBuffer.h"
#include "Library/Camera.h"
#include "GameLib/Math.h"
using namespace GameLib;

const Vector3 gPositions[ 4 ] = {
	Vector3( -10.0, -10.0, 0.0 ),
	Vector3( -10.0, 10.0, 0.0 ),
	Vector3( 10.0, -10.0, 0.0 ),
	Vector3( 10.0, 10.0, 0.0 ),
};

const Vector2 gUVs[ 4 ] = {
	Vector2( 0.0, 0.0 ),
	Vector2( 0.0, 1.0 ),
	Vector2( 1.0, 0.0 ), 
	Vector2( 1.0, 1.0 ),
};

const unsigned gIndices[ 6 ] = {
	0, 1, 2,
	3, 1, 2,
};

bool gFirstFrame = true;
VertexBuffer* gVertexBuffer;
IndexBuffer* gIndexBuffer;
GameLib::Texture* gTexture;
Batch* gBatch;
Model* gModel;
Camera* gCamera;
int gCount;

namespace GameLib{
	void Framework::update(){
		if ( gFirstFrame ){
			setFrameRate( 60 );
			gFirstFrame = false;
			//���_�o�b�t�@����
			gVertexBuffer = new VertexBuffer( 4 );
			for ( int i = 0; i < 4; ++i ){
				gVertexBuffer->setPosition( i, gPositions[ i ] );
				gVertexBuffer->setUV( i, gUVs[ i ] );
			}
			//�C���f�N�X�o�b�t�@����
			gIndexBuffer = new IndexBuffer( 6 );
			for ( int i = 0; i < 6; ++i ){
				gIndexBuffer->setIndex( i, gIndices[ i ] );
			}
			//�e�N�X�`������
			createTexture( &gTexture, "grid.tga" );
			//�o�b�`����
			gBatch = new Batch( gVertexBuffer, gIndexBuffer, gTexture, BLEND_OPAQUE );
			//���f������
			gModel = new Model( gBatch );
			//�J��������
			gCamera = new Camera();
		}
		//�J�����X�V
		gCamera->setPosition( Vector3(
			sin( gCount ) * 20.0,
			10.0,
			cos( gCount ) * 20.0 ) );

		Matrix44 pvm;
		gCamera->createPerspectiveViewMatrix( &pvm );
		//���f���̉�]�X�V
		gModel->setAngle( Vector3( gCount, gCount * 2, gCount * 3 ) );
		//�`��
		gModel->draw( pvm );

		++gCount;

		//��n���B��������ԂƋt�ɏ����̂��ʂ̋Z���B
		//�g���Ă�������ɏ����āA�g���Ă�����̂͌�ɏ����ׂ��ł���B
		if ( isEndRequested() ){
			SAFE_DELETE( gCamera );
			SAFE_DELETE( gModel );
			SAFE_DELETE( gBatch );
			SAFE_DELETE( gIndexBuffer );
			SAFE_DELETE( gVertexBuffer );
			destroyTexture( &gTexture );
		}
	}
}
