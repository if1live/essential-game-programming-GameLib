#include "GameLib/Framework.h"
using namespace GameLib;

#include "State.h"
#include "File.h"
#include "Image.h"
#include "Sequence/Parent.h"

//�O���[�o���ϐ�
Sequence::Parent* gRootSequence = 0; //�������V�[�P���X
int gCounter = 0; //���C�����[�v��������񐔂𐔂���J�E���^

//���[�U�����֐��B���g��mainLoop()�Ɋۓ���
namespace GameLib{
	void Framework::update(){
		if ( !gRootSequence ){
			gRootSequence = new Sequence::Parent();
		}
		//�t���[�����[�g����
		setFrameRate( 60 ); //���Ăׂ΂����̂����ʓ|�Ȃ̂ŌĂ�ł��܂��B

		if ( gCounter % 60 == 0 ){ //60�t���[���Ɉ��t���[�����[�g�\��
			cout << " FrameRate:" << frameRate() << endl;
		}
		++gCounter;

		gRootSequence->update();

		//�I������(q�������ꂽ���A�}�E�X�Ł~�{�^�����@���ꂽ��)
		if ( isKeyOn( 'q' ) ){
			requestEnd();
		}
		if ( isEndRequested() ){
			SAFE_DELETE( gRootSequence );
		}
	}
}


