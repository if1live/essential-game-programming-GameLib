#include "GameLib/Framework.h"
#include "GameLib/Threading/Thread.h"
#include <sstream>
using namespace GameLib::Threading;

//�����ϐ�
int gCount;
//���B�Ƃ������g�p���̎D
bool gLock;

//�֐��N���X
class IncrementThread : public Thread{
public:
	IncrementThread(){
		start();
	}
	~IncrementThread(){
		wait();
	}
	//100����C���N�������g
	void operator()(){
		for ( int i = 0; i < 1000*1000; ++i ){
			while ( gLock ){ //�g�p���B���킻�킵�Ȃ���҂�
				;
			}
			gLock = true; //��������
			gCount += 2; //�p�𑫂��A
			gLock = false; //��������B�����J����B
		}
	}
};

namespace GameLib{
	void Framework::update(){
		gCount = 0;
		setFrameRate( 60 );
		{
			IncrementThread t0;
			IncrementThread t1;
		} //�����ŃX���b�h�f�X�g���N�g
		std::ostringstream o;
		o << gCount;
		drawDebugString( 0, 0, o.str().c_str() );
	}
} //namespace GameLib

