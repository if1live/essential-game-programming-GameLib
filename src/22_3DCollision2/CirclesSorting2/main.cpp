#include "GameLib/Framework.h"
#include "GameLib/Math.h"
#include "Vector2.h"
#include "List.h"
#include <sstream>
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
bool testCircles( int index0, int index1 ); //1�Â̔���֐�

//----------------------���������̏͂̃~�\----------------------------------------

//�\�[�g���邽�߂̍\����
struct Key{
	//�L�[���m�̑召�֌W���`
	bool operator<( const Key& a ){
		if ( mX < a.mX ){ //���������true
			return true;
		}else if ( mX > a.mX ){ //�傫�����false
			return false;
		}else if ( mIsLeft && !a.mIsLeft ){ //�������B�����������͍���D��
			return true;
		}else if ( !mIsLeft && a.mIsLeft ){
			return false;
		}else{ //�����܂œ����Ȃ�index�Ō��߂Ă��
			return ( mIndex < a.mIndex ); 
		}
	}

	double mX; //X���W
	int mIndex; //�~�̔ԍ�
	bool mIsLeft; //true�Ȃ獶�[�Afalse�Ȃ�E�[
};

void processCollision( int* test, int* hit ){
	*test = 0;
	*hit = 0;
#if 1
	int n = N*N; //����
	//�\�[�g���邽�߂̔z������B
	Key* keys = new Key[ n*2 ];
	//�L�[�̒��g�𖄂߂܂�
	for ( int i = 0; i < n; ++ i){
		const Circle& c = gCircles[ i ];
		keys[ 2 * i + 0 ].mX = c.mPosition.x - R; //���[
		keys[ 2 * i + 1 ].mX = c.mPosition.x + R; //�E�[
		keys[ 2 * i + 0 ].mIndex = keys[ 2 * i + 1 ].mIndex = i; //�����̔ԍ�
		keys[ 2 * i + 0 ].mIsLeft = true; //��
		keys[ 2 * i + 1 ].mIsLeft = false; //�E
	}
	//�\�[�g
	sort( keys, keys + n*2 );

	List metList( n );
	int* iteratorList = new int[ n ]; //metList�̂ǂ��ɓ��ꂽ���o���Ă����B

	//�����A������Ȃ߂Ă�����
	for ( int i = 0; i < n*2; ++i ){
		int i0 = keys[ i ].mIndex;
		if ( keys[ i ].mIsLeft ){ //���������B
			int iterator = metList.first(); //���X�g�S���Ɣ��肷���
			while ( !metList.isEnd( iterator ) ){
				int i1 = metList.value( iterator );
				++( *test );
				if ( testCircles( i0, i1 ) ){
					++( *hit );
				}
				iterator = metList.next( iterator );
			}
			//���X�g�ɒǉ�
			int pos = metList.addTail( i0 );
			//�ǉ������ꏊ���o���Ă�����
			iteratorList[ i0 ] = pos;
		}else{ //�E�������ꍇ�B
			//���X�g����폜���悤�B
			//�ꏊ���o���Ă������̂Ō����͂���Ȃ�
			metList.remove( iteratorList[ i0 ] );
		}
	}
	SAFE_DELETE_ARRAY( iteratorList );
	SAFE_DELETE_ARRAY( keys );
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
	if ( sql < RSUM2 ){
		double l = GameLib::sqrt( sql ) + 0.0000001; //0�Ŋ���Ȃ��悤�ɕی�
		t *= 0.25 / l; //�K���ɒ����𒲐�
		//�͂����Ԃ��Bt��p0->p1�̃x�N�^������A�����c1�ɑ����Ac0��������B
		c1.mVelocity += t;
		c0.mVelocity -= t;
		return true;
	}else{
		return false;
	}
}

namespace GameLib{
	void Framework::update(){
		if ( !gCircles ){
			gCircles = new Circle[ N*N ];
			//�����z�ux,y�ɂ���[-50,50]
			for ( int i = 0; i < N*N; ++i ){
				gCircles[ i ].mPosition.set( 
					static_cast< double >( ( ( i % N ) - N/2 ) * 4 ) + 0.001 * i, //������Ƃ��炷
					static_cast< double >( ( ( i / N ) - N/2 ) * 4 ) );
			}
		}
		//���x������
		for ( int i = 0;i < N*N; ++i ){
			//���x�����_�����ŏ�����
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
