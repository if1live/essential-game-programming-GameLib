#ifndef INCLUDED_SEQUENCE_CHILD_H
#define INCLUDED_SEQUENCE_CHILD_H

namespace Sequence{
class Parent;

class Child{
public:
	virtual ~Child(){} //�Ȃɂ����Ȃ��̂ł���ł���
	virtual void update( Parent* ) = 0;
};

} //namespace Sequence

#endif