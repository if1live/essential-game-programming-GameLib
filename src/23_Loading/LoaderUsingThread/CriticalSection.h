#ifndef INCLUDED_CRITICAL_SECTION_H
#define INCLUDED_CRITICAL_SECTION_H

//�N���e�B�J���Z�N�V�����N���X
class CriticalSection{
public:
	CriticalSection();
	void enter();
	void leave();
private:
	int mEntered;
};

#endif
