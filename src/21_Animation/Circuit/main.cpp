#include "GameLib/Framework.h"

#include "Library/GraphicsDatabase.h"
#include "Library/Model.h"
#include "Library/Vector3.h"
#include "Library/Matrix44.h"
#include "Library/Matrix34.h"
#include "GameLib/Math.h"
using namespace GameLib;

GraphicsDatabase* gDatabase;
Model* gModel;
double gT; //����

const int N = 9; //�_�̐�

//��ԓ_
Vector3 gPoints[ N ] = {
	Vector3( -10.0, 0.0, 3.0 ),
	Vector3( -10.0, 0.0, -3.0 ),
	Vector3( 10.0, 0.0, -3.0 ),
	Vector3( 10.0, 0.0, 3.0 ),
	Vector3( 5.0, -2.0, 3.0 ),
	Vector3( 5.0, -4.0, 0.0 ),
	Vector3( -5.0, 4.0, 0.0 ),
	Vector3( -5.0, 2.0, 3.0 ),
	Vector3( -10.0, 0.0, 3.0 ), //�n�_
};

//�ʉߎ���
double gTimes[ N ] = {
	0.0,
	100.0,
	300.0,
	400.0,
	500.0,
	550.0,
	600.0,
	650.0,
	700.0,
};

//2���A1���A0���̏�
Vector3 gEquations[ N - 1 ][ 3 ];

namespace GameLib{
	void Framework::update(){
		if ( !gDatabase ){
			setFrameRate( 60 );
			gDatabase = new GraphicsDatabase( "cube.txt" );
			gModel = gDatabase->createModel( "cube" );
			/*��Ԃ̕������������B

			p0 = a*t0^2 + b*t0 + c ...(0)
			p1 = a*t1^2 + b*t1 + c ...(1)
			2a*t0 + b = v           ...(2)

			�܂���Ԏ��̌`���P����(2)���g����b=�̌`�ɂ���B

			b = v - 2a*t0 ...(3)

			����(1)-(0)��c�������B

			p1 - p0 = a(t1^2- t0^2) + b(t1 - t0) ...(4)

			(4)��(3)�������Ă��B

			p1 - p0 = a(t1^2- t0^2) + (v-2a*t0)(t1 - t0) 

			����΂��ēW�J����B

			p1 - p0 = a( t1^2 - t0^2 - 2*t0*t1 + 2*t0^2 ) + v(t1-t0)

			a�Ő�������

			a( t0^2 - 2*t0*t1 + t1^2 ) = (p1-p0) - v(t1-t0)
			
			������a�̌W����(t0-t1)^2�Ɉ��������ł���B

			a�ɂ��ĉ����āA

			a = ((p1-p0) - v(t1-t0)) / ( t0-t1 )^2 ...(5)

			���(3)���g����b���o�邵�A(0)���g����c���o��B
			*/
			Vector3 v( 0.0, 0.0, 0.0 ); //����Ԃ̎n�_�̌X����0�Ƃ��悤���B3�v�Z����̂Ńx�N�^�B
			//�ȉ�XYZ�ŕʁX�̌v�Z�����邪�A���₷���悤�Ƀx�N�^�N���X�̌v�Z�֐��͎g��Ȃ��B�v�f���v�Z�ōs���B
			for ( int i = 0; i < N-1; ++i ){ //�_�̐���N�Ȃ񂾂����Ԃ̐���N-1
				//���₷���悤�ɒ萔�����B
				Vector3 p0 = gPoints[ i ];
				Vector3 p1 = gPoints[ i + 1 ];
				double t0 = gTimes[ i ];
				double t02 = t0 * t0;
				double t1 = gTimes[ i + 1 ];
				double t10 = t1 - t0;
				double t102 = t10 * t10;

				Vector3 a,b,c; //xyz�Ōv�Z���邩��S���x�N�^
				//�܂�a�����߂�B
				a.x = ( ( p1.x - p0.x ) - v.x*t10 ) / t102;
				a.y = ( ( p1.y - p0.y ) - v.y*t10 ) / t102;
				a.z = ( ( p1.z - p0.z ) - v.z*t10 ) / t102;
				//����b
				b.x = v.x - 2.0 * a.x * t0;
				b.y = v.y - 2.0 * a.y * t0;
				b.z = v.z - 2.0 * a.z * t0;
				//�Ō��c
				c.x = p0.x - a.x*t02 - b.x*t0;
				c.y = p0.y - a.y*t02 - b.y*t0;
				c.z = p0.z - a.z*t02 - b.z*t0;

				//����ۑ�
				gEquations[ i ][ 0 ] = a;
				gEquations[ i ][ 1 ] = b;
				gEquations[ i ][ 2 ] = c;

				//�I�_�̌X�����v�Z
				v.x = 2.0 * a.x * t1 + b.x;
				v.y = 2.0 * a.y * t1 + b.y;
				v.z = 2.0 * a.z * t1 + b.z;
			}
		}

		//���[�v��������B
		double period = gTimes[ N - 1 ];
		double quot = gT / period;
		int quotInt = static_cast< int >( quot ); 
		gT -= static_cast< double >( quotInt ) * period;

		//��Ԃ�����
		int last = 0;
		for ( int i = 0; i < N; ++i ){
			if ( gTimes[ i ] > gT ){
				break;
			}
			last = i;
		}

		//���ɂԂ�����Œl���o��
		Vector3 p;
		Vector3 a = gEquations[ last ][ 0 ];
		Vector3 b = gEquations[ last ][ 1 ];
		Vector3 c = gEquations[ last ][ 2 ];
		Vector3 p0 = gPoints[ last ];
		Vector3 p1 = gPoints[ last + 1 ];
		p.x = ( ( a.x * gT ) + b.x ) * gT + c.x;
		p.y = ( ( a.y * gT ) + b.y ) * gT + c.y;
		p.z = ( ( a.z * gT ) + b.z ) * gT + c.z;
		//�l�����܂����B���f���ɃZ�b�g
		gModel->setPosition( p );

		Matrix44 pm;
		pm.setPerspectiveTransform( 45.0, width(), height(), 1.0, 10000.0 );
		Matrix34 vm;
		vm.setViewTransform( Vector3( 0.0, 30.0, 30.0 ), Vector3( 0.0, 0.0, 0.0 ) );
		pm *= vm;
		gModel->draw( pm, Vector3( 0.0, 1.0, 0.0 ), Vector3( 1.0, 1.0, 1.0 ), Vector3( 0.2, 0.2, 0.2 ) );

		gT += 1.0;
		if ( isEndRequested() ){
			SAFE_DELETE( gModel );
			SAFE_DELETE( gDatabase );
		}
	}
}
