#include "GameLib/Framework.h"
#include "GameLib/Threading/Thread.h"
using namespace GameLib;
using namespace GameLib::Threading;

//�X���b�h�h��
class MyThread : public Thread{
public:
	void operator()(){
		cout << "I am a thread." << endl;
	}
};

MyThread* gThread;

namespace GameLib{
	void Framework::update(){
		if ( !gThread ){
			//�X���b�h�����A�J�n
			gThread = new MyThread();
			gThread->start();
		}
		if ( isEndRequested() ){
			gThread->wait(); //�I����Ă���������ƁB
			SAFE_DELETE( gThread );
		}
	}
} //namespace GameLib
