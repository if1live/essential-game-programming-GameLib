#ifndef INCLUDED_GAMELIB_MATH_RANDOM_H
#define INCLUDED_GAMELIB_MATH_RANDOM_H

namespace GameLib{
namespace Math{

///���������N���X
class Random{
public:
	static Random create( int seed = -1 );
	///��Đݒ�
	void setSeed( int );
	///-0x80000000 - 0x7fffffff
	int getInt();
	///a-1�܂� [0,a)
	int getInt( int a );
	///a����b-1�܂� [a,b)
	int getInt( int a, int b );
	///0-1�����A1�͊܂܂Ȃ� ( 23bit )
	float getFloat();
	///a����b�܂ŁB������b�͊܂܂Ȃ��B[a, b) ( 23bit )
	float getFloat( float a );
	///a����b�܂ŁB������b�͊܂܂Ȃ��B[a, b) ( 23bit )
	float getFloat( float a, float b );

	//�ȉ��m��Ȃ��Ă���
	Random();
	Random( const Random& );
	~Random();
	operator void*() const;
	Random& operator=( const Random& );
	void release();
	bool operator==( const Random& ) const;
	bool operator!=( const Random& ) const;
	bool operator<( const Random& ) const;
private:
	class Impl;
	Impl* mImpl;
};

} //namespace Math
} //namespace GameLib

#endif