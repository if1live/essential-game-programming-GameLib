#include "GameLib/Framework.h"
#include "GameLib/Threading/Thread.h"
#include <sstream>
using namespace std;
using namespace GameLib::Threading;

int gX;
bool gWrite;
bool gRead;

//�X���b�h�Ŏ��s����֐������N���X
class MyThread : public Thread{
public:
	MyThread(){ start(); }
	~MyThread(){ wait(); }
	void operator()(){
		for ( int i = 0; i < 100; ++i ){
			while ( !gRead ){ //�ǂ܂��܂ő҂�
				;
			}
			gX += 2;
			gRead = false;
			gWrite = true;
		}
	}
};

namespace GameLib{
	void Framework::update(){
		setFrameRate( 60 );
		ostringstream o;

		gX = 0;
		gWrite = false;
		gRead = true;
		MyThread t;
		for ( int i = 0; i < 100; ++i ){
			while ( !gWrite ){ //�������܂ő҂�
				;
			}
			o.str( "" );
			o << gX;
			gWrite = false;
			gRead = true;
			drawDebugString( ( i % 16 ) * 4, i / 16, o.str().c_str() );
		}
	}
} //namespace GameLib
