#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "Vector3.h"
#include "Matrix34.h"

#include <sstream>
using namespace std;

bool gFirstFrame = true;
Vector3 gTranslation( 0.0, 0.0, 0.0 );
Vector3 gScale( 1.0, 1.0, 1.0 );
int gCount;

namespace GameLib{
	void Framework::update(){
		setFrameRate( 60 );

		//���̒��_�z��
		Vector3 p[ 4 ];
		p[ 0 ].set( -1000.0, 0.0, -1000.0 );
		p[ 1 ].set( -1000.0, 0.0, 1000.0 );
		p[ 2 ].set( 1000.0, 0.0, -1000.0 );
		p[ 3 ].set( 1000.0, 0.0, 1000.0 );
		//�s�����낤
		//�Ƃ肠����3���œK���ɉ񂵂Ă��炿����Ɖ��Ɉړ�������B���₷���悤�ɁB
		//�s��쐬�͋t���Ȃ̂ŁA�ŏ��͈ړ�����B
		Matrix34 m;
		m.setTranslation( Vector3( 0.0, -100.0, -3000.0 ) );
		m.rotateZ( gCount );
		m.rotateY( gCount );
		m.rotateX( gCount );
		//�s��Ƀx�N�^�������ĉ��
		for ( int i = 0; i < 4; ++i ){
			m.multiply( &p[ i ], p[ i ] );
		}

		//�j�A�N���b�v�ƃt�@�[�N���b�v
		const double nearClip = 1.0;
		const double farClip = 10000.0;
		//�j�A�ƃt�@�[����Z�͈͕ϊ��̎������
		const double zConvA = 1.0 / ( nearClip - farClip ); //1/(n-f)
		const double zConvB = nearClip * zConvA; //n/(n-f)

		//�n�[�h�E�F�A�ɓn������������B4������
		double p4[ 4 ][ 4 ];
		for ( int i = 0; i < 4; ++i ){
			p4[ i ][ 0 ] = p[ i ].x;
			//y��640/480�������ďc����𒲐�
			p4[ i ][ 1 ] = p[ i ].y * 640.0 / 480.0;
			//w�ɔ͈͕ϊ��O��z���u�}�C�i�X�ɂ��āv�i�[�BZ������O�������Ƃ��̃}�C�i�X���K�v�B
			p4[ i ][ 3 ] = -p[ i ].z;
			//z�͈͕ϊ�
			p4[ i ][ 2 ] = zConvA * p[ i ].z + zConvB; //�͈͕ϊ���Z���̌�����
			//z��w�������Ă����B
			p4[ i ][ 2 ] *= p4[ i ][ 3 ];
		}
		//�l�p�`��`���B
		drawTriangle3DH( p4[ 0 ], p4[ 1 ], p4[ 2 ] );
		drawTriangle3DH( p4[ 3 ], p4[ 1 ], p4[ 2 ] );

		++gCount;
	}
}
