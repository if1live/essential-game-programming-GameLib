#include "GameLib/GameLib.h"
#include "GameLib/Scene/CollisionDetector.h"
#include "GameLib/Base/Impl/ReferenceType.h"
#include "GameLib/Base/Tank.h"
#include "GameLib/Base/Pool.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Functions.h"

namespace {
using namespace GameLib;
using namespace GameLib::Scene;
typedef CollisionDetector::Pair Pair; //���p

class Sphere{
public:
	Vector3 mCenter;
	float mRadius;
};

//Node::Build�̈����Z�b�g
class Args{
public:
	Args( 
	const Sphere* spheres,
	int sphereNumber ) : 
	mSpheres( spheres ),
	mIndexPool( sphereNumber ),
	mResults( sphereNumber ){
		mHitFlags = NEW bool[ sphereNumber * 2 ];
	}
	~Args(){
		mSpheres = 0;
		SAFE_DELETE_ARRAY( mHitFlags );
	}

	const Sphere* mSpheres;
	Pool< int > mIndexPool;
	Tank< Pair > mResults;
	bool* mHitFlags;
};

//k-d tree�̖{��
class Node{
public:
	Node() : mIndices( 0 ), mIndexNumber( 0 ){}
	//�\�z���Ă͒[���画�肵�Ă���
	void build(
	const Vector3& minV, //�͈͍ŏ��l(������)
	const Vector3& maxV, //�͈͍ő�l(�E��O)
	Args* args,
	int restLevel ){
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
		bool* hitR = hitL + mIndexNumber;
		//���E�̕��̐�
		int cl = 0;
		int cr = 0;
		//�ǂ���ɑ���������
		const Sphere* spheres = args->mSpheres;
		for ( int i = 0; i < mIndexNumber; ++i ){ 
			const Sphere& s = spheres[ mIndices[ i ] ];
			//���ɂ��čŏ��ő���擾
			float minP, maxP;
			minP = s.mCenter[ axis ] - s.mRadius;
			maxP = s.mCenter[ axis ] + s.mRadius;
	
			hitL[ i ] = ( minP < div ); //���炩�ɍ��ɏ��� �덷�͍l�����Ȃ��B�������̂̏ꍇ�덷�łǂ������Ȃ��Ă����Ȃ����A��������������v�Z���Ȃ��B
			hitR[ i ] = ( maxP > div ); //���炩�ɉE�ɏ���
			//�������҂�����ł���ꍇ�B���̏ꍇ�E�ɏ���������B�����ɏ���������Ƃ��Ԃ邩�炾�B
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

		//�m�[�h�̓��[�J���ϐ��ł���B�O�_���B�Ɠ����ɔ�����s���Ė߂��Ă��Ă��܂�����A����Ă����K�v�͂Ȃ��B
		Node left;
		Node right;
		left.mIndices = args->mIndexPool.allocate( cl );
		right.mIndices = args->mIndexPool.allocate( cr );

		for ( int i = 0; i < mIndexNumber; ++i ){ //�e�O�p�`�ɂ��Ē��ׂ�
			if ( hitL[ i ] ){
				left.mIndices[ left.mIndexNumber ] = mIndices[ i ];
				++( left.mIndexNumber );
			}
			if ( hitR[ i ] ){
				right.mIndices[ right.mIndexNumber ] = mIndices[ i ];
				++( right.mIndexNumber );
			}
		}
		ASSERT( ( left.mIndexNumber == cl ) && ( right.mIndexNumber == cr ) );

		//�q�b�g�t���O�̈���g���܂킷�֌W�ŁA�ċA�Ăяo���͐U�蕪�����I����Ă���B
		if ( restLevel > 1 ){ //�ŏI���x���͂����Ă΂Ȃ�
			if ( cl > 1 ){
				Vector3 boxDiv = maxV;
				boxDiv[ axis ] = div;
				left.build( minV, boxDiv, args, restLevel - 1 );
			}
			if ( cr > 1 ){
				Vector3 boxDiv = minV;
				boxDiv[ axis ] = div;
				right.build( boxDiv, maxV, args, restLevel - 1 );
			}
		}else{
			//�������肷���B
			left.test( args );
			right.test( args );
		}
	}
	void test( Args* args ) const {
		const Sphere* spheres = args->mSpheres;

		for ( int i = 0; i < mIndexNumber; ++i ){
			int i0 = mIndices[ i ];
			const Sphere& s0 = spheres[ mIndices[ i ] ];
			const Vector3& p0 = s0.mCenter;
			float r0 = s0.mRadius;
			for ( int j = i + 1; j < mIndexNumber; ++j ){
				int i1 = mIndices[ j ];
				const Sphere& s1 = spheres[ mIndices[ j ] ];
				const Vector3& p1 = s1.mCenter;
				float r1 = s1.mRadius;
				//������B�덷�͍l�����Ƃ��ǂ�
				Vector3 d;
				d.setSub( p1, p0 );
				float rsum = r0 + r1;
				float rsum2 = rsum * rsum;
				float dist2 = d.squareLength();
				if ( dist2 <= rsum2 ){
					Pair* pair = args->mResults.add();
					if ( i0 < i1 ){ //����������mId0�Ƃ���B
						pair->mId0 = i0;
						pair->mId1 = i1;
					}else{
						pair->mId0 = i1;
						pair->mId1 = i0;
					}
				}
			}
		}
	}
	int* mIndices;
	int mIndexNumber;
};

} //namespace {}

namespace GameLib{
namespace Scene{

class CollisionDetector::Impl : public ReferenceType{
public:
	Impl( int capacity, int maxLevel ) : 
	mSpheres( 0 ),
	mSphereNumber( 0 ),
	mCapacity( capacity ),
	mMaxLevel( maxLevel ){
		mSpheres = NEW Sphere[ mCapacity ];
	}
	~Impl(){
		SAFE_DELETE_ARRAY( mSpheres );
	}
	int add( const Vector3& c, float r ){
		int i = mSphereNumber;
		ASSERT( i < mCapacity );
		mSpheres[ i ].mCenter = c;
		mSpheres[ i ].mRadius = r;
		++mSphereNumber;
		return i;
	}
	void detect( Array< Pair >* resultsOut ){
#if 1
		Node root;
		Args args( mSpheres, mSphereNumber );
		root.mIndexNumber = mSphereNumber;
		root.mIndices = args.mIndexPool.allocate( mSphereNumber );
		//���m�[�h�Ƀf�[�^�[�U�B�����ɔ͈͂��v�Z
		Vector3 minV( FLOAT_MAX ); //�ő�ŏ�����
		Vector3 maxV( -minV.x ); //�ŏ��ŏ�����
		for ( int i = 0; i < mSphereNumber; ++i ){
			const Sphere& s = mSpheres[ i ];
			Vector3 tMin = s.mCenter;
			Vector3 tMax = s.mCenter;
			Vector3 r( s.mRadius );
			tMin -= r;
			tMin += r;
			minV.min( tMin ); //min��XYZ�ōŏ��l������Ď����ɓ����B
			maxV.max( tMax ); //max��XYZ�ōő�l������Ď����ɓ����B
			//�ԍ��[�U
			root.mIndices[ i ] = i;
		}
		//�m�[�h�͋�����Â��ɓ��鐔�����p�ӂ��邪�A��ł����炭�ς�邾�낤�B
		//��n�ȏ�ŏ���2�̙p�����߁A���̓�{�����p�ӂ���΂悢�B
		int n = mSphereNumber;
		int maxNodeNumber = 1;
		int maxLevel = 0; //�ő啪����
		while ( maxNodeNumber < n ){
			maxNodeNumber <<= 1;
			++maxLevel;
		}
		root.build( minV, maxV, &args, maxLevel );
		//Tank���̏d������������B
		//1.�A�N�Z�X�����������邽�߂ɔz��Ɉڂ�
		int resultSize = args.mResults.size();
		Pair* resultsArray = NEW Pair[ resultSize ];
		int pos = 0;
		while ( !args.mResults.isEnd() ){
			resultsArray[ pos ] = *args.mResults.get();
			++pos;
			args.mResults.toNext();
		}
		//2.��Ԗڂ�i�ł�����̂���������̂��𐔂���B
		int* counts = NEW int[ mSphereNumber * 2 ];
		int* offsets = counts + mSphereNumber; //�ꏏ�Ɋm�ہB����������₱�������Ƃ�����ƃo�O�̌��ɂȂ�
		for ( int i = 0; i < mSphereNumber; ++i ){ //���������[�v
			counts[ i ] = 0;
		}
		for ( int i = 0; i < resultSize; ++i ){
			++counts[ resultsArray[ i ].mId0 ];
		}
		//2.�I�t�Z�b�g�ɕϊ�
		int offset = 0;
		for ( int i = 0; i < mSphereNumber; ++i ){
			offsets[ i ] = offset;
			offset += counts[ i ];
			counts[ i ] = 0;
		}
		//1.�\�[�g��z��Ƀ\�[�g���Ȃ���i�[
		int* sorted = NEW int[ offset ]; //offset�ɂ��傤�Ǎ��v�������Ă���B
		for ( int i = 0; i < resultSize; ++i ){
			int i0 = resultsArray[ i ].mId0;
			sorted[ offsets[ i0 ] + counts[ i0 ] ] = resultsArray[ i ].mId1;
			++counts[ i0 ];
		}
		//�����Ƃɏd�������������̂�߂�l�ɃZ�b�g
		resultsOut->setSize( offset ); //�d�����݂œ��鐔�����m��
		pos = 0;
		for ( int i = 0; i < mSphereNumber; ++i ){
			int* box = &sorted[ offsets[ i ] ];
			int n = counts[ i ];
			for ( int j = 0; j < n; ++j ){
				if ( box[ j ] >= 0 ){
					( *resultsOut )[ pos ].mId0 = i;
					( *resultsOut )[ pos ].mId1 = box[ j ];
					++pos;
					for ( int k = j + 1; k < n; ++k ){ //�����̂����ɂ���΂Ԃ��B
						if ( box[ j ] == box[ k ] ){
							box[ k ] = -1;
						}
					}
				}
			}
		}
		//�ŏI���ʂ̃T�C�Y���k���B���z���͋N���Ȃ��B
		resultsOut->setSize( pos );
		//��n��
		SAFE_DELETE_ARRAY( sorted );
		SAFE_DELETE_ARRAY( counts );
		SAFE_DELETE_ARRAY( resultsArray );
#else //������
		Tank< Pair > results( mSphereNumber );
		for ( int i = 0; i < mSphereNumber; ++i ){
			const Sphere& s0 = mSpheres[ i ];
			const Vector3& p0 = s0.mCenter;
			float r0 = s0.mRadius;
			for ( int j = i + 1; j < mSphereNumber; ++j ){
				const Sphere& s1 = mSpheres[ j ];
				const Vector3& p1 = s1.mCenter;
				float r1 = s1.mRadius;
				//������B�덷�͌�ōl����
				Vector3 d;
				d.setSub( p1, p0 );
				float rsum = r0 + r1;
				float rsum2 = rsum * rsum;
				float dist2 = d.getSquareLength();
				if ( dist2 <= rsum2 ){
					Pair* pair = results.add();
					pair->mId0 = i;
					pair->mId1 = j;
				}
			}
		}
		//�z��ɕϊ�
		results.copyTo( resultsOut );
#endif
		//�����ɋ��̌���0�ɁB
		mSphereNumber = 0;
	}
private:
	void operator=( const Impl& ); //�֎~

	Sphere* mSpheres;
	int mSphereNumber;
	const int mCapacity;
	const int mMaxLevel;
};

CollisionDetector::CollisionDetector( Impl* impl ) : mImpl( impl ){
	if ( mImpl ){
		mImpl->refer();
	}
}

CollisionDetector CollisionDetector::create( int capacity, int maxLevel ){
	CollisionDetector r;
	r.mImpl = NEW Impl( capacity, maxLevel );
	return r;
}

int CollisionDetector::add( const Vector3& sphereCenter, float sphereRadius ){
	return mImpl->add( sphereCenter, sphereRadius );
}

void CollisionDetector::detect( Array< Pair >* resultOut ){
	mImpl->detect( resultOut );
}

#define TYPE CollisionDetector
#include "GameLib/Base/Impl/ReferenceTypeTemplate.h"

} //namespace Scene
} //namespace GameLib
