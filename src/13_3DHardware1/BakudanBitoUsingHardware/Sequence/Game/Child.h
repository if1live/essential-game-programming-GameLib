#ifndef INCLUDED_SEQUENCE_GAME_CHILD_H
#define INCLUDED_SEQUENCE_GAME_CHILD_H

namespace Sequence{
namespace Game{
class Parent;

class Child{
public:
	virtual ~Child(){} //�Ȃɂ����Ȃ��̂ł���ł���
	virtual void update( Parent* ) = 0;
};

} //namespace Game
} //namespace Sequence

#endif