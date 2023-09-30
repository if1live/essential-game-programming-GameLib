#include "GameLib/Framework.h"
#include "GameLib/Sound/Wave.h"
#include "GameLib/Sound/Player.h"
using namespace GameLib::Sound;

Wave gWave;
Player gPlayer;

namespace GameLib{
	void Framework::update(){
		sleep( 16 );
		//gWave����Ȃ烍�[�h�J�n
		if ( !gWave ){
			gWave = Sound::Wave::create( "dokaan.wav" );
		}
		if ( !gPlayer && gWave.isReady() ){ //�v���C���[����Ń��[�h���I����Ă���Ȃ�
			if ( !gWave.isError() ){ //�G���[���`�F�b�N���悤�ˁB�t�@�C����ǂݑ��˂Ă��~�܂�Ȃ��̂ŁB
				gPlayer = Sound::Player::create( gWave );
				gPlayer.play();
			}
		}
		if ( gPlayer ){ //gPlayer������Ă���Ȃ�
			static int a = 0;
			static int b = 1;
			//�{�����[���������Ă݂����
			gPlayer.setVolume( a );
			if ( a == 0 || a == -100 ){
				b *= -1;
			}
			a += b;
			if ( !gPlayer.isPlaying() ){ //�I������̂ŉ���B
				gPlayer.release();
				gWave.release();
			}
		}
		if ( isEndRequested() ){
			gWave.release();
			gPlayer.release();
		}
	}
}
