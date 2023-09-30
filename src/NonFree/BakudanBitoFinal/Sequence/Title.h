#ifndef INCLUDED_SEQUENCE_TITLE_H
#define INCLUDED_SEQUENCE_TITLE_H

#include "Sequence/Child.h"

class Image;

namespace Sequence{
class Parent;

class Title : public Child{
public:
	Title();
	~Title();
	void update( Parent* );
private:
	Image* mImage; //�^�C�g����ʉ摜
	Image* mCursorImage; //�^�C�g����ʉ摜
	int mCursorPosition;
};

} //namespace Sequence

#endif