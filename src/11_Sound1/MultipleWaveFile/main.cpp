#include "GameLib/Framework.h"
#include "GameLib/Sound/Wave.h"
#include "GameLib/Sound/Player.h"
using namespace GameLib::Sound;

Wave gWaveSE;
Wave gWaveBGM;
Player gPlayerSE0;
Player gPlayerSE1;
Player gPlayerBGM;

#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include <sstream>
using namespace std;

namespace GameLib{
	void Framework::update(){
		sleep( 16 );
		//SE���[�h
		if ( !gWaveSE ){
			gWaveSE = Sound::Wave::create( "dokaan.wav" );
		}
		//BGM���[�h
		if ( !gWaveBGM ){
			gWaveBGM = Sound::Wave::create( "charara.wav" );
		}
		//SE�v���C���[�쐬
		if ( !gPlayerSE0 && !gPlayerSE1 && gWaveSE.isReady() ){ //�v���C���[����Ń��[�h���I����Ă���Ȃ�
			if ( !gWaveSE.isError() ){ //�G���[���`�F�b�N���悤�ˁB�t�@�C����ǂݑ��˂Ă��~�܂�Ȃ��̂ŁB
				//1Wave����2�v���C���[�쐬
				gPlayerSE0 = Sound::Player::create( gWaveSE );
				gPlayerSE1 = Sound::Player::create( gWaveSE );
			}
		}
		//BGM�v���C���[�쐬
		if ( !gPlayerBGM && gWaveBGM.isReady() ){ //�v���C���[����Ń��[�h���I����Ă���Ȃ�
			if ( !gWaveBGM.isError() ){ //�G���[���`�F�b�N���悤�ˁB�t�@�C����ǂݑ��˂Ă��~�܂�Ȃ��̂ŁB
				gPlayerBGM = Sound::Player::create( gWaveBGM );
				gPlayerBGM.play( true ); //���[�v�Đ��B�����ɖ�B
			}
		}
		//SE��80*16�~���b���ƂɌ��݂ɖ炷�B�Е��͏������B
		if ( gPlayerSE0 && gPlayerSE1 ){
			static int a;
			if ( a % 80 == 0 ){
				gPlayerSE0.setVolume( -10 );
				gPlayerSE0.play(); 
			}
			if ( a % 80 == 40 ){
				gPlayerSE1.setVolume( -20 );
				gPlayerSE1.play(); 
			}
			++a;
		}

		static int v = 0;
		if ( Input::Manager::instance().keyboard().isTriggered( Input::Keyboard::KEY_DOWN ) ){
			--v;
			if ( v < -100 ){
				v = -100;
			}
		}
		if ( Input::Manager::instance().keyboard().isTriggered( Input::Keyboard::KEY_UP ) ){
			++v;
			if ( v > 0 ){
				v = 0;
			}
		}
		ostringstream oss;
		oss << "VOLUME: " << v;
		drawDebugString( 0, 0, oss.str().c_str() );
		if ( gPlayerBGM ){
			gPlayerBGM.setVolume( v );
		}
		if ( isEndRequested() ){
			gWaveSE.release();
			gWaveBGM.release();
			gPlayerSE0.release();
			gPlayerSE1.release();
			gPlayerBGM.release();
		}
	}
}
