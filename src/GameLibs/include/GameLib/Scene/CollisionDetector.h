#ifndef INCLUDED_GAMELIB_COLLISIONDETECTOR_H
#define INCLUDED_GAMELIB_COLLISIONDETECTOR_H

namespace GameLib{
namespace Math{
	class Vector3;
};
template< class T > class Array;
namespace Scene{
using namespace Math;

///���Փ˔���N���X
/*
���t���[���̎g�p��z�肵�Ă���B
add�œo�^�Adetect�Ōv�Z���s���Č��ʂ�Ԃ��B
add�̌��ʂ�detect��񕪂����Ȃ��̂ŁA
���t���[��add���邱�Ƃɒ��ӁB

���󎎍�i�B����k-d tree�Ȃ̂Ō��Ă݂ė~�����B
*/
class CollisionDetector{
public:
	///���ʂ̔ԍ��y�A
	struct Pair{
		int mId0, mId1;
	};
	static CollisionDetector create( int capacity, int maxLevel = -1 );
	///���ڒǉ��Bdetect��񂾂��L���B�߂�l�͌��ʂɓ����Ă���ID�B
	int add( const Vector3& sphereCenter, float sphereRadius );
	///���o�֐��Badd�������̂͏�����B�����ɂ͌���ID�y�A�z�������B
	void detect( Array< Pair >* resultsOut );

	//�ȉ��C�ɂ��Ȃ��Ă����֐�
	CollisionDetector();
	~CollisionDetector();
	CollisionDetector( const CollisionDetector& );
	operator void*() const;
	CollisionDetector& operator=( const CollisionDetector& );
	void release();
	bool operator==( const CollisionDetector& ) const;
	bool operator!=( const CollisionDetector& ) const;
private:
	class Impl;
	explicit CollisionDetector( Impl* );
	Impl* mImpl;
};

} //namespace Scene
} //namespace GameLib

#endif
