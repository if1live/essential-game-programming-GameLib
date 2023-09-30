#ifndef INCLUDED_GAMELIB_THREADING_EVENT_H
#define INCLUDED_GAMELIB_THREADING_EVENT_H

namespace GameLib{
namespace Threading{

class Event{
public:
	static Event create( bool initialiState = false );
	///true�ɂ����̂�҂�
	void wait() const;
	///true�ɂ���
	void set();
	///false�ɂ���
	void reset();
	///true?����Ƃ�false?
	bool isSet() const;

	Event();
	~Event();
	Event( const Event& );
	operator void*() const;
	Event& operator=( const Event& );
	void release();
	bool operator==( const Event& ) const;
	bool operator!=( const Event& ) const;
private:
	class Impl;
	Impl* mImpl;
};

} //namespace Threading
} //namespace GameLib

#endif
