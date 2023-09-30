#ifndef INCLUDED_SEQUENCE_GAME_CHILD_H
#define INCLUDED_SEQUENCE_GAME_CHILD_H

#include "Sequence/Base.h"

namespace Sequence{
namespace Game{
class Parent;

class Child : public Base{
public:
	virtual ~Child();
	Base* update( Base* ); //����͎��̂�����Ă��
	virtual Base* update( Parent* ) = 0;
};


} //namespace Game
} //namespace Sequence

#endif