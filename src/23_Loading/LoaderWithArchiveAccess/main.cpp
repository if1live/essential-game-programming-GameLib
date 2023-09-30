#include "GameLib/Framework.h"
#include "Loader.h"
#include <sstream>
using namespace std;
using namespace GameLib;

File* gFiles[ 100 ];

int gCount = 0;

namespace GameLib{
	void Framework::update(){
		if ( !Loader::instance() ){
			Loader::create( "data.bin" );
//			Loader::create(); //�������̍s�ɂ���ƁA�A�[�J�C�u�Ȃ��ŃA�N�Z�X�B
		}
		//���[�_���s
		Loader::instance()->update();

		//���[�h�ς݂Ȃ�폜
		for ( int i = 0; i < 100; ++i ){
			if ( gFiles[ i ] ){
				if ( gFiles[ i ]->isReady() ){
					Loader::instance()->destroyFile( &gFiles[ i ] );
				}
			}
		}
		ostringstream oss;
		//�󂢂Ă邾���S�X���b�g���[�h
		for ( int i = 0; i < 100; ++i ){
			if ( gFiles[ i ] == 0 ){
				oss.str( "" );
				oss << "data\\" << getRandom( 1000 ) << ".txt";
				Loader::instance()->createFile( &gFiles[ i ], oss.str().c_str() );
			}
		}
		oss.str( "" );
		int framerate = frameRate();
		oss << "Framerate: " << frameRate();
		int c = ( framerate > 255 ) ? 255 : framerate;
		drawDebugString( 0, 0, oss.str().c_str(), 0xffff0000 | ( c << 8 ) | c );
		int previousFrame = previousFrameInterval();
		oss.str( "" );
		oss << "PreviousFrame: " << previousFrameInterval();
		c = ( previousFrame > 63 ) ? 0 : ( 255 - previousFrame * 4 );
		drawDebugString( 0, 1, oss.str().c_str(), 0xffff0000 | ( c << 8 ) | c );

		//�~�܂��Ă�̂��킩��₷������`��
		double p[ 4 ][ 4 ];
		p[ 0 ][ 2 ] = p[ 1 ][ 2 ] = p[ 2 ][ 2 ] = p[ 3 ][ 2 ] = 0.0;
		p[ 0 ][ 3 ] = p[ 1 ][ 3 ] = p[ 2 ][ 3 ] = p[ 3 ][ 3 ] = 1.0;
		p[ 0 ][ 0 ] = p[ 2 ][ 0 ] = ( gCount % 640 ) / 320.0 - 1.05;
		p[ 1 ][ 0 ] = p[ 3 ][ 0 ] = ( gCount % 640 ) / 320.0 - 0.95;
		p[ 0 ][ 1 ] = p[ 1 ][ 1 ] = -0.05;
		p[ 2 ][ 1 ] = p[ 3 ][ 1 ] = 0.05;
		drawTriangle3DH( p[ 0 ], p[ 1 ], p[ 2 ] );
		drawTriangle3DH( p[ 3 ], p[ 1 ], p[ 2 ] );
		++gCount;

		if ( isEndRequested() ){
			Loader::destroy();
		}
	}
} //namespace GameLib