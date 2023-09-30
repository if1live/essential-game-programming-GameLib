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

//�E��y��
const Note gTrack1[] = {
	{ 0, 150, 0 }, //�x��
	{ D, 50, 15 },

	{ G, 50, 17 },
	{ A, 50, 20 },
	{ H, 50, 22 },
	{ C+12, 50, 25 },

	{ D+12, 50, 30 },
	{ C+12, 25, 25 },
	{ H, 25, 22 },
	{ A, 50, 20 },
	{ D+12, 50, 22 },

	{ G+12, 50, 35 },
	{ Fis+12, 50, 32 },
	{ E+12, 50, 20 },
	{ D+12, 50, 28 },

	{ D+12, 12, 20 },
	{ Cis+12, 13, 20 },
	{ D+12, 12, 20 },
	{ Cis+12, 13, 20 },
	{ H, 25, 20 },
	{ Cis+12, 25, 20 },
	{ A, 25, 25 },
	{ H, 25, 15 },
	{ G, 25, 15 },
	{ A, 25, 15 },

	{ A+12, 100, 15 },
	{ A+12, 100, 15 },

	{ H+12, 100, 20 },
	{ H+12, 100, 20 },

	{ A+12, 50, 25 },
	{ H+12, 50, 25 },
	{ A+12, 50, 22 },
	{ G+12, 50, 20 },

	{ G+12, 12, 15 },
	{ Fis+12, 13, 15 },
	{ G+12, 12, 15 },
	{ Fis+12, 13, 15 },
	{ E+12, 25, 15 },
	{ Fis+12, 25, 15 },
	{ D+12, 100, 12 },
};

//����y��
const Note gTrack2[] = {
	{ 0, 200, 0 }, //�x��

	{ G-12, 100, 10 },
	{ G, 100, 10 },

	{ G, 50, 10 },
	{ Fis, 25, 10 },
	{ E, 25, 10 },
	{ Fis, 50, 10 },
	{ D, 50, 10 },

	{ E, 50, 10 },
	{ Fis, 50, 10 },
	{ G, 50, 10 },
	{ E, 50, 10 },

	{ A, 50, 10 },
	{ G, 50, 10 },
	{ Fis, 50, 10 },
	{ E, 50, 10 },

	{ Fis, 50, 10 },
	{ A, 50, 12 },
	{ D+12, 50, 15 },
	{ Cis+12, 50, 12 },

	{ H, 50, 15 },
	{ D+12, 50, 20 },
	{ G+12, 50, 25 },
	{ Fis+12, 50, 20 },

	{ E+12, 12, 10 },
	{ D+12, 13, 10 },
	{ E+12, 12, 10 },
	{ D+12, 13, 10 },
	{ E+12, 12, 10 },
	{ D+12, 13, 10 },
	{ E+12, 12, 10 },
	{ D+12, 13, 10 },
	{ E+12, 10, 10 },
	{ D+12, 10, 10 },
	{ E+12, 10, 10 },
	{ D+12, 10, 10 },
	{ Cis+12, 10, 10 },
	{ H, 25, 10 },
	{ Cis+12, 25, 10 },

	{ D+12, 25, 15 },
	{ A, 25, 15 },
	{ G, 25, 12 },
	{ A, 25, 12 },
	{ Fis, 100, 10 },
};

void addSound( 
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

	int m = waveWidth * f * 2 / 44100 ; //�s���Ė߂�̂Ŕ{

	//�ȉ��O�p�g
	int period = 44100 / f; //i�������Ŋ����߂邩
	//��̓f�[�^���߂邾��
	for ( int i = 0; i < sampleNumber; ++i ){
		int j = i % period;
		int w = ( j < period / 2 ) ? ( j*m ) : ( 2*waveWidth - j*m );
		w += data[ i ];
		data[ i ] = static_cast< short >( w ); 
	}
}

bool gFirstFrame = true;

namespace GameLib{
	void Framework::update(){
		sleep( 16 );
		if ( gFirstFrame ){ //�ŏ��̃t���[�������������Ȃ�
			gFirstFrame = false;
			short* wave = new short[ 44100 * 60 ]; //�Ƃ肠����1���B�z�񂪑傫������new�������������B

			//��U0�𖄂߂�
			for ( int i = 0; i < 44100 * 60; ++i ){
				wave[ i ] = 0;
			}

			//�y�������߂��܂��B
			//1�g���b�N��
			int time = 0;
			int noteNumber = sizeof( gTrack1 ) / sizeof( Note ); //����ŉ����̐����o�܂�
			for ( int i = 0; i < noteNumber; ++i ){
				const Note& n = gTrack1[ i ];
				if ( n.mVolume > 0 ){ //�����Ȃ珈����΂�
					addSound( 
						wave + time, 
						n.mVolume * 300,
						n.mName,
						n.mLength * 441 );
				}
				time += n.mLength * 441;
			}
			//2�g���b�N��
			time = 0;
			noteNumber = sizeof( gTrack2 ) / sizeof( Note ); //����ŉ����̐����o�܂�
			for ( int i = 0; i < noteNumber; ++i ){
				const Note& n = gTrack2[ i ];
				if ( n.mVolume > 0 ){ //�����Ȃ珈����΂�
					addSound( 
						wave + time, 
						n.mVolume * 300,
						n.mName,
						n.mLength * 441 );
				}
				time += n.mLength * 441;
			}
			playSound( wave, 44100 * 60 );
			SAFE_DELETE_ARRAY( wave );
		}
	}
}