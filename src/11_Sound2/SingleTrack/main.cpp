#include "GameLib/Framework.h"

//�y����`
struct Note{
	int mName; //�h���~�B0��65Hz�̃h�B24��������^�񒆂̃h�B
	int mLength; // 100����1�b�P��
	int mVolume; // 0����100
};

const int C = 24;
const int Cis = 25;
const int D = 26;
const int Dis = 27;
const int E = 28;
const int F = 29;
const int Fis = 30;
const int G = 31;
const int Gis = 32;
const int A = 33;
const int Ais = 34;
const int H = 35;

const Note gNotes[] = {
	{ E, 25, 20 },
	{ Dis, 25, 20 },

	{ E, 25, 20 },
	{ Dis, 25, 20 },
	{ E, 25, 20 },
	{ H-12, 25, 20 },
	{ D, 25, 20 },
	{ C, 25, 20 },

	{ A-12, 75, 20 },
	{ C-12, 25, 20 },
	{ E-12, 25, 20 },
	{ A-12, 25, 20 },

	{ H-12, 75, 20 },
	{ E-12, 25, 20 },
	{ Gis-12, 25, 20 },
	{ H-12, 25, 20 },

	{ C, 75, 20 },
};
const int gNoteNumber = 17;

void createSound( 
short* data, //�o��
int waveWidth, //�g�̕�=����
int note, //���ԍ�0��261Hz�̃h�B12��522Hz�̃h�B�ƌ��������̉��ԍ�
int sampleNumber ){ //�f�[�^��
	//���g���e�[�u���B2��12�捪(1.05946�{)�Â���Ă���
	int frequencyTable[] = {
		261, 277, 293, 311, 329, 349, 370, 392, 415, 440, 466, 494,
	};
	int octave = note / 12; //�I�N�^�[�u������B���̐��������g��2�{���Ă��B
	int f = frequencyTable[ note % 12 ]; //�\�������ăh���~�����߂�

	//2�I�N�^�[�u���ɂ��炷
	octave -= 2;
	if ( octave >= 0 ){
		f <<= octave; //octave��2�Ȃ�4�{���������A����͍��V�t�g2���B
	}else{
		f >>= -octave;
	}

	//i�Ɋ|����搔���v�Z���悤
	/*
	1 �~�������g�� = 44100 / (i�������Ŕg�������߂邩)
	2 (i�������Ŕg�������߂邩) = �g�̕� / i�Ɋ|����搔
	
	1,2�̎�����A

	i�Ɋ|����搔 = �g�̕� * �~�������g�� / 44100
	*/

	//i�Ɋ|���鐔�ɒ���(��̎��Q��)
	int m = waveWidth * f / 44100;

	//��̓f�[�^���߂邾��
	for ( int i = 0; i < sampleNumber; ++i ){
		data[ i ] = static_cast< short >( (i*m) % waveWidth );
	}
}

bool gFirstFrame = true;

namespace GameLib{
	void Framework::update(){
		sleep( 16 );
		if ( gFirstFrame ){ //�ŏ��̃t���[�������������Ȃ�
			gFirstFrame = false;
			short* wave = new short[ 44100 * 60 ]; //�Ƃ肠����1���B�z�񂪑傫������new�������������B

			//�y�������߂��܂��B
			int time = 0;
			for ( int i = 0; i < gNoteNumber; ++i ){
				const Note& n = gNotes[ i ];
				createSound( 
					wave + time, 
					n.mVolume * 300,
					n.mName,
					n.mLength * 441 );
				time += n.mLength * 441;
			}
			//�c��������0�𖄂߂�
			for ( int i = time; i < 44100 * 60; ++i ){
				wave[ i ] = 0;
			}
			playSound( wave, 44100 * 60 );
			SAFE_DELETE_ARRAY( wave );
		}
	}
}