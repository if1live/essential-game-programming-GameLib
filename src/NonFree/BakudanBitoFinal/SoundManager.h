#ifndef INCLUDED_SOUND_MANAGER_H
#define INCLUDED_SOUND_MANAGER_H

#include "GameLib/Sound/Wave.h"
#include "GameLib/Sound/Player.h"

//�V���O���g��
class SoundManager{
public:
	static SoundManager* instance();
	static void create();
	static void destroy();
	bool hasLoaded(); //���[�h�I������H

	enum Bgm{
		BGM_TITLE, //�^�C�g�����
		BGM_GAME, //�Q�[����

		BGM_MAX,
	};
	enum Se{
		SE_EXPLOSION, //����
		SE_DEATH, //���S
		SE_CURSOR_MOVE, //�J�[�\���ړ�
		SE_SELECTION, //���ڌ���
		SE_SET_BOMB, //���e�ݒu��

		SE_MAX,
	};
	//BGM��炷�BBGM�͓����Ɉ������Ȃ��B�܂��A����Ƀ��[�v����B
	void playBgm( Bgm );
	void stopBgm();
	//SE��炷�BSE�͖点�Ƃ���ꂽ������B
	void playSe( Se );
private:
	SoundManager();
	~SoundManager();

	//Wave�͑O�����ă��[�h���Ă��܂��B
	GameLib::Sound::Wave mBgmWaves[ BGM_MAX ];
	GameLib::Sound::Wave mSeWaves[ SE_MAX ];
	//Player�͍Đ�������
	GameLib::Sound::Player mBgmPlayer; //���������Ă���
	static const int SE_PLAYER_MAX = 4; //�����ɖ炷SE��4�܂ŁB�Â����ɏ����Ă����B
	GameLib::Sound::Player mSePlayers[ SE_PLAYER_MAX ]; //����͔z��Ŗ��t���[���`�F�b�N���ďI����Ă���폜�B
	int mSePlayerPos; //����SE������ꏊ�B

	static SoundManager* mInstance;
};

#endif
