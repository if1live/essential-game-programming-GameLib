#ifndef INCLUDED_SEQUENCE_BASE_H
#define INCLUDED_SEQUENCE_BASE_H

namespace Sequence{

class Base{
public:
	virtual ~Base(){} //なにもしないのでこれでいい
	virtual Base* update( Base* ) = 0;
};

} //namespace Sequence

#endif