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
const int gDivision = 30;
const int gBoxListBlockSize = 10000; //�Ă��Ƃ�
const int gHitListBlockSize = 10000; //�Ă��Ƃ�
bool testCircles( int index0, int index1 ); //1�Â̔���֐�(��:���ŗ͂͗^���Ȃ�)
void addForce( int i0, int i1 ); //�͂𑫂��̂͂�����

//----------------------���������̏͂̃~�\--------------Z--------------------------

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

//���ԍ��Ɖ~�ԍ��̃y�A
struct ObjPair{
	void set( int box, int circle ){
		mBox = box;
		mCircle = circle;
	}
	int mBox;
	int mCircle;
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
	list< ObjPair* > boxList;
	int boxListBlockPos = gBoxListBlockSize;

	//���i�K�B�t���b�g��ArrayList�ɔ��ԍ��ƕ��̔ԍ����i�[
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
				if ( boxListBlockPos == gBoxListBlockSize ){
					boxList.push_back( new ObjPair[ gBoxListBlockSize ] );
					boxListBlockPos = 0;
				}
				boxList.back()[ boxListBlockPos ].set( boxIndex, i ) ;
				++boxListBlockPos;
			}
		}
	}
	//���i�K�B���𐔂���
	int* boxListSize = new int[ gDivision * gDivision ];
	//������
	for ( int i = 0; i < gDivision * gDivision; ++i ){
		boxListSize[ i ] = 0;
	}
	//���[�v�Ő�����
	typedef list< ObjPair* >::iterator It; //�ʖ�
	int blockPos = 0;
	int blockNumber = static_cast< int >( boxList.size() );
	for ( It i = boxList.begin(); i != boxList.end(); ++i ){
		//���u���b�N�̃T�C�Y�����߂�B�Ōゾ�������Ⴄ�B����Ă���r�����������炾�B
		int blockSize = ( blockPos == ( blockNumber - 1 ) ) ? boxListBlockPos : gBoxListBlockSize;
		for ( int j = 0; j < blockSize; ++j ){
			const ObjPair& o = ( *i )[ j ];
			++boxListSize[ o.mBox ];
		}
		++blockPos;
	}
	//boxListSize�͐������A�����擪����I�t�Z�b�g�ɕϊ�����B
	int* boxListOffset = new int[ gDivision * gDivision ];
	int offset = 0;
	for ( int i = 0; i < gDivision * gDivision; ++i ){
		boxListOffset[ i ] = offset;
		offset += boxListSize[ i ];
		boxListSize[ i ] = 0; //�T�C�Y�z���0�ɂ��Ă��܂��B
		//���ɓ���Ă������ɂ����ɐ����Ȃ����΂܂��������̂���������B
		//��O�i�K�łǂ��܂œ��ꂽ�����o���Ă������߂̔z���ʂɍ��Ȃ����߂̌Ƒ��ȍH�v�B
	}
	//��O�i�K�B���ۂɔ����Ƃ̃��X�g�𐶐��B

	//���̔z����m�ہB���傤�Ǐ��offset�ɍ��v�������Ă���B
	int* boxArray = new int[ offset ];
	//����Ă����B
	blockPos = 0;
	blockNumber = static_cast< int >( boxList.size() );
	for ( It i = boxList.begin(); i != boxList.end(); ++i ){
		//���u���b�N�̃T�C�Y�����߂�B�Ōゾ�������Ⴄ�B����Ă���r�����������炾�B
		int blockSize = ( blockPos == ( blockNumber - 1 ) ) ? boxListBlockPos : gBoxListBlockSize;
		for ( int j = 0; j < blockSize; ++j ){
			const ObjPair& o = ( *i )[ j ];
			boxArray[ boxListOffset[ o.mBox ] + boxListSize[ o.mBox ] ] = o.mCircle;
			++boxListSize[ o.mBox ]; //�T�C�Y��+1�B���̃��[�v�𔲂������ɂ͍ŏ��ɐ��������Ɠ�����ԂɂȂ��Ă���B
		}
		SAFE_DELETE_ARRAY( *i );
		++blockPos;
	}
	//�����Ƃɑ�����B�z��ɂ��Ă��܂����֌W����ɒP�������Ă���B
	list< HitPair* > hitList;
	int hitListBlockPos = gHitListBlockSize;
	for ( int i = 0; i < gDivision * gDivision; ++i ){ //���Ń��[�v
		//���̒��̐�������
		int boxSize = boxListSize[ i ];
		const int* box = &boxArray[ boxListOffset[ i ] ]; //�ʖ�
		for ( int j = 0; j < boxSize; ++j ){ //��ڂ̕��̃��[�v
			for ( int k = j + 1; k < boxSize; ++k ){ //��ڂ̕��̃��[�v
				int i0 = box[ j ];
				int i1 = box[ k ];
				++( *test );
				if ( testCircles( i0, i1 ) ){
					++( *hit );
					HitPair hit;
					//�����y�A�͓����łȂ��ƍ���̂ŁA����������O�ɂ���
					if ( i0 < i1 ){
						hit.mI0 = i0;
						hit.mI1 = i1;
					}else{
						hit.mI0 = i0;
						hit.mI1 = i1;
					}
					if ( hitListBlockPos == gHitListBlockSize ){
						hitList.push_back( new HitPair[ gHitListBlockSize ] );
						hitListBlockPos = 0;
					}
					hitList.back()[ hitListBlockPos ] = hit;
					++hitListBlockPos;
				}
			}
		}
	}
	//�����������X�g�̏d������������addForce()���ĂԁB

	//�R�[�h���V���v���ɂ��Ă����������邽�߁A
	//�z��̃��X�g�𕁒ʂ̔z��ɒ���
	HitPair* hitListArray = new HitPair[ hitList.size() * gHitListBlockSize ];
	int hitListArraySize = 0;
	typedef list< HitPair* >::iterator HIt; //�ʖ�
	blockPos = 0;
	blockNumber = static_cast< int >( hitList.size() );
	for ( HIt i = hitList.begin(); i != hitList.end(); ++i ){
		//���u���b�N�̃T�C�Y�����߂�B�Ōゾ�������Ⴄ�B����Ă���r�����������炾�B
		int blockSize = ( blockPos == ( blockNumber - 1 ) ) ? hitListBlockPos : gHitListBlockSize;
		for ( int j = 0; j < blockSize; ++j ){
			hitListArray[ hitListArraySize ] = ( *i )[ j ];
			++hitListArraySize;
		}
		SAFE_DELETE_ARRAY( *i ); //��������Ȃ��̂ŏ���
		++blockPos;
	}

	//�܂��y�A�̈�Ԗڂ�i�ł�����̂��������邩�𐔂���
	int* hitListSize = new int[ n ];
	//������
	for ( int i = 0; i < n; ++i ){
		hitListSize[ i ] = 0;
	}
	//���[�v�Ő�����
	for ( int i = 0; i < hitListArraySize; ++i ){
		++hitListSize[ hitListArray[ i ].mI0 ];
	}
	//hitListSize�͐������A�����擪����I�t�Z�b�g�ɕϊ�����B
	int* hitListOffset = new int[ n ];
	offset = 0;
	for ( int i = 0; i < n; ++i ){
		hitListOffset[ i ] = offset;
		offset += hitListSize[ i ];
		hitListSize[ i ] = 0; //�T�C�Y�z���0�ɂ��Ă��܂��B
		//���ɓ���Ă������ɂ����ɐ����Ȃ����΂܂��������̂���������B
		//��O�i�K�łǂ��܂œ��ꂽ�����o���Ă������߂̔z���ʂɍ��Ȃ����߂̌Ƒ��ȍH�v�B
	}
	//���̔z����m�ہB���傤�Ǐ��offset�ɍ��v�������Ă���B
	int* hitArray = new int[ offset ];
	for ( int i = 0; i < hitListArraySize; ++i ){
		const HitPair& o = hitListArray[ i ];
		hitArray[ hitListOffset[ o.mI0 ] + hitListSize[ o.mI0 ] ] = o.mI1;
		++hitListSize[ o.mI0 ];//�T�C�Y��+1�B���̃��[�v�𔲂������ɂ͍ŏ��ɐ��������Ɠ�����ԂɂȂ��Ă���B
	}
	SAFE_DELETE_ARRAY( hitListArray );

	//��͔����Ƃɏ����Bsort��unique�����Ȃ����A�������������̂ł���Ȃ��H�v��������P���ɂ�������������̂��B
	for ( int i = 0; i < n; ++i ){
		int* box = &hitArray[ hitListOffset[ i ] ];
		int boxSize = hitListSize[ i ];
		for ( int j = 0; j < boxSize; ++j ){
			if ( box[ j ] >= 0 ){
				addForce( i, box[ j ] );
				//��������z�����ɂ���悤�Ȃ炻�����Ԃ��B
				for ( int k = j + 1; k < boxSize; ++k ){
					if ( box[ k ] == box[ j ] ){
						box[ k ] = -1;
					}
				}
			}
		}
	}
	//��n��
	SAFE_DELETE_ARRAY( hitArray );
	SAFE_DELETE_ARRAY( hitListSize );
	SAFE_DELETE_ARRAY( hitListOffset );

	SAFE_DELETE_ARRAY( boxArray );
	SAFE_DELETE_ARRAY( boxListOffset );
	SAFE_DELETE_ARRAY( boxListSize );

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
