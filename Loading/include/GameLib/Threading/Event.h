#ifndef INCLUDED_GAMELIB_THREADING_EVENT_H
#define INCLUDED_GAMELIB_THREADING_EVENT_H

namespace GameLib{
namespace Threading{

class Event{
public:
	static Event create( bool initialiState = false );
	///trueにされるのを待つ
	void wait() const;
	///trueにする
	void set();
	///falseにする
	void reset();
	///true?それともfalse?
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
