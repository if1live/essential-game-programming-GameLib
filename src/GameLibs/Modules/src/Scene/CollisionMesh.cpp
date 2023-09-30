#include "GameLib/GameLib.h"
#include "GameLib/Scene/CollisionMesh.h"
#include "GameLib/PseudoXml/Document.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
#include "GameLib/Base/RefString.h"
#include "GameLib/Base/Impl/ReferenceType.h"
#include "GameLib/Base/Pool.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Functions.h"
//using namespace std;
using namespace GameLib;
using namespace GameLib::PseudoXml;
using namespace GameLib::Math;

//#define STRONG_DEBUG //���̍s�������Ă����k-d tree�Ƒ�����̌��ʂ����S�Ɉ�v���邩�𖈉�m�F����B
int G;

namespace{

//�A���������������֐��B�N���[�����@�ɂĉ����B
//| ax bx cx | | ox | = | dx |
//| ay by cy | | oy | = | dy |
//| az bz cz | | oz | = | dz |
//�Ƃ����`��3�����x�N�^4���󂯎��A1�ɉ������ĕԂ��Bo�����߂���ł���B
//�������A�������̂��߂ɁA������ł���x�ɔ�����s���āA�͈͊O�Ȃ瑦����false��Ԃ�
//true���Ԃ�͓̂��������������ł���B
bool solveLinearSystem(
Vector3* o,
const Vector3& a,
const Vector3& b,
const Vector3& c,
const Vector3& d ){
	float m1021 = a.y * b.z;
	float m1022 = a.y * c.z;

	float m1120 = b.y * a.z;
	float m1122 = b.y * c.z;

	float m1220 = c.y * a.z;
	float m1221 = c.y * b.z;

	//2���ς̍����O�B�s�񎮗p�����A��Ŏg���܂킹��B
	float m1122_m1221 = m1122 - m1221;
	float m1220_m1022 = m1220 - m1022; //�}�C�i�X
	float m1021_m1120 = m1021 - m1120;

	//�s��
	//00*( 11*22 - 12*21 ) - 01*( 10*22 - 12*20 ) + 02*( 10*21 - 11*20 )
	float delta = a.x*( m1122_m1221 ) + b.x*( m1220_m1022 ) + c.x*( m1021_m1120 );
	if ( delta == 0.f ){ //�����Ȃ��B�����ł͉��Z�덷�͖������A������0�ɂȂ������݂̂͂����B
		return false;
	}
	float rcpDelta = 1.f / delta;	
	
	//���Z�덷�̖��͋ɂ߂Ėʓ|�ł���B
	//�����ł͌�������3���ɂƂǂ܂�ƍl���AEPSILON*1000���g���B
	//��������3��������ɂ͓��ς�0.001�ȉ��ɂȂ�K�v������A�����0.05�x�ȉ��̕��s�������B
	static const float zero = 0.f - EPSILON * 1000.f;
	static const float one = 1.f + EPSILON * 1000.f;

	//�ł����t�s��ɉE�Ӄx�N�^���悶�����v�Z����Bdelta�̏��Z�͍Ō�ɍs���B
	float m0221 = c.x * b.z;
	float m0122 = b.x * c.z;
	float m0112 = b.x * c.y;
	float m0211 = c.x * b.y;
	o->x = m1122_m1221 * d.x + ( m0221-m0122 ) * d.y + ( m0112-m0211 ) * d.z;
	//t�̔�����s���B
	o->x *= rcpDelta;
	if ( ( o->x < zero ) || ( o->x > one ) ){
		return false;
	}
	//����u���v�Z
	float m0022 = a.x * c.z;
	float m0220 = c.x * a.z;
	float m0210 = c.x * a.y;
	float m0012 = a.x * c.y;
	o->y = m1220_m1022 * d.x + ( m0022-m0220 ) * d.y + ( m0210-m0012 ) * d.z;
	o->y *= rcpDelta;
	if ( o->y < zero ){
		return false;
	}
	//�Ō��v���v�Z
	float m0120 = b.x * a.z;
	float m0021 = a.x * b.z;
	float m0011 = a.x * b.y;
	float m0110 = b.x * a.y;
	o->z = m1021_m1120 * d.x + ( m0120-m0021 ) * d.y + ( m0011-m0110 ) * d.z;
	o->z *= rcpDelta;
	if ( o->z < zero ){
		return false;
	}
	//u,v�̘a��1�ȉ����`�F�b�N
	if ( ( o->y + o->z ) > one ){
		return false;
	}
	return true;
}

struct Triangle{
	int mIndices[ 3 ];
};

//�Փˏ��������肢���鎞�ɓn�����́B���������炵�ăI�[�o�[�w�b�h�����炷���߂̋���̍�
struct Query{
	Query( 
	const Vector3& begin,
	const Vector3& vec,
	const Triangle* triangles,
	const Vector3* vertices ) :
	mBegin( begin ), mVector( vec ), mTriangles( triangles ), mVertices( vertices ){
	}
	~Query(){
		mTriangles = 0;
		mVertices = 0;
	}
	const Vector3& mBegin;
	const Vector3& mVector;
	const Triangle* mTriangles;
	const Vector3* mVertices;
private:
	void operator=( const Query& ); //�Ȃ�
};

//�m�[�h�\�z������ۂ�build()�ɓn�����������炷���߂̍\����
class Node;
struct BuildArgs{
	BuildArgs( 
	const Triangle* triangles,
	const Vector3* vertices,
	Node* nodes,
	int triangleNumber ) :
	mTriangles( triangles ),
	mVertices( vertices ),
	mNodePos( nodes ),
	mIndexPool( triangleNumber ),
	mHitFlags( 0 ){
		mHitFlags = NEW bool[ triangleNumber * 2 ];
	}
	~BuildArgs(){
		SAFE_DELETE_ARRAY( mHitFlags );
		mTriangles = 0;
		mVertices = 0;
		mNodePos = 0;
	}

	const Triangle* mTriangles;
	const Vector3* mVertices;
	Node* mNodePos; //�m�[�h�z��|�C���^
	Pool< int > mIndexPool; //�C���f�N�X�m�ۗp�v�[��
	bool* mHitFlags; //�ꎞ�ۊǗp�q�b�g�t���O�̈�
private:
	void operator=( const BuildArgs& ); //�Ȃ�
};

bool getIntersectionTriangleAndLineSegment( 
float* tOut, 
const Query& q,
int index ){ //�O�p�`�C���f�N�X
	const Triangle& tri = q.mTriangles[ index ];
	int i0 = tri.mIndices[ 0 ];
	int i1 = tri.mIndices[ 1 ];
	int i2 = tri.mIndices[ 2 ];
	const Vector3& p0 = q.mVertices[ i0 ];
	const Vector3& p1 = q.mVertices[ i1 ];
	const Vector3& p2 = q.mVertices[ i2 ];

	Vector3 minusD, minusE; //�ӂ̋t����
	Vector3 f;
	f.setSub( p0, q.mBegin ); 
	minusD.setSub( p0, p1 );
	minusE.setSub( p0, p2 );
	//�����ł͘A���������������Ĉꔭ�ŋ��߂�B�{��18.3.2�Q�Ƃ̂��ƁB
	//| bx -dx -ex | | ox | = | fx |
	//| by -dy -ey | | oy | = | fy |
	//| bz -dz -ez | | oz | = | fz |
	//bt+du+ev=p0-a �������Ηǂ��B
	Vector3 tuv; //��
	if ( solveLinearSystem( &tuv, q.mVector, minusD, minusE, f ) ){
		*tOut = tuv.x; //t��Ԃ�
		return true;
	}else{
		return false; //�O��
	}
}

class Node{
public:
	struct InnerNode{ //�����m�[�h�̃����o
		float mLine; //������
		Node* mLeft; //��������
		Node* mRight; //�傫����
	};
	struct OuterNode{ //���[�m�[�h�̃����o�B�O�p�`�����͖̂��[�m�[�h�����ł���B
		int* mIndices; //�O�p�`�ԍ��z��
		int mIndexNumber;
	};
	Node() : mAxis( -1 ){
		mInner.mLine = 0.f;
		mInner.mLeft = mInner.mRight = 0;
		mOuter.mIndices = 0;
		mOuter.mIndexNumber = 0;
	}
	//�ċA����֐�
	bool isIntersect( 
	const Query& q,
	float beginT, //a,b�̓I���W�i���̂܂܂ŁA���̃T�C�Y�ɍ��킹��̂�a'=a+beginT*b,b'=a+endT*b,�ƌv�Z���čs���B
	float endT ) const {
		//�O�_�ł���ΒP�ɑ�����
		if ( mAxis == -1 ){ //��������-1�͊O�_�̂��邵
			for ( int i = 0; i < mOuter.mIndexNumber; ++i ){
				int triIdx = mOuter.mIndices[ i ];
++G;
				float dummyT;
				if ( getIntersectionTriangleAndLineSegment( &dummyT, q, triIdx ) ){ //�I���W�i�������Ŕ��肷��B
					return true;
				}
			}
		}else{ //���_�B
			//�ʖ�
			const Vector3& a = q.mBegin;
			const Vector3& b = q.mVector;
			const Node* l = mInner.mLeft;
			const Node* r = mInner.mRight;
			//���ʂɓ����鎞�������߂�B d = a + bt ��� t = ( d - a ) / b
			float divT;
			if ( b[ mAxis ] == 0.f ){ //����0
				divT = FLOAT_MAX;
			}else{
				divT = ( mInner.mLine - a[ mAxis ] ) / b[ mAxis ];
			}
			float endT0, beginT1; //�ŏ��ɍs���m�[�h�̏I�[�����A��ɍs���m�[�h�̎n�_����
			if ( divT >= endT ){ //���̃m�[�h�܂œ��B���Ȃ�
				endT0 = endT;
				beginT1 = endT;
			}else if ( divT > beginT ){ //�r���Ŏ��̃m�[�h�ɓ˓�����
				//���Z�덷���Ȃ����divT�����̂܂ܓn���΂������A�덷���|���̂łƂ肠�����\���L�߂Ɏ��B
				endT0 = ( endT + divT * 15.f ) * ( 1.f / 16.f );
				beginT1 = ( beginT + divT * 15.f ) * ( 1.f / 16.f ); // 1/16�����n�_�ƏI�_��������B0.001�Ȃǂ𑫂��Ɣ͈͂������Ȃ������ɗ��[���z���Ă��܂����炾�B
			}else{ //���̃m�[�h�ւ͌�����Ȃ�
				endT0 = endT;
				beginT1 = endT;
			}
			//�n�_�����߂�B1�v�f�ŗǂ�
			float begin = a[ mAxis ] + beginT * b[ mAxis ];
			//�덷���v�Z�B�g���Ă��鍀����������Ό덷�͋@�B���x�̂܂܂ł���BbeginT�͉ߏ�ɗ]�T������Ă���B
			float e = GameLib::Math::abs( a[ mAxis ] ) + GameLib::Math::abs( beginT * b[ mAxis ] ) + GameLib::Math::abs( mInner.mLine );
			e *= EPSILON * 10.f; //���A�O�̂���10�{
			//�n�_:��
			if ( begin + e < mInner.mLine ){
				if ( l && l->isIntersect( q, beginT, endT0 ) ){
					return true;
				}
				if ( beginT1 < endT ){
					if ( r && r->isIntersect( q, beginT1, endT ) ){
						return true;
					}
				}
			}else if ( begin - e > mInner.mLine ){ //�n�_:�E
				if ( r && r->isIntersect( q, beginT, endT0 ) ){
					return true;
				}
				if ( beginT1 < endT ){
					if ( l && l->isIntersect( q, beginT1, endT ) ){
						return true;
					}
				}
			}else{ //�ǂ������킩��Ȃ��B
				//���A�P�[�X�Ȃ̂Ŏ��Ԕ͈͂͊ۂ��ƂƂ���B
				if ( l && l->isIntersect( q, beginT, endT ) ){
					return true;
				}
				if ( r && r->isIntersect( q, beginT, endT ) ){
					return true;
				}
			}
		}
		return false;
	}
	//�ċA����֐��B�߂�l�͎O�p�`�ԍ�
	void getIntersection(
	int* minIndex, //�ŏ������œ��������O�p�`�ԍ�
	float* minT, //�ŏ�����
	const Query& q,
	float beginT, //a,b�̓I���W�i���̂܂܂ŁA���̃T�C�Y�ɍ��킹��̂�a'=a+beginT*b,b'=a+endT*b,�ƌv�Z���čs���B
	float endT ) const {
		//�O�_�ł���ΒP�ɑ�����
		if ( mAxis == -1 ){ //��������-1�͊O�_�̂��邵
			for ( int i = 0; i < mOuter.mIndexNumber; ++i ){
				int triIdx = mOuter.mIndices[ i ];
++G;
				float t;
				if ( getIntersectionTriangleAndLineSegment( &t, q, triIdx ) ){ //�I���W�i�������Ŕ��肷��B
					if ( t < *minT ){
						*minT = t; //�X�V
						*minIndex = triIdx;
					}
				}
			}
		}else{ //���_�B
			//�ʖ�
			const Vector3& a = q.mBegin;
			const Vector3& b = q.mVector;
			const Node* l = mInner.mLeft;
			const Node* r = mInner.mRight;
			//���ʂɓ����鎞�������߂�B d = a + bt ��� t = ( d - a ) / b
			float divT;
			if ( b[ mAxis ] == 0.f ){ //����0
				divT = FLOAT_MAX;
			}else{
				divT = ( mInner.mLine - a[ mAxis ] ) / b[ mAxis ];
			}
			float endT0, beginT1; //�ŏ��ɍs���m�[�h�̏I�[�����A��ɍs���m�[�h�̎n�_����
			if ( divT >= endT ){ //���̃m�[�h�܂œ��B���Ȃ�
				endT0 = endT;
				beginT1 = endT;
			}else if ( divT > beginT ){ //�r���Ŏ��̃m�[�h�ɓ˓�����
				//���Z�덷���Ȃ����divT�����̂܂ܓn���΂������A�덷���|���̂łƂ肠�����\���L�߂Ɏ��B
				endT0 = ( endT + divT * 15.f ) * ( 1.f / 16.f );
				beginT1 = ( beginT + divT * 15.f ) * ( 1.f / 16.f ); // 1/16�����n�_�ƏI�_��������
			}else{ //���̃m�[�h�ւ͌�����Ȃ�
				endT0 = endT;
				beginT1 = endT;
			}
			//�n�_�����߂�B1�v�f�ŗǂ�
			float begin = a[ mAxis ] + beginT * b[ mAxis ];
			//�덷���v�Z�B�g���Ă��鍀����������Ό덷�͋@�B���x�̂܂܂ł���B
			float e = GameLib::Math::abs( a[ mAxis ] ) + GameLib::Math::abs( beginT * b[ mAxis ] ) + GameLib::Math::abs( mInner.mLine );
			e *= EPSILON * 10.f; //���A�O�̂���10�{
			//�n�_:��
			if ( begin + e < mInner.mLine ){
				if ( l ){
					l->getIntersection( minIndex, minT, q, beginT, endT0 );
				}
				if ( beginT1 < endT ){
					if ( r ){
						r->getIntersection( minIndex, minT, q, beginT1, endT );
					}
				}
			}else if ( begin - e > mInner.mLine ){ //�n�_:�E
				if ( r ){
					r->getIntersection( minIndex, minT, q, beginT, endT0 );
				}
				if ( beginT1 < endT ){
					if ( l ){
						l->getIntersection( minIndex, minT, q, beginT1, endT );
					}
				}
			}else{ //�ǂ������킩��Ȃ��B
				//���A�P�[�X�Ȃ̂Ŏ��Ԕ͈͂͊ۂ��ƂƂ���B
				if ( l ){
					l->getIntersection( minIndex, minT, q, beginT, endT );
				}
				if ( r ){
					r->getIntersection( minIndex, minT, q, beginT, endT );
				}
			}
		}
	}
	//�ċA�\�z�֐�
	/*
	�����̍ہA�������̌v�Z�ɂ͌덷���������A�덷���݂ŏo�������������̗p����B
	�܂�A0.5�ɂȂ�ׂ���������0.51�ɂȂ�̂ł���΁A0.51�𕪊����Ƃ���B
	�������Ƃ��ĉ����g�����̓A���S���Y����C�ӂ�����ł���B
	�����ĎO�p�`�̒��_���W�͌��f�[�^�Ƃ��Ĉ����A�덷�̍����͉��肵�Ȃ��B
	���A�O�p�`�̍��W�Ɋւ��Ă͔�r�ȊO�̉��Z���s�킸�덷�͍������Ȃ��B
	����āA�e�O�p�`���������ɑ΂��č��E�ǂ���ɂ��邩�͌����Ɍ덷���l�����Ɍ��߂Ă悢�B
	�Ȃ��A�������Ɍ����ɏ��O�p�`�́u�E�ɂ���v�Ƃ���B
	*/
	void build(
	const Vector3& minV, //�͈͍ŏ��l
	const Vector3& maxV, //�͈͍ő�l
	BuildArgs* args,
	int restLevel ){ //���Ɖ��񕪊����邩
		//�ŏ���Outer�������g��
		OuterNode* outer = &mOuter;

		//�ŗǂ̕������ǂ����߂邩�͓����肾���A�����ł͍ł���������������B���ꂪ���ϓI�ɂ͑Ó��Ȍ��ʂ��o���B
		int axis = -1; //������
		Vector3 size;
		size.setSub( maxV, minV );
		float maxSize = -FLOAT_MAX;
		for ( int i = 0; i < 3; ++i ){
			if ( size[ i ] > maxSize ){
				maxSize = size[ i ];
				axis = i;
			}
		}
		//������
		float div;
		div = minV[ axis ] + maxV[ axis ];
		div *= 0.5f;
		//���茋�ʂ͌�Ŏg���̂�bool�z��ɕێ����Ă����B�̈��new����ƒx���̂ŁA�����ł�����Ă���B
		bool* hitL = args->mHitFlags;
		bool* hitR = hitL + outer->mIndexNumber;
		//���E�̎O�p�`��
		int cl = 0;
		int cr = 0;
		//�ǂ���ɑ���������
		const Triangle* triangles = args->mTriangles;
		const Vector3* vertices = args->mVertices;
		for ( int i = 0; i < outer->mIndexNumber; ++i ){ //�e�O�p�`�ɂ��Ē��ׂ�
			const Triangle& tri = triangles[ outer->mIndices[ i ] ];
			const Vector3& p0 = vertices[ tri.mIndices[ 0 ] ];
			const Vector3& p1 = vertices[ tri.mIndices[ 1 ] ];
			const Vector3& p2 = vertices[ tri.mIndices[ 2 ] ];
			//���ɂ��čŏ��ő���擾
			float minP, maxP;
			minP = min( p0[ axis ], p1[ axis ] );
			minP = min( minP, p2[ axis ] );
			maxP = max( p0[ axis ], p1[ axis ] );
			maxP = max( maxP, p2[ axis ] );

			hitL[ i ] = ( minP < div ); //���炩�ɍ��ɏ��� �덷���l�����Ă��Ȃ����R�͊֐��̑O�ɂ���R�����g���Q�Ƃ̂��ƁB
			hitR[ i ] = ( maxP > div ); //���炩�ɉE�ɏ���
			//�������҂�����ł���ꍇ�B���̏ꍇ�E�ɏ���������B�����ɏ���������Ƃ��Ԃ邩�炾�B
			//�덷�̍l���͔��莞�ɍs���B
			if ( ( minP == maxP ) && ( minP == div ) ){
				hitR[ i ] = true;
			}
			if ( hitL[ i ] ){
				++cl;
			}
			if ( hitR[ i ] ){
				++cr;
			}
		}
		//�����J�n
		//������InnerNode�ɕς��BOuterNode�̃����o�͈�U���[�J���ϐ��ɑޔ�
		//union�͓�������������̂ł��܂�����ׂ��ł͂Ȃ����A���\��̗��R������Ύd������܂��B
		int* indices = outer->mIndices;
		int indexNumber = outer->mIndexNumber;
		outer->mIndices = 0;
		outer->mIndexNumber = 0;
		outer = 0; //����ȍ~Outer�͂����Ȃ�

		InnerNode* inner = &mInner; //�����ȍ~inner�݂̂ɃA�N�Z�X

		//mAxis��-1�ȊO�������Ă��邱�Ƃ�InnerNode�ł��邵�邵���B
		mAxis = axis; //�����axis==0�Ȃ炻�̂܂܁A1�Ȃ�Y�A2�Ȃ�Z�ɂȂ�
		inner->mLine = div; //����������������

		//���E�m�[�h�̊m��
		if ( cl > 0 ){ //�O�p�`�������
			inner->mLeft = args->mNodePos;
#ifdef STRONG_DEBUG
Vector3 boxDiv = maxV;
boxDiv[ axis ] = div;
inner->mLeft->mMin = minV;
inner->mLeft->mMax = boxDiv;
inner->mLeft->mOrigN = cl;
#endif
			++( args->mNodePos );
			inner->mLeft->mOuter.mIndices = args->mIndexPool.allocate( cl );
		}
		if ( cr > 0 ){
			inner->mRight = args->mNodePos;
#ifdef STRONG_DEBUG
Vector3 boxDiv = minV;
boxDiv[ axis ] = div;
inner->mRight->mMin = boxDiv;
inner->mRight->mMax = maxV;
inner->mRight->mOrigN = cr;
#endif
			++( args->mNodePos );
			inner->mRight->mOuter.mIndices = args->mIndexPool.allocate( cr );
		}
		//���E�m�[�h�ɐU�蕪���B
		for ( int i = 0; i < indexNumber; ++i ){ //�e�O�p�`�ɂ��Ē��ׂ�
			if ( hitL[ i ] ){
				OuterNode* l = &inner->mLeft->mOuter;
				l->mIndices[ l->mIndexNumber ] = indices[ i ];
				++( l->mIndexNumber );
			}
			if ( hitR[ i ] ){
				OuterNode* r = &inner->mRight->mOuter;
				r->mIndices[ r->mIndexNumber ] = indices[ i ];
				++( r->mIndexNumber );
			}
		}
		//�q�b�g�t���O�̈���g���܂킷�֌W�ŁA�ċA�Ăяo���͐U�蕪�����I����Ă���B
		if ( restLevel > 1 ){ //�ŏI���x���͂����Ă΂Ȃ�
			if ( cl > 1 ){
				Vector3 boxDiv = maxV;
				boxDiv[ axis ] = div;
				inner->mLeft->build( minV, boxDiv, args, restLevel - 1 );
			}
			if ( cr > 1 ){
				Vector3 boxDiv = minV;
				boxDiv[ axis ] = div;
				inner->mRight->build( boxDiv, maxV, args, restLevel - 1 );
			}
		}
	}
	void count( int* nodeNumber, int* indexNumber ){
		++( *nodeNumber ); //�����̕������Z
		if ( mAxis != -1 ){
			if ( mInner.mLeft ){
				mInner.mLeft->count( nodeNumber, indexNumber );
			}
			if ( mInner.mRight ){
				mInner.mRight->count( nodeNumber, indexNumber );
			}
		}else{
			*indexNumber += mOuter.mIndexNumber;
		}		
	}
	Node* copy( Node** nodePos, int** indexPos ){
		Node* to = *nodePos; //�R�s�[��
		++( *nodePos );
#ifdef STRONG_DEBUG
to->mMin = mMin;
to->mMax = mMax;
to->mOrigN = mOrigN;
#endif
		to->mAxis = mAxis;
		if ( mAxis != -1 ){ //�����m�[�h�̏ꍇ
			to->mInner.mLine = mInner.mLine;
			if ( mInner.mLeft ){
				to->mInner.mLeft = mInner.mLeft->copy( nodePos, indexPos );
			}
			if ( mInner.mRight ){
				to->mInner.mRight = mInner.mRight->copy( nodePos, indexPos );
			}
		}else{ //�C���f�N�X�m��
			to->mOuter.mIndices = *indexPos;
			//�C���f�N�X���R�s�[
			for ( int i = 0; i < mOuter.mIndexNumber; ++i ){
				to->mOuter.mIndices[ i ] = mOuter.mIndices[ i ];
			}
			to->mOuter.mIndexNumber = mOuter.mIndexNumber;
			( *indexPos ) += mOuter.mIndexNumber;
		}
		return to;
	}
	//���̕��̖��ʂȃm�[�h���폜����B���͈̂ȉ���3���
	//1.�����O�_�ŉE����B���̓��e�������Ɉ����グ��B
	//2.�E���O�_�ō�����B�E�̓��e�������Ɉ����グ��B
	//3.���E���O�_�ŎO�p�`���X�g������B�q�������Ď����ɃR�s�[����B
	void optimize(){
		if ( mAxis == -1 ){ //�O�_�Ȃ珈���Ȃ��B�I���B
			return;
		}
		Node* l = mInner.mLeft;
		Node* r = mInner.mRight;
		//�܂��ċA�Ăяo��
		if ( l ){
			l->optimize();
		}
		if ( r ){
			r->optimize();
		}
		bool lIsOuter = ( l && ( l->mAxis == -1 ) );
		bool rIsOuter = ( r && ( r->mAxis == -1 ) );
		OuterNode* o = 0; //�����グ�O�_������΂����ɓ����B
		if ( lIsOuter && !r ){ //�����O�_�ŉE����B
			o = &l->mOuter;
		}else if ( rIsOuter && !l ){ //�E���O�_�ō�����
			o = &r->mOuter;
		}else if ( lIsOuter && rIsOuter ){ //����`�F�b�N
			OuterNode* lo = &l->mOuter;
			OuterNode* ro = &r->mOuter;
			bool match = true;
			if ( lo->mIndexNumber == ro->mIndexNumber ){
				for ( int i = 0; i < lo->mIndexNumber; ++i ){
					if ( lo->mIndices[ i ] != ro->mIndices[ i ] ){
						match = false;
					}
				}
				if ( match ){ //�S���}�b�`�B���������Ɉ����グ��B
					o = lo;
				}
			}
		}
		if ( o ){ //�����グ��񂠂�B�������O�_�ɂ���B
			mAxis = -1;
			mInner.mLeft = mInner.mRight = 0;
			mOuter.mIndexNumber = o->mIndexNumber;
			mOuter.mIndices = o->mIndices;
		}
	}
	void checkTriangleIndexExistance( Array< bool >& f ){
		if ( mAxis == -1 ){
			for ( int i = 0; i < mOuter.mIndexNumber; ++i ){
				f[ mOuter.mIndices[ i ] ] = true;
			}
		}else{
			if ( mInner.mLeft ){
				mInner.mLeft->checkTriangleIndexExistance( f );
			}
			if ( mInner.mRight ){
				mInner.mRight->checkTriangleIndexExistance( f );
			}
		}
	}
	void checkTriangle( const Triangle* triangles, const Vector3* vertices ) const {
		if ( mAxis == -1 ){ //���_�̂݃`�F�b�N
			return;
		}
		if ( mInner.mLeft ){
			if ( mInner.mLeft->mAxis == -1 ){
				const OuterNode& o = mInner.mLeft->mOuter;
				for ( int i = 0; i < o.mIndexNumber; ++i ){
					int triIdx = o.mIndices[ i ];
					const Triangle& tri = triangles[ triIdx ];
					bool hit = false;
					for ( int j = 0; j < 3; ++j ){
						const Vector3& p = vertices[ tri.mIndices[ j ] ];
						for ( int k = 0; k < 3; ++k ){
							if ( p[ k ] < mInner.mLine ){
								hit = true;
							}
						}
					}
					STRONG_ASSERT( hit );
				}
			}else{
				mInner.mLeft->checkTriangle( triangles, vertices );
			}
		}
		if ( mInner.mRight ){
			if ( mInner.mRight->mAxis == -1 ){
				const OuterNode& o = mInner.mRight->mOuter;
				for ( int i = 0; i < o.mIndexNumber; ++i ){
					int triIdx = o.mIndices[ i ];
					const Triangle& tri = triangles[ triIdx ];
					bool hit = false;
					for ( int j = 0; j < 3; ++j ){
						const Vector3& p = vertices[ tri.mIndices[ j ] ];
						for ( int k = 0; k < 3; ++k ){
							if ( p[ k ] >= mInner.mLine ){
								hit = true;
							}
						}
					}
					STRONG_ASSERT( hit );
				}
			}else{
				mInner.mRight->checkTriangle( triangles, vertices );
			}
		}
	}
#ifdef STRONG_DEBUG
Vector3 mMin; //�f�o�O�p
Vector3 mMax;
int mOrigN;
#endif
	int mAxis; //�������B0:X, 1:Y, 2:Z, -1��OuterNode�B
	union{ //union�̒��̓����������L�����B���[�Ɠ����ł͕K�v�ȃ����o���Ⴄ�̂Ő��\����̂��߂ɂ������Ă���B
		InnerNode mInner;
		OuterNode mOuter;
	};
};

} //namespace{}

namespace GameLib{
namespace Scene{

class CollisionMesh::Impl : public ReferenceType {
public:
	explicit Impl( const char* filename ) :
	mVertices( 0 ), 
	mVertexNumber( 0 ),
	mTriangles( 0 ),
	mTriangleNumber( 0 ),
	mNodes( 0 ),
	mIndicesInNode( 0 ){
		mDocument = Document::create( filename );
	}
	explicit Impl( ConstElement e ) : 
	mVertices( 0 ), 
	mVertexNumber( 0 ),
	mTriangles( 0 ),
	mTriangleNumber( 0 ),
	mNodes( 0 ),
	mIndicesInNode( 0 ){
		createFromElement( e );
	}
	~Impl(){
		SAFE_DELETE_ARRAY( mVertices );
		SAFE_DELETE_ARRAY( mTriangles );
		SAFE_DELETE_ARRAY( mNodes );
		SAFE_DELETE_ARRAY( mIndicesInNode );
	};
	void createFromElement( ConstElement e ){
		//���p�X�B���_���A�O�p�`���𐔂���B
		int n = e.childNumber();
		for ( int i = 0; i < n; ++i ){
			ConstElement child = e.child( i );
			RefString name( child.name() );
			if ( name == "VertexBuffer" ){
				mVertexNumber += child.childNumber();
			}else if ( name == "IndexBuffer" ){
				mTriangleNumber += child.childNumber();
			}
		}
		//�m��
		mVertices = NEW Vector3[ mVertexNumber ];
		mTriangles = NEW Triangle[ mTriangleNumber ];
		//���p�X�B���_�[�U
		int vertexPos = 0;
		int trianglePos = 0;
		for ( int i = 0; i < n; ++i ){
			ConstElement child = e.child( i );
			RefString name( child.name() );
			if ( name == "VertexBuffer" ){
				vertexPos = fillVertices( child, vertexPos );
			}else if ( name == "IndexBuffer" ){
				trianglePos = fillTriangles( child, trianglePos );
			}
		}
		//��O�p�Xk-d tree����
		buildKDTree();
	}
	void buildKDTree(){
		//�m�[�h�͎O�p�`�����z�I��1�Âi�[���ꂽ�Ɖ��肵���Ƃ���܂Ŋ��鐔�����p�ӂ���B
		//�O�p�`��tn�ȏ�ŏ���2�̙p�����߁A���̓�{�����p�ӂ���΂悢�B
		int tn = mTriangleNumber;
		int maxNodeNumber = 1;
		int maxLevel = 0; //�ő啪����
		while ( maxNodeNumber < tn ){
			maxNodeNumber <<= 1;
			++maxLevel;
		}
		Node* tmpNodes = NEW Node[ maxNodeNumber * 2 ];
		//�����\���̏���
		BuildArgs args( mTriangles, mVertices, tmpNodes, mTriangleNumber );

		Node root;
		//�����O�p�`�ԍ��[�U
		root.mOuter.mIndices = args.mIndexPool.allocate( mTriangleNumber );
		root.mOuter.mIndexNumber = mTriangleNumber;
		for ( int i = 0; i < mTriangleNumber; ++i ){
			root.mOuter.mIndices[ i ] = i;
		}
		//���E�̃T�C�Y�𑪒�BXYZ�̍ő�ŏ��𒲂ׂ�B
		Vector3 minV( FLOAT_MAX ); //�ő�ŏ�����
		Vector3 maxV( -minV.x ); //�ŏ��ŏ�����
		for ( int i = 0; i < mVertexNumber; ++i ){
			minV.min( mVertices[ i ] ); //min��XYZ�ōŏ��l������Ď����ɓ����B
			maxV.max( mVertices[ i ] ); //max��XYZ�ōő�l������Ď����ɓ����B
		}
		root.build( minV, maxV, &args, maxLevel );
#ifdef STRONG_DEBUG
root.mMin = minV;
root.mMax = maxV;
#endif
		root.optimize(); //���p�ȃm�[�h������Đ��\���グ��B
		//���ɕK�v�ȗ̈�𑪒肵�āA�Œ肵���̈�ɃR�s�[����B
		int indexNumber = 0;
		int nodeNumber = 0;
		root.count( &nodeNumber, &indexNumber );
		mIndicesInNode = NEW int[ indexNumber ];
		mNodes = NEW Node[ nodeNumber ];
		//�S�m�[�h���R�s�[
		Node* nodePos = mNodes;
		int* indicesPos = mIndicesInNode;
		root.copy( &nodePos, &indicesPos );
		//��n��
		SAFE_DELETE_ARRAY( tmpNodes );
#ifdef STRONG_DEBUG
		Array< bool > triangleExistFlags( mTriangleNumber );
		for ( int i = 0; i < mTriangleNumber; ++i ){
			triangleExistFlags[ i ] = false;
		}
		mNodes[ 0 ].checkTriangleIndexExistance( triangleExistFlags );
		for ( int i = 0; i < mTriangleNumber; ++i ){
			STRONG_ASSERT( triangleExistFlags[ i ] );
		}
		mNodes[ 0 ].checkTriangle( mTriangles, mVertices );
#endif
	}
	bool isReady(){
		bool ok = true;
		if ( mDocument ){
			if ( mDocument.isReady() ){
				ConstElement e = mDocument.root();
				//�q������l�Ŗ��O��Container�̎��Ɍ����āA�q��n���B
				if ( ( e.childNumber() == 1 ) && ( RefString( "Container" ) != e.name() ) ){
					e = e.child( 0 );
				}
				createFromElement( e );
				e.release(); //��ɔj���B
				mDocument.release();
			}else{
				ok = false;
			}
		}
		return ok;
	}
	bool isIntersect( const Vector3& a, const Vector3& b ) const {
		Query q( a, b, mTriangles, mVertices );
		bool ret = mNodes[ 0 ].isIntersect( q, 0.f, 1.f );
#ifdef STRONG_DEBUG //��r�p�������
		bool retCheck = false;
		for ( int i = 0; i < mTriangleNumber; ++i ){
			float dummyT;
			if ( getIntersectionTriangleAndLineSegment( &dummyT, q, i ) ){
				retCheck = true;
			}
		}
		STRONG_ASSERT( ret == retCheck );
#endif
		return ret;
	}
	bool getIntersection(
	float* tOut,
	Vector3* nOut,
	const Vector3& a,
	const Vector3& b ) const {
G = 0;
		float t = FLOAT_MAX; //��������o��
		int index = -1;
		Query q( a, b, mTriangles, mVertices );
		mNodes[ 0 ].getIntersection( &index, &t, q, 0.f, 1.f );
#ifdef STRONG_DEBUG //��r�p�������
		int indexCheck = -1;
		float tCheck = FLOAT_MAX;
		for ( int i = 0; i < mTriangleNumber; ++i ){
			float tmpT;
			if ( getIntersectionTriangleAndLineSegment( &tmpT, q, i ) ){
				if ( tmpT < tCheck ){
					tCheck = tmpT;
					indexCheck = i;

				}
			}
		}
		STRONG_ASSERT( t == tCheck ); //index�͍���Ȃ����Ƃ�����B
#endif
		if ( index != -1 ){
			//�ŏI�I�ɓ��������O�p�`�̏����i�[
			if ( tOut ){
				*tOut = t;
			}
			if ( nOut ){
				int i0 = mTriangles[ index ].mIndices[ 0 ];
				int i1 = mTriangles[ index ].mIndices[ 1 ];
				int i2 = mTriangles[ index ].mIndices[ 2 ];
				const Vector3& p0 = mVertices[ i0 ];
				const Vector3& p1 = mVertices[ i1 ];
				const Vector3& p2 = mVertices[ i2 ];
				Vector3 e0, e1; //Edge(=��)
				e0.setSub( p1, p0 );
				e1.setSub( p2, p0 );
				nOut->setCross( e0, e1 );
				nOut->normalize();
			}
			return true;
		}else{
			return false;
		}
	}
private:
	int fillVertices( ConstElement vb, int pos ){
		int n = vb.childNumber();
		RefString positionStr( "position" );
		for ( int i = 0; i < n; ++i ){
			ConstElement v = vb.child( i );
			int an = v.attributeNumber();
			for ( int j = 0; j < an; ++j ){
				ConstAttribute a = v.attribute( j );
				if ( positionStr == a.name() ){
					a.getFloatValues( &mVertices[ pos ].x, 3 );
					++pos;
				}
			}
		}
		return pos;
	}
	int fillTriangles( ConstElement ib, int pos ){
		int n = ib.childNumber();
		for ( int i = 0; i < n; ++i ){
			ConstElement t = ib.child( i );
			ASSERT( t.attributeNumber() > 0 && "IndexBuffer : It's not valid element." );
			ConstAttribute a = t.attribute( 0 );
			a.getIntValues( mTriangles[ pos ].mIndices, 3 );
			++pos;
		}
		return pos;
	}
	Document mDocument;
	Vector3* mVertices;
	int mVertexNumber;
	Triangle* mTriangles;
	int mTriangleNumber;
	Node* mNodes; //0�Ԃ�����
	int* mIndicesInNode; //�m�[�h���C���f�N�X�e�z��
};

CollisionMesh::CollisionMesh( Impl* impl ) : mImpl( impl ){
	if ( mImpl ){
		mImpl->refer();
	}
}

CollisionMesh CollisionMesh::create( const char* filename ){
	CollisionMesh r;
	r.mImpl = NEW Impl( filename );
	return r;
}

CollisionMesh CollisionMesh::create( ConstElement e ){
	CollisionMesh r;
	r.mImpl = NEW Impl( e );
	return r;
}

bool CollisionMesh::isIntersect( const Vector3& a, const Vector3& b ) const {
	return mImpl->isIntersect( a, b );
}

bool CollisionMesh::getIntersection(
float* t,
Vector3* n,
const Vector3& a,
const Vector3& b ) const {
	return mImpl->getIntersection( t, n, a, b );
}

bool CollisionMesh::isReady(){
	return mImpl->isReady();
}

#define TYPE CollisionMesh
#include "GameLib/Base/Impl/ReferenceTypeTemplate.h"

} //namespace Scene
}//namespace GameLib