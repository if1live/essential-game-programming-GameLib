#include "GameLib/Framework.h"
#include "GameLib/Math.h"
#include "Vector2.h"
#include <sstream>
#include <list>
#include <algorithm>
using namespace std;
using namespace GameLib;

struct Circle{
	Vector2 mPosition;
	Vector2 mVelocity;
};
Circle* gCircles = 0;
const int N = 40; //����̓��ł��
const double R = 2.0; //���a2��
const double RSUM2 = ( R + R ) * ( R + R ); //���a�a�̓��
const Vector2 gMinimum( -160.0, -160.0 );
const Vector2 gMaximum( 160.0, 160.0 );
int gDivision = 10;
bool testCircles( int index0, int index1 ); //1�Â̔���֐�(��:���ŗ͂͗^���Ȃ�)
void addForce( int i0, int i1 ); //�͂𑫂��̂͂�����

//----------------------���������̏͂̃~�\----------------------------------------

//���������y�A
struct HitPair{ 
	bool operator<( const HitPair& a ) const { //�\�[�g�ɕK�v�ȕs����
		if ( mI0 < a.mI0 ){
			return true;
		}else if ( mI0 > a.mI0 ){
			return false;
		}else{
			return ( mI1 < a.mI1 );
		}
	}
	bool operator==( const HitPair& a ) const {//unique�ɕK�v�ȓ���
		return ( ( mI0 == a.mI0 ) && ( mI1 == a.mI1 ) );
	}
	int mI0;
	int mI1;
};

void processCollision( int* test, int* hit ){
	*test = 0;
	*hit = 0;
#if 1
	int n = N*N; //����
	//���̃T�C�Y���v�Z
	Vector2 boxSize; //���T�C�Y
	boxSize.setSub( gMaximum, gMinimum );
	boxSize *= 1.0 / static_cast< float >( gDivision );
	//���X�g�p��
	list< int >* boxList = new list< int >[ gDivision * gDivision ];
	for ( int i = 0; i < n; ++i ){
		Vector2 t;
		t.setSub( gCircles[ i ].mPosition, gMinimum );
		double minX = t.x - R;
		double maxX = t.x + R;
		double minY = t.y - R;
		double maxY = t.y + R;
		//���ԍ��ɕϊ�(���Z�͍������ł���ˁH)
		int minXBox = static_cast< int >( minX / boxSize.x );
		int maxXBox = static_cast< int >( maxX / boxSize.x );
		int minYBox = static_cast< int >( minY / boxSize.y );
		int maxYBox = static_cast< int >( maxY / boxSize.y );
		ASSERT( minXBox >= 0 && maxXBox < gDivision && minYBox >=0 && maxYBox < gDivision );
		for ( int j = minXBox; j <= maxXBox; ++j ){
			for ( int k = minYBox; k <= maxYBox; ++k ){
				int boxIndex = k * gDivision + j; //���ԍ��́uY * (������) + X�v
				boxList[ boxIndex ].push_back( i );
			}
		}
	}
	//�����Ƃɑ�����
	typedef list< int >::iterator It; //�����̂ŕʖ�
	list< HitPair > hitList;
	for ( int i = 0; i < gDivision * gDivision; ++i ){
		for ( It j = boxList[ i ].begin(); j != boxList[ i ].end(); ++j ){
			It k = j; //j�̎�����
			++k;
			for ( ; k != boxList[ i ].end(); ++k ){
++( *test );
				if ( testCircles( *j, *k ) ){
++( *hit );
					HitPair hit;
					//�����y�A�͓����łȂ��ƍ���̂ŁA����������O�ɂ���
					if ( *j < *k ){
						hit.mI0 = *j;
						hit.mI1 = *k;
					}else{
						hit.mI0 = *k;
						hit.mI1 = *j;
					}
					hitList.push_back( hit );
				}
			}
		}
	}
	//������y�A����������̂ł�����z��ɃR�s�[
	HitPair* hitArray = new HitPair[ hitList.size() ];
	int j = 0;
	for ( list< HitPair >::iterator i = hitList.begin(); i != hitList.end(); ++i ){
		hitArray[ j ] = *i;
		++j;
	}
	//�\�[�g
	sort( hitArray, hitArray + j );
	//�d��������
	size_t hitN = unique( hitArray, hitArray + j ) - hitArray;
	//�͂�������
	for ( size_t i = 0; i < hitN; ++i ){
		addForce( hitArray[ i ].mI0, hitArray[ i ].mI1 );
	}
	SAFE_DELETE_ARRAY( hitArray );
	SAFE_DELETE_ARRAY( boxList );
#else //�f�o�O�p�������
	for ( int i = 0; i < N*N; ++i ){
		for ( int j = i + 1; j < N*N; ++j ){
			++( *test );
			if ( testCircles( i, j ) ){
				++( *hit );
			}
		}
	}
#endif
}

//----------------------�������牺�͂��̖̏͂{�؂ł͂Ȃ��R�[�h---------------------------

//2��circle���������钆�g�B�������true
bool testCircles( int i0, int i1 ){
	Circle& c0 = gCircles[ i0 ];
	const Vector2& p0 = c0.mPosition;
	Circle& c1 = gCircles[ i1 ];
	const Vector2& p1 = c1.mPosition;
	//�����́H
	Vector2 t;
	t.setSub( p1, p0 );
	double sql = t.squareLength();
	if ( sql < RSUM2 ){ //���a��4�ł�������
		return true;
	}else{
		return false;
	}
}

void addForce( int i0, int i1 ){
	Vector2 t;
	t.setSub( gCircles[ i0 ].mPosition, gCircles[ i1 ].mPosition );
	double l = t.length();
	t *= 0.25 / l; //�K���ɒ����𒲐�
	//�͂����Ԃ��Bt��p0->p1�̃x�N�^������A�����c1�ɑ����Ac0��������B
	gCircles[ i0 ].mVelocity += t;
	gCircles[ i1 ].mVelocity -= t;
}

namespace GameLib{
	void Framework::update(){
		if ( !gCircles ){
			gCircles = new Circle[ N*N ];
			//�����z�u
			for ( int i = 0; i < N*N; ++i ){
				gCircles[ i ].mPosition.set( 
					static_cast< double >( ( ( i % N ) - N/2 ) * 4 ) + 0.001 * i, //������Ƃ��炷
					static_cast< double >( ( ( i / N ) - N/2 ) * 4 ) );
			}
		}
		//���x������
		for ( int i = 0;i < N*N; ++i ){
			//���x�����_�ւ̈��͂ŏ�����
			gCircles[ i ].mVelocity.setMul( gCircles[ i ].mPosition, -0.001 );
		}
		int test;
		int hit;
		processCollision( &test, &hit ); //�Փˌ��o�֐�

		//�X�V
		for ( int i = 0;i < N*N; ++i ){
			gCircles[ i ].mPosition += gCircles[ i ].mVelocity;

			//�`��
			double p[ 4 ][ 2 ];
			p[ 0 ][ 0 ] = p[ 1 ][ 0 ] = gCircles[ i ].mPosition.x - 0.5 + 160.0;
			p[ 2 ][ 0 ] = p[ 3 ][ 0 ] = gCircles[ i ].mPosition.x + 0.5 + 160.0;
			p[ 0 ][ 1 ] = p[ 2 ][ 1 ] = gCircles[ i ].mPosition.y - 0.5 + 120.0;
			p[ 1 ][ 1 ] = p[ 3 ][ 1 ] = gCircles[ i ].mPosition.y + 0.5 + 120.0;
			drawTriangle2D( p[ 0 ], p[ 1 ], p[ 2 ] );
			drawTriangle2D( p[ 3 ], p[ 1 ], p[ 2 ] );
		}
		ostringstream oss;
		oss << frameRate() << " " << test << " " << hit;
		drawDebugString( 0, 0, oss.str().c_str() );
	}
}
