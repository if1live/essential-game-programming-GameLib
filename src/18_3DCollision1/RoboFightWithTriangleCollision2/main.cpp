#include <sstream>
#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "Library/Vector3.h"
#include "Library/Matrix34.h"
#include "Library/Matrix44.h"
#include "Robo.h"
#include "Stage.h"
#include "Pad.h"
#include "GameLib/Math.h"
using namespace GameLib;
using namespace std;

namespace { //�������O��ԁB�ق���cpp����݂��ė~�����Ȃ����̂͂����ɓ����B

bool gFirstFrame = true;
Robo* gRobo[ 2 ]; //���{�O���[�o���ɂ����Č����B�}�l���Ȃ��łˁB
Stage* gStage; //�X�e�[�W�O���[�o���ɂ����Č����B�}�l���Ȃ��łˁB
int gCount;
Vector3 gEyePosition( 0.0, 0.0, 1.0 );
Vector3 gEyeTarget( 0.0, 0.0, 0.0 );

} //namespace{}

namespace GameLib{
	void Framework::update(){
		if ( gFirstFrame ){
			setFrameRate( 60 );
			gFirstFrame = false;
			
			Pad::create(); //�p�b�h������
			gRobo[ 0 ] = new Robo( 0 );
			gRobo[ 1 ] = new Robo( 1 );
			gStage = new Stage();
			//�����ʒu�ݒ�
			gRobo[ 0 ]->setPosition( Vector3( 0.0, 15.0, 45.0 ) );
			gRobo[ 1 ]->setPosition( Vector3( 0.0, 15.0, -45.0 ) );
			gRobo[ 0 ]->setAngleY( 180.0 );
			gRobo[ 1 ]->setAngleY( 0.0 );
		}

		//���C�����[�v
		//�����̂݌v�Z�BRobo::update()�Ɏg���̂ŁB�ł����_�f�[�^�͑O�̃t���[��
		Vector3 eyeVector;
		eyeVector.setSub( gEyeTarget, gEyePosition );

		//�ړ��\��擾
		Vector3 move[ 2 ];
		for ( int i = 0; i < 2; ++i ){
			gRobo[ i ]->getMove( &move[ i ], eyeVector );
		}
		//�Փˌ��o�pSphere�Q�b�g
		const Sphere& spRobo0 = *gRobo[ 0 ]->sphere();
		const Sphere& spRobo1 = *gRobo[ 1 ]->sphere();
		//�ړ������𐧌�
		//0�ԃ��{
		spRobo0.restrictMove( &move[ 0 ], spRobo1 );
		gStage->restrictMove( &move[ 0 ], *gRobo[ 0 ]->position() );

		//1�ԃ��{
		spRobo1.restrictMove( &move[ 1 ], spRobo0 );
		gStage->restrictMove( &move[ 1 ], *gRobo[ 1 ]->position() );

		//�ړ��\�蔽�f�Ƃ��̑�����
		for ( int i = 0; i < 2; ++i ){
			gRobo[ i ]->update( move[ i ], *gRobo[ 1-i ]->position() );
		}

		//----�`��----
		//�r���[�ϊ��s�񐶐�
		Vector3 dir;
		gRobo[ 0 ]->getDirection( &dir );
		gEyePosition = *( gRobo[ 0 ]->position() ); //�܂����{�̈ʒu
		//������6���[�g���ɂ���dir������
		Vector3 t;
		t.setMul( dir, 6.0 );
		gEyePosition -= t;
		gEyePosition.y += 4.0; //6���[�g���ォ�猩�Ă�낤
		//�����_�͋t�ɁA8���[�g���ɂ������̂𑫂��B
		gEyeTarget = *( gRobo[ 0 ]->position() );
		t.setMul( dir, 8.0 );
		gEyeTarget += t;
		Matrix34 vm;
		vm.setViewTransform( gEyePosition, gEyeTarget );

		//�����ϊ��s�񐶐�
		//�����ϊ��s��
		Matrix44 pm; //60�x�ł������낤�B
		pm.setPerspectiveTransform( 60.0, 640.0, 480.0, 1.0, 1000.0 );
		//��Z
		pm *= vm;
		//�`��
		for ( int i = 0; i < 2; ++i ){
			gRobo[ i ]->draw( pm );
		}
		gStage->draw( pm );

		++gCount;
		//��n��
		if ( isEndRequested() ){
			SAFE_DELETE( gRobo[ 0 ] );
			SAFE_DELETE( gRobo[ 1 ] );
			SAFE_DELETE( gStage );
			Pad::destroy();
		}
	}
}
