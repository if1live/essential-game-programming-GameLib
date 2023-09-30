#include <sstream>
#include "GameLib/Framework.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
#include "Vector3.h"
#include "Matrix34.h"
#include "Matrix44.h"
#include "GameLib/Math.h"
using namespace GameLib;
using namespace std;

bool gMultiplyWToZByHand = false;
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

		//���[���h�ϊ�(Z��])
		Matrix34 wm;
		wm.setRotationZ( gCount * 4.0 );
		//�r���[�s�����낤
		Vector3 eyePosition;
		eyePosition.x = sin( gCount ) * 4000;
		eyePosition.z = cos( gCount ) * 4000;
		eyePosition.y = 1000.f;
		Vector3 eyeTarget( 0.0, 0.0, 0.0 );
		Matrix34 vm;
		vm.setViewTransform( eyePosition, eyeTarget );
		//�����ϊ��s��
		Matrix44 pm;
		pm.setPerspectiveTransform( 60.0, 640.0, 480.0, 1.0, 10000.0, gMultiplyWToZByHand );
		//�܂�VW�����
		vm *= wm;
		//����PVW�����
		pm *= vm;
		//�s��Ƀx�N�^�������ĉ��
		double p4[ 4 ][ 4 ];
		for ( int i = 0; i < 4; ++i ){
			pm.multiply( p4[ i ], p[ i ] );
		}
		//�蓮��z *= w������Ȃ���
		if ( gMultiplyWToZByHand ){
			for ( int i = 0; i < 4; ++i ){
				p4[ i ][ 2 ] *= p4[ i ][ 3 ];
			}
		}
		//�l�p�`��`���B
		drawTriangle3DH( p4[ 0 ], p4[ 1 ], p4[ 2 ], 0, 0, 0, c[ 0 ], c[ 1 ], c[ 2 ] );
		drawTriangle3DH( p4[ 3 ], p4[ 1 ], p4[ 2 ], 0, 0, 0, c[ 3 ], c[ 1 ], c[ 2 ] );

		//Z�o�b�t�@���[�h�ؑ�
		if ( Input::Manager::instance().keyboard().isTriggered( 'z' ) ){
			gMultiplyWToZByHand = !gMultiplyWToZByHand;
		}

		//�\��
		if ( gMultiplyWToZByHand ){
			drawDebugString( 0, 0, "Linear Z Buffer ( high precision )" );
		}else{
			drawDebugString( 0, 0, "Reciprocal Z Buffer ( low precision )" );
		}
		++gCount;
	}
}
