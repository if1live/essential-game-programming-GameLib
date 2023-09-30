#ifndef INCLUDED_GAMELIB_THREADING_SEMAPHORE_H
#define INCLUDED_GAMELIB_THREADING_SEMAPHORE_H

namespace GameLib{
namespace Threading{

///���܂����l�����������Ƀ��b�N�ł���Mutex�B
class Semaphore{
public:
	///�����̏����l�Ő����BmaxCount��0����startCount�ɂȂ�B
	static Semaphore create( int startCount, int maxCount = 0 );
	///�J�E���g��1���炻���Ƃ���B���ł�0���Ɩ��炳���B
	void decrease();
	///�J�E���g�𑝂₷�B�������ȗ������1�ÂB
	void increase( int value = 1 );

	Semaphore();
	~Semaphore();
	Semaphore( const Semaphore& );
	operator void*() const;
	Semaphore& operator=( const Semaphore& );
	void release();
	bool operator==( const Semaphore& ) const;
	bool operator!=( const Semaphore& ) const;
private:
	class Impl;
	Impl* mImpl;
};

} //namespace Threading
} //namespace GameLib

#endif
