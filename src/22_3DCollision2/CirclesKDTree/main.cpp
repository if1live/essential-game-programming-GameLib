#include "GameLib/Framework.h"
#include "GameLib/Math.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/Input/Keyboard.h"
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
const int gHitListBlockSize = 10000; //�Ă��Ƃ�
bool testCircles( int index0, int index1 ); //1�Â̔���֐�(��:���ŗ͂͗^���Ȃ�)
void addForce( int i0, int i1 ); //�͂𑫂��̂͂�����
const int MAX_LEVEL = 13; //�ő啪���i��

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

struct Node{
	enum Direction{
		DIR_X,
		DIR_Y,
		DIR_NONE,
	};
	Node() : 
	mDirection( DIR_NONE ),
	mLeft( 0 ), 
	mRight( 0 ), 
	mIndices( 0 ),
	mIndexNumber ( 0 ){
	}
	//�ċA�\�z(������x,y�͈̔�)
	void build( 
		double x0, 
		double x1,
		double y0,
		double y1,
		int* indices,
		int* indexPos,
		Node* nodes,
		int* nodePos,
		int restLevel, //���Ɖ��񕪗􂷂�̂��H
		bool drawLine );
	//�ċA����
	void detect( list< HitPair* >& hitList, int* hitListBlockPos, int* test, int* hit ) const; 

	Direction mDirection;
	Node* mLeft;
	Node* mRight;
	int* mIndices;
	int mIndexNumber;
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

	//�m�[�h�z���p�ӁB
	//�ő�MAX_LEVEL�i�ɂȂ�̂ŁA�m�[�h�̐���2^MAX_LEVEL-1
	//1�i�Ȃ�1�A2�i�Ȃ�1+2�A3�i�Ȃ�1+2+4�A�ƍl���Ă����΂����B
	int nodePosMax = 1;
	for ( int i = 0; i < MAX_LEVEL; ++i ){
		nodePosMax *= 2;
	}
	nodePosMax -= 1;
	//�C���f�N�X���̍ő�l�͂����ɂȂ邩�H
	//�O�����Ă͂킩��Ȃ��̂�100�{�d������Ƃ��Ă����B����͉ߏ�B
	int indexPosMax = n * 100;
	Node* nodes = new Node[ nodePosMax ];
	int* indices = new int[ indexPosMax ];
	//���m�[�h��p�ӂ���B
	Node root;
	root.mIndices = indices;
	for ( int i = 0; i < n; ++i ){
		root.mIndices[ i ] = i;
	}
	root.mIndexNumber = n;
	int indexPos = n; //�؂�o���ʒu
	int nodePos = 1; //�؂�o���ʒu
	//�ċA�\�z(���̊֐��̒�������̃~�\)
	bool draw = true;
	if ( Input::Manager::instance().keyboard().isOn( ' ' ) ){
		draw = false;
	}
	root.build( 
		gMinimum.x, 
		gMaximum.x, 
		gMinimum.y,
		gMaximum.y, 
		indices, 
		&indexPos, 
		nodes, 
		&nodePos, 
		MAX_LEVEL - 1, //��MAX_LEVEL-1�񕪊�����A�̈Ӗ��B
		draw );
	//�ċA����(���̊֐��̒��͂���Ȃɓ���Ȃ�)
	list< HitPair* > hitList;
	int hitListBlockPos = gHitListBlockSize;
	root.detect( hitList, &hitListBlockPos, test, hit ); //�������͍X�V����̂Ń|�C���^

	//�d���r�����ďՓˉ����B�������牺��ArrayList3�Ɠ���B
	//�R�[�h���V���v���ɂ��Ă����������邽�߁A
	//�z��̃��X�g�𕁒ʂ̔z��ɒ���
	HitPair* hitListArray = new HitPair[ hitList.size() * gHitListBlockSize ];
	int hitListArraySize = 0;
	typedef list< HitPair* >::iterator HIt; //�ʖ�
	int blockPos = 0;
	int blockNumber = static_cast< int >( hitList.size() );
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
	int offset = 0;
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
	SAFE_DELETE_ARRAY( indices );
	SAFE_DELETE_ARRAY( nodes );
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

//����������̃~�\�ł���
void Node::build( 
double x0, 
double x1, 
double y0, 
double y1,
int* indices,
int* indexPos,
Node* nodes,
int* nodePos,
int restLevel, //�㉽�񊄂邩
bool draw ){
	//�����ł͈�ԒP���ȕ��@�Ŋ���B
	//x,y�̒������Ŋ���̂��B
	double div; //������
	if ( ( x1 - x0 ) > ( y1 - y0 ) ){
		mDirection = DIR_X;
		div = ( x0 + x1 ) * 0.5;
	}else{
		mDirection = DIR_Y;
		div = ( y0 + y1 ) * 0.5;
	}
	//�����鏀��
	int c0, c1;
	c0 = c1 = 0; //���E�m�[�h�Ɋ���U��O�p�`�̐�
	//�q�m�[�h�m��
	mLeft = &nodes[ *nodePos + 0 ];
	mRight = &nodes[ *nodePos + 1 ];
	*nodePos += 2;
	//��͕����ɉ����ĕ���
	if ( mDirection == DIR_X ){
		for ( int i = 0; i < mIndexNumber; ++i ){
			const Circle& c = gCircles[ mIndices[ i ] ]; //�O���[�o���͌����ˁB�}�l����񂶂�Ȃ���H
			const Vector2& p = c.mPosition;
			if ( p.x - R <= div ){ ++c0; } //�����̃R�[�h�͐��l���Z�덷���l����ƕs�K���B�l���Ă݂悤�B
			if ( p.x + R >= div ){ ++c1; } //��������<= divX+e, >= divX-e�ƂȂ�Be�͌덷���e�͈͂��B
		}
		//�q�z��؂�o��
		mLeft->mIndices = indices + *indexPos;
		*indexPos += c0;
		mRight->mIndices = indices + *indexPos;
		*indexPos += c1;
		//���z
		for ( int i = 0; i < mIndexNumber; ++i ){
			int idx = mIndices[ i ];
			const Circle& c = gCircles[ idx ]; //�O���[�o���͌����ˁB�}�l����񂶂�Ȃ���H
			const Vector2& p = c.mPosition;
			if ( p.x - R <= div ){
				mLeft->mIndices[ mLeft->mIndexNumber ] = idx;
				++mLeft->mIndexNumber;
			}
			if ( p.x + R >= div ){
				mRight->mIndices[ mRight->mIndexNumber ] = idx;
				++mRight->mIndexNumber;
			}
		}
		if ( restLevel > 1 ){ //�q�ɂ܂�����������Ȃ�(>0�łȂ����R���l���Ă݂悤)
			if ( c0 > 1 ){ //2�ȏ�Ȃ��Ɗ���Ӗ��͂Ȃ�
				mLeft->build( x0, div, y0, y1, indices, indexPos, nodes, nodePos, restLevel - 1, draw );
			}
			if ( c1 > 1 ){
				mRight->build( div, x1, y0, y1, indices, indexPos, nodes, nodePos, restLevel - 1, draw );
			}
		}
		if ( draw ){ //�������`��B
			double p[ 4 ][ 2 ];
			p[ 0 ][ 0 ] = p[ 1 ][ 0 ] = div - 0.25 + 160.0;
			p[ 2 ][ 0 ] = p[ 3 ][ 0 ] = div + 0.25 + 160.0;
			p[ 0 ][ 1 ] = p[ 2 ][ 1 ] = y0 + 120.0;
			p[ 1 ][ 1 ] = p[ 3 ][ 1 ] = y1 + 120.0;
			Framework::instance().drawTriangle2D( p[ 0 ], p[ 1 ], p[ 2 ], 0, 0, 0, 0xff00ff00, 0xff00ff00, 0xff00ff00 );
			Framework::instance().drawTriangle2D( p[ 3 ], p[ 1 ], p[ 2 ], 0, 0, 0, 0xff00ff00, 0xff00ff00, 0xff00ff00 );
		}
	}else{
		for ( int i = 0; i < mIndexNumber; ++i ){
			const Circle& c = gCircles[ mIndices[ i ] ]; //�O���[�o���͌����ˁB�}�l����񂶂�Ȃ���H
			const Vector2& p = c.mPosition;
			if ( p.y - R <= div ){ ++c0; } //�����̃R�[�h�͐��l���Z�덷���l����ƕs�K���B�l���Ă݂悤�B
			if ( p.y + R >= div ){ ++c1; } //��������<= divX+e, >= divX-e�ƂȂ�Be�͌덷���e�͈͂��B
		}
		//�q�z��؂�o��
		mLeft->mIndices = indices + *indexPos;
		*indexPos += c0;
		mRight->mIndices = indices + *indexPos;
		*indexPos += c1;
		//���z
		for ( int i = 0; i < mIndexNumber; ++i ){
			int idx = mIndices[ i ];
			const Circle& c = gCircles[ idx ]; //�O���[�o���͌����ˁB�}�l����񂶂�Ȃ���H
			const Vector2& p = c.mPosition;
			if ( p.y - R <= div ){
				mLeft->mIndices[ mLeft->mIndexNumber ] = idx;
				++mLeft->mIndexNumber;
			}
			if ( p.y + R >= div ){
				mRight->mIndices[ mRight->mIndexNumber ] = idx;
				++mRight->mIndexNumber;
			}
		}
		if ( restLevel > 1 ){ //�q�ɂ܂�����������Ȃ�(>0�łȂ����R���l���Ă݂悤)
			if ( c0 > 1 ){ //2�ȏ�Ȃ��Ɗ���Ӗ��͂Ȃ�
				mLeft->build( x0, x1, y0, div, indices, indexPos, nodes, nodePos, restLevel - 1, draw );
			}
			if ( c1 > 1 ){
				mRight->build( x0, x1, div, y1, indices, indexPos, nodes, nodePos, restLevel - 1, draw );
			}
		}
		if ( draw ){ //�������`��B
			double p[ 4 ][ 2 ];
			p[ 0 ][ 0 ] = p[ 1 ][ 0 ] = x0 + 160.0;
			p[ 2 ][ 0 ] = p[ 3 ][ 0 ] = x1 + 160.0;
			p[ 0 ][ 1 ] = p[ 2 ][ 1 ] = div - 0.25 + 120.0;
			p[ 1 ][ 1 ] = p[ 3 ][ 1 ] = div + 0.25 + 120.0;
			Framework::instance().drawTriangle2D( p[ 0 ], p[ 1 ], p[ 2 ], 0, 0, 0, 0xff00ff00, 0xff00ff00, 0xff00ff00 );
			Framework::instance().drawTriangle2D( p[ 3 ], p[ 1 ], p[ 2 ], 0, 0, 0, 0xff00ff00, 0xff00ff00, 0xff00ff00 );
		}
	}
	//�����̔z��͗v��Ȃ��B�������J���͂ł��Ȃ��̂ŁA�ԈႢ���N����ʂ悤0�ɂ��Ă����B
	//�Ȃ��A��������̕����ōė��p����悤�ɃR�[�h���������Ƃ��o���A
	//��������ΑO�����Ċm�ۂ���z������Ȃ�Z���ł��邪�A���Ȃ蕡�G�ɂȂ�B
	mIndices = 0;
	mIndexNumber = 0;
}

//�g�����͊ȒP
void Node::detect( list< HitPair* >& hitList, int* hitListBlockPos, int* test, int* hit ) const{
	if ( mDirection == DIR_NONE ){ //�������Ȃ��C�R�[���������I�_�B
		for ( int i = 0; i < mIndexNumber; ++i ){
			int i0 = mIndices[ i ];
			for ( int j = i + 1; j < mIndexNumber; ++j ){
				int i1 = mIndices[ j ];
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
					if ( *hitListBlockPos == gHitListBlockSize ){
						hitList.push_back( new HitPair[ gHitListBlockSize ] );
						*hitListBlockPos = 0;
					}
					hitList.back()[ *hitListBlockPos ] = hit;
					++( *hitListBlockPos );
				}
			}
		}
	}else{ //�q������̂Ŏq�ɓn���B
		mLeft->detect( hitList, hitListBlockPos, test, hit );
		mRight->detect( hitList, hitListBlockPos, test, hit );
	}
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
