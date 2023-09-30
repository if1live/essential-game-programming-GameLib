#include "GameLib/Framework.h"
using namespace GameLib;

#include "State.h"
#include "File.h"

//�֐��v���g�^�C�v
void mainLoop();

//�O���[�o���ϐ�
State* gState = 0;
bool gPrevInputS = false;
bool gPrevInputA = false;
bool gPrevInputW = false;
bool gPrevInputZ = false;

//���[�U�����֐��B���g��mainLoop()�Ɋۓ���
namespace GameLib{
	void Framework::update(){
		mainLoop();
	}
}

void mainLoop(){
	Framework f = Framework::instance();
	//�ŏ��̃t���[���͏�����
	if ( !gState ){ 
		File file( "stageData.txt" );
		if ( !( file.getData() ) ){ //�f�[�^�Ȃ��I
			cout << "stage file could not be read." << endl;
			return;
		}
		gState = new State( file.getData(), file.getSize() );
	}
	bool cleared = false;
	//���C�����[�v
	//�N���A�`�F�b�N
	if ( gState->hasCleared() ){
		cleared = true;
	}
	//���͎擾
	int dx = 0;
	int dy = 0;
	bool inputA = f.isKeyOn( 'a' );
	bool inputS = f.isKeyOn( 's' );
	bool inputW = f.isKeyOn( 'w' );
	bool inputZ = f.isKeyOn( 'z' );
	if ( inputA && ( !gPrevInputA ) ){
		dx -= 1;
	}else if ( inputS && ( !gPrevInputS ) ){
		dx += 1;
	}else if ( inputW && ( !gPrevInputW ) ){
		dy -= 1;
	}else if ( inputZ && ( !gPrevInputZ ) ){
		dy += 1;
	}
	gPrevInputA = inputA;
	gPrevInputS = inputS;
	gPrevInputW = inputW;
	gPrevInputZ = inputZ;
	//�X�V
	gState->update( dx, dy );
	//�`��
	gState->draw();

	if ( cleared ){
		//�j���̃��b�Z�[�W
		cout << "Congratulation! you win." << endl;
		delete gState;
		gState = 0;
	}
	//�I������
	if ( f.isKeyOn( 'q' ) ){
		f.requestEnd();
	}
	if ( f.isEndRequested() ){
		if ( gState ){
			delete gState;
			gState = 0;
		}
	}
}


