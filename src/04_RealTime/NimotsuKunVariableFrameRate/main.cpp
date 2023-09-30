#include "GameLib/Framework.h"
using namespace GameLib;

#include "State.h"
#include "File.h"

//�֐��v���g�^�C�v
void mainLoop();

//�O���[�o���ϐ�
State* gState = 0;
const int gFrameInterval = 16; //16�~���b�B62.5fps�B
unsigned gPreviousTime[ 10 ]; //10�t���[�����̎������L�^
int gCounter = 0; //���C�����[�v��������񐔂𐔂���J�E���^

//���[�U�����֐��B���g��mainLoop()�Ɋۓ���
namespace GameLib{
	void Framework::update(){
		mainLoop();
	}
}

void mainLoop(){
	//�t���[�����[�g��艻�����B�擾���������߂�΍��t��
	Framework f = Framework::instance();
	//�S�͂ŉ��Ƒ��̃v���O�����ɖ��f��������̂ŁA���������Q��
	f.sleep( 1 );
	//�t���[�����[�g�v�Z
	unsigned currentTime = f.time();
	unsigned frameInterval = currentTime - gPreviousTime[ 9 ]; //update�ɓn��
	unsigned frameInterval10 = currentTime - gPreviousTime[ 0 ];
	if ( gCounter % 60 == 0 ){ //60�t���[���Ɉ��t���[�����[�g�\��
		cout << "frameInterval:" << frameInterval10/10;
		cout << " FrameRate:" << 10*1000/frameInterval10 << endl;
	}
	++gCounter;
	//�����X�V
	for ( int i = 0; i < 9; ++i ){
		gPreviousTime[ i ] = gPreviousTime[ i + 1 ];
	}
	gPreviousTime[ 9 ] = currentTime;

	//�ŏ��̃t���[���͏������B
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
	int dx = 0;
	int dy = 0;
	if ( f.isKeyOn( 'a' ) ){
		dx -= 1;
	}else if ( f.isKeyOn( 's' ) ){
		dx += 1;
	}else if ( f.isKeyOn( 'w' ) ){
		dy -= 1;
	}else if ( f.isKeyOn( 'z' ) ){
		dy += 1;
	}
	//�X�V
	gState->update( dx, dy, frameInterval );
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


