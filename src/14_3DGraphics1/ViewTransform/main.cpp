#include <sstream>
#include "GameLib/Framework.h"
#include "Vector3.h"
#include "Matrix34.h"
#include "GameLib/Math.h"
using namespace GameLib;
using namespace std;

Vector3 gEyePosition( 0.0, 0.0, 1.0 );
Vector3 gEyeTarget( 0.0, 0.0, 0.0 );
int gCount;

namespace GameLib{
	void Framework::update(){
		setFrameRate( 60 );
		//���̒��_�z��
		Vector3 p[ 4 ];
		unsigned c[ 4 ]; //�킩��₷���悤�ɐF
		p[ 0 ].set( -1000.0, 0.0, -1000.0 );
		p[ 1 ].set( -1000.0, 0.0, 1000.0 );
		p[ 2 ].set( 1000.0, 0.0, -1000.0 );
		p[ 3 ].set( 1000.0, 0.0, 1000.0 );
		c[ 0 ] = 0xffff0000; //��
		c[ 1 ] = 0xff00ff00; //��
		c[ 2 ] = 0xff0000ff; //��
		c[ 3 ] = 0xffffffff; //��

		//�r���[�s�����낤
		Matrix34 m;
		m.setViewTransform( gEyePosition, gEyeTarget ); //���̒����{�̂Ȃ̂ł��������Q�Ƃ̂���

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
		drawTriangle3DH( p4[ 0 ], p4[ 1 ], p4[ 2 ], 0, 0, 0, c[ 0 ], c[ 1 ], c[ 2 ] );
		drawTriangle3DH( p4[ 3 ], p4[ 1 ], p4[ 2 ], 0, 0, 0, c[ 3 ], c[ 1 ], c[ 2 ] );

		++gCount;

		//���_�ƒ����_��K���ɂ�����
		gEyePosition.x = sin( gCount ) * 2000;
		gEyePosition.z = cos( gCount ) * 2000;
		gEyePosition.y = 1000.f;
		gEyeTarget.x = gCount % 100;
		gEyeTarget.y = gCount % 200;
		gEyeTarget.z = gCount % 300;
	}
}
