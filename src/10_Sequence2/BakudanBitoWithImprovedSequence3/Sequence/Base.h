#ifndef INCLUDED_SEQUENCE_BASE_H
#define INCLUDED_SEQUENCE_BASE_H

namespace Sequence{

class Base{
public:
	virtual ~Base(){} //�Ȃɂ����Ȃ��̂ł���ł���
	virtual Base* update( Base* ) = 0;
};

} //namespace Sequence

#endif