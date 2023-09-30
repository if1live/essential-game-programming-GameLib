#include "GameLib/GameLib.h"
#include "Track.h"
#include "Note.h"
#include <cmath>
using namespace std;

Track::Track() : mNotes( 0 ), mNoteNumber( 0 ){}

Track::~Track(){
	if ( mNotes ){
		SAFE_DELETE_ARRAY( mNotes );
	}
	mNoteNumber = 0;
}

void Track::setData( const Note* notes, int noteNumber ){
	mNotes = new Note[ noteNumber ];
	for ( int i = 0; i < noteNumber; ++i ){
		mNotes[ i ] = notes[ i ];
	}
	mNoteNumber = noteNumber;
}

void Track::addWave( short* buffer, int bufferSize, int frequency ) const {
	int time = 0; //1/100�P�ʎ���
	int pos = 0; //�T���v�����P�ʎ���
	for ( int i = 0; i < mNoteNumber; ++i ){
		const Note& n = mNotes[ i ];
		int endTime = time + n.mLength;
		int length = ( endTime * frequency / 100 ) - pos;
		if ( pos + length >= bufferSize ){
			length = bufferSize - pos; //���ӂ�Ă܂�
		}
		if ( n.mVolume > 0 ){ //�����Ȃ珈����΂�
			addNote( 
				buffer + pos,
				n.mVolume * 32767 / 100,
				n.mName,
				length,
				frequency );
		}
		time = endTime;
		pos += length;
	}
}

void Track::addNote( 
short* buffer,
int waveWidth, 
int name, 
int sampleNumber,
int frequency ) const {
	//57��440Hz�ɂȂ�悤��name�����߂���B
	double f = 440.0 * pow( 2.0, ( name - 57 ) * ( 1.0 / 12.0 ) );

	//�����Ǝg��PI
	const double pi = 3.1415926535897932384626433832795;
	//�ȉ������g����
	/*
	y = c*sin( ax )
	�������g�̎��Bsine��2PI��1����������A�܂�a=2PI�ɂ����x��1��1�������B
	���ۂ̎�����f/44100�����炱����悶��
	a = 2PI * f / 44100;
	b��waveWidth�̔������B-b����b�܂œ����B
	���̃T���v���ł͂��񂾂񉹂��������Ȃ�悤�ɂ��Ă݂��B
	*/
	double a = f * ( 2.0 * pi ) / frequency;
	double b = waveWidth / 2.0;
	double rcpSampleNumber = 1.0 / sampleNumber;

	//�y�������F����(�g�����y�b�g�̃t�[���G��͌���(7�{���܂�))
	//������������Ƃ����ȉ��F���ł��܂��B
	//�e�{���̊ܗL��
	const double factor[] = { 
		0.5, //�����g��
		0.85, //2�{��
		1.05, //3�{��
		0.85,
		0.4,
		0.3,
		0.1,
		0.0
	};
	//�e�{���̈ʑ�(�P�ʂ͓x)
	const double phase[] = { 
		339.0, //�����g��
		9.0, //2�{��
		18.0, //3�{��
		3.0,
		351.0,
		353.0,
		358.0,
		0,
	};
	//�r�u���[�g�psin(ax)��a(�b��8��)
	const double va = 8.f * ( 2.0 * pi ) / frequency;

	for ( int i = 0; i < sampleNumber; ++i ){
		double posInNote = i * rcpSampleNumber; //�����̒��̈ʒu
		double tb = b * ( 1.0 - posInNote ); //���ʂ����񂾂񏬂���
		tb *= ( 1.0 + posInNote * 0.2 * sin( va * i ) ); //���B�u���[�g(�����̌��قǋ���)
		//8�̐����g������
		double w = 0.0;
		for ( int j = 0; j < 8; ++j ){
			w += tb * sin( a * j * i + ( phase[ j ] / pi ) ) * factor[ j ];
		}
		w += buffer[ i ];
 		buffer[ i ] = static_cast< short >( w );
	}
}
