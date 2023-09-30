#include "GameLib/Framework.h"
#include "GameLib/DebugScreen.h"
#include "GameLib/Math/Random.h"
#include <algorithm>
#include <cmath>
using namespace std;
using namespace GameLib;
using namespace GameLib::Math;

//�����_�����̌���
//32*32*32�̔��Ƀ����_���ɓ���Ă����A�����ƃ����_�������m�F����B
Random gRandom;

const int N = 1000*1000;
const int M = 32;
const int M3 = M * M * M;
double gK = 0.0;
double gC = 0.0;

namespace GameLib{
	void Framework::configure( Configuration* c ){
		c->enableVSync( false );
	}
	void Framework::update(){
		if ( !gRandom ){
			gRandom = Random::create();
		}
		int* b = new int[ M3 ];
		for ( int i = 0; i < M3; ++i ){
			b[ i ] = 0;
		}
		for ( int i = 0; i < N; ++i ){
			int v[ 3 ];
			for ( int j = 0; j < 3; ++j ){
#if 0 //1�ɂ���ƕW��rand(),0�ɂ����Math::Random
				v[ j ] = rand();
#else
				v[ j ] = gRandom.getInt();
#endif
				v[ j ] &= 0x1f;
			}
			++b[ ( ( ( v[ 0 ] * M ) + v[ 1 ] ) * M ) + v[ 2 ] ];
		}
		double s = 0.0;
		double a = static_cast< double >( N ) / M3;
		for ( int i = 0; i < M3; ++i ){
			s += ( b[ i ] - a ) * ( b[ i ] - a );
		}
		s /= M3;
		s = std::sqrt( s );
		double sIdeal = std::sqrt( a );
		SAFE_DELETE_ARRAY( b );

		gK += s - sIdeal;
		gC += 1.0;

		//���Ɍv�Z�����W���΍��A���z�I�W���΍��A���z�Ƃ̍��A���z�Ƃ̍�(�ݐ�)
		DebugScreen sc;
		sc << s << " " << sIdeal << " " << s - sIdeal << " " << gK/gC;
	}
}
