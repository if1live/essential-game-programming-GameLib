#include "GameLib/Framework.h"

void createSound( 
short* data, //�o��
int waveWidth, //�g�̕�=����
int note, //���ԍ�0��262Hz�̃h�B12��522Hz�̃h�B�ƌ��������̉��ԍ�
int sampleNumber ){ //�f�[�^��
	//���g���e�[�u���B2��12�捪(1.05946�{)�Â���Ă���
	int frequencyTable[] = {
		261, 277, 293, 311, 329, 349, 370, 392, 415, 440, 466, 494,
	};
	int octave = note / 12; //�I�N�^�[�u������B���̐��������g��2�{���Ă��B
	int f = frequencyTable[ note % 12 ]; //�\�������ăh���~�����߂�
	f <<= octave; //octave��2�Ȃ�4�{���������A����͍��V�t�g2���B

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

namespace GameLib{
	void Framework::update(){
		sleep( 16 );
		static int a = 0;
		static int b = 0;
		if ( a++ % 120 == 0 ){
			short wave[ 44100 ];
			//�h�~�\�~�h�[�A�݂����Ȋ�����5�B���y�̎��Ƃ��v���o���܂��񂩁H
			createSound( wave + 44100*0/5, 20000, b+0, 44100/5 );
			createSound( wave + 44100*1/5, 20000, b+4, 44100/5 );
			createSound( wave + 44100*2/5, 20000, b+7, 44100/5 );
			createSound( wave + 44100*3/5, 20000, b+4, 44100/5 );
			createSound( wave + 44100*4/5, 20000, b+0, 44100/5 );

			playSound( wave, 44100 );
			++b;
		}
	}
}