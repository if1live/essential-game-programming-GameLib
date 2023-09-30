#ifndef INCLUDED_SEQUENCE_GAME_READY_H
#define INCLUDED_SEQUENCE_GAME_READY_H

#include "Sequence/Game/Child.h"

class Image;

namespace Sequence{
namespace Game{
class Parent;

class Ready : public Child{
public:
	Ready();
	~Ready();
	Child* update( Parent* );
private:
	Image* mImage;
	int mCount;
	bool mStarted;
};

} //namespace Game
} //namespace Sequence

#endif