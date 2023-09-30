#include "GameLib/Framework.h"
using namespace GameLib;

#include "State.h"
#include "File.h"
#include "Image.h"

//�֐��v���g�^�C�v
void mainLoop();
void game(); //�Q�[�����[�v
void title(); //�^�C�g����ʃ��[�v

//�V�[�P���X��
enum Sequence{
	SEQUENCE_GAME,
	SEQUENCE_TITLE,
};

//�O���[�o���ϐ�
State* gState = 0; //�Q�[�����
Image* gTitleImage = 0; //�^�C�g����ʉ摜
Sequence gSequence = SEQUENCE_TITLE; //�V�[�P���X�ԍ�
int gCounter = 0;

//���[�U�����֐��B���g��mainLoop()�Ɋۓ���
namespace GameLib{
	void Framework::update(){
		mainLoop();
	}
}

void mainLoop(){
	//�t���[�����[�g����
	Framework f = Framework::instance();
	f.setFrameRate( 60 ); //���Ăׂ΂����̂����ʓ|�Ȃ̂ŌĂ�ł��܂��B

	if ( gCounter % 60 == 0 ){ //60�t���[���Ɉ��t���[�����[�g�\��
		cout << " FrameRate:" << f.frameRate() << endl;
	}
	++gCounter;

	//�V�[�P���X����
	switch ( gSequence ){
		case SEQUENCE_GAME: 
			game();
			break;
		case SEQUENCE_TITLE:
			title();
			break;
	}
	//�I������(q�������ꂽ���A�}�E�X�Ł~�{�^�����@���ꂽ��)
	if ( f.isKeyOn( 'q' ) ){
		f.requestEnd();
	}
	if ( f.isEndRequested() ){
		SAFE_DELETE( gState );
		SAFE_DELETE( gTitleImage );
	}
	f.sleep(10);
}

//�Q�[�����[�v�B�O�̃T���v���܂ł�GameLoop()�̑唼���������ɈڐA����B
void game(){
	//�ŏ��̃t���[���͏������B
	if ( !gState ){ 
		File file( "stageData.txt" );
		if ( !( file.data() ) ){ //�f�[�^�Ȃ��I
			cout << "stage file could not be read." << endl;
			return;
		}
		gState = new State( file.data(), file.size() );
	}
	bool cleared = false;
	//���C�����[�v
	//�N���A�`�F�b�N
	if ( gState->hasCleared() ){
		cleared = true;
	}
	//���͎擾
	Framework f = Framework::instance();
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
	gState->update( dx, dy );
	//�`��
	gState->draw();

	//�N���A�����Ȃ��n�����ă^�C�g����ʂ�
	if ( cleared ){
		gSequence = SEQUENCE_TITLE;
		SAFE_DELETE( gState );
	}
}

void title(){
	//�ŏ��̃t���[���͏������B
	if ( !gTitleImage ){
		gTitleImage = new Image( "title.dds" );
	}
	//���͎擾
	if ( Framework::instance().isKeyOn( ' ' ) ){
		gSequence = SEQUENCE_GAME;
		SAFE_DELETE( gTitleImage );
		return;
	}
	gTitleImage->draw( 0, 0, 0, 0, gTitleImage->width(), gTitleImage->height() );
}

