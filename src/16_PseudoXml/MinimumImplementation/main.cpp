#include "GameLib/Framework.h"
#include "Library/Vector3.h"
#include "Library/Vector2.h"
#include "Library/Matrix44.h"
#include "Library/Model.h"
#include "Library/GraphicsDatabase.h"
#include "Library/Camera.h"
#include "PseudoXml/Document.h"
#include "PseudoXml/Element.h"
#include "PseudoXml/Attribute.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "GameLib/Math.h"
using namespace GameLib;

bool gFirstFrame = true;
GraphicsDatabase* gGraphicsDatabase;
Model* gModel;
Camera* gCamera;
int gCount;

namespace GameLib{
	void Framework::update(){
		sleep( 16 );
		if ( gFirstFrame ){
			gFirstFrame = false;
			//�f�[�^�x�[�X�ǂݍ���
			gGraphicsDatabase = new GraphicsDatabase( "data.txt" );
			//���f�������
			gModel = gGraphicsDatabase->createModel( "batch" );
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

		//�Uxml�����o���e�X�g
		if ( GameLib::Input::Manager::instance().keyboard().isTriggered( ' ' ) ){
			using namespace PseudoXml;
			Document document;
			Element* root = document.root();
			//�������Ɉ������
			root->setChildNumber( 1 );
			Element* cameraData = root->child( 0 );
			cameraData->setName( "CameraData" );
			//�f�[�^�������
			cameraData->setChildNumber( 2 );
			Element* e = cameraData->child( 0 );
			e->setName( "Position" );
			//�ʒu�f�[�^
			e->setAttributeNumber( 3 );
			e->attribute( 0 )->set( "x", gCamera->position()->x );
			e->attribute( 1 )->set( "y", gCamera->position()->y );
			e->attribute( 2 )->set( "z", gCamera->position()->z );
			//�����_�f�[�^
			e = cameraData->child( 1 );
			e->setName( "Target" );
			e->setAttributeNumber( 3 );
			e->attribute( 0 )->set( "x", gCamera->target()->x );
			e->attribute( 1 )->set( "y", gCamera->target()->y );
			e->attribute( 2 )->set( "z", gCamera->target()->z );
			//��������
			document.write( "camera.txt" );
		}
		++gCount;

		//��n���B��������ԂƋt�ɏ����̂��ʂ̋Z���B
		//�g���Ă�������ɏ����āA�g���Ă�����̂͌�ɏ����ׂ��ł���B
		if ( isEndRequested() ){
			SAFE_DELETE( gCamera );
			SAFE_DELETE( gModel );
			SAFE_DELETE( gGraphicsDatabase );
		}
	}
}
