#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"

#include <sstream>
using namespace std;

double gZMove = 2000.0;
bool gFirstFrame = true;
GameLib::Texture* gTexture;

namespace GameLib{
	void Framework::update(){
		setFrameRate( 60 );
		if ( gFirstFrame ){
			gFirstFrame = false;
			createTexture( &gTexture, "grid.tga" ); 
		}

		double p[ 4 ][ 4 ] = { //4���� 
			{ -500.0, -300.0, -1000.0, 1.0 }, //Y�͏オ�v���X�Ȃ̂ŉ��ɂ��炷�ɂ̓}�C�i�X
			{ -500.0, -300.0, 1000.0, 1.0 },
			{ 500.0, -300.0, -1000.0, 1.0 },
			{ 500.0, -300.0, 1000.0, 1.0 },
		};
		double uv[ 4 ][ 2 ] = { 
			{ 0.0, 0.0 },
			{ 1.0, 0.0 },
			{ 0.0, 1.0 },
			{ 1.0, 1.0 },
		};

		//�n�[�h�E�F�A�ɓn������
		for ( int i = 0; i < 4; ++i ){
			//y��640/480�������ďc����𒲐�
			p[ i ][ 1 ] *= 640.0 / 480.0;
			//�ړ��ʔ��f
			p[ i ][ 2 ] += gZMove;
			//w�ɔ͈͕ϊ��O��z���i�[
			p[ i ][ 3 ] = p[ i ][ 2 ];
			//z�͈͕ϊ�
			p[ i ][ 2 ] = ( 1.0/9999.0 ) * p[ i ][ 2 ] - ( 1.0/9999.0 );
			//z�ɔ͈͕ϊ��O��z��]���ɏ�Z
			p[ i ][ 2 ] *= p[ i ][ 3 ];
		}
		//�l�p�`��`���B
		setTexture( gTexture );
		drawTriangle3DH( p[ 0 ], p[ 1 ], p[ 2 ], uv[ 0 ], uv[ 1 ], uv[ 2 ], 0xffff0000, 0xffff0000, 0xffff0000 );
		drawTriangle3DH( p[ 3 ], p[ 1 ], p[ 2 ], uv[ 3 ], uv[ 1 ], uv[ 2 ], 0xff00ff00, 0xff00ff00, 0xff00ff00 );

		if ( Input::Manager::instance().keyboard().isOn( 'z' ) ){
			gZMove -= 10.f;
		}else if ( Input::Manager::instance().keyboard().isOn( 'w' ) ){
			gZMove += 10.f; 
		}
		//�\��
		ostringstream oss;
		oss << "[w-z] Z MOVEMENT: " << gZMove;
		drawDebugString( 10, 0, oss.str().c_str() );
		oss.str( "" );

		if ( isEndRequested() ){
			destroyTexture( &gTexture );
		}
	}
}
