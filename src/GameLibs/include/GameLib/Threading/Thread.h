#ifndef INCLUDED_GAMELIB_THREADING_THREAD_H
#define INCLUDED_GAMELIB_THREADING_THREAD_H

namespace GameLib{
namespace Threading{

///�X���b�h���ۊ��
/*!
operator()()��p�ӂ����h�������A
start()����Ύ��s���n�܂�B

<���ӁI>
�f�X�g���N�^���ĂԑO�ɂ�wait()���ĂԂ��AisFinished()��true��Ԃ��܂ŌĂԂ���
�ǂ��炩���K�v�B�ǂ�������Ȃ��Ńf�X�g���N�g�����ASSERT����B
*/
class Thread{
public:
	///����������ō��
	virtual void operator()() = 0;
	///���s�J�n�B���s���Ȃ�I����҂��čĎ��s�B
	void start();
	///�I����҂�
	void wait();
	///�I�����������
	bool isFinished();
	///���ڌĂт͂��Ȃ��R���X�g���N�^
	Thread();
protected: //���͒���delete�ł��܂���B�h����delete���܂��傤�B
	~Thread();
private:
	class Impl;
	Impl* mImpl;
};

} //namespace Threading
} //namespace GameLib

#endif
