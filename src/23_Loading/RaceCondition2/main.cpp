#include "GameLib/Framework.h"
#include "GameLib/Threading/Thread.h"
#include "GameLib/Threading/Mutex.h"
#include <sstream>
using namespace std;
using namespace GameLib::Threading;

class Bool{
public:
	Bool() : mBool( false ){
		mMutex = Mutex::create();
	}
	void operator=( bool f ){
		mMutex.lock();
		mBool = f;
		mMutex.unlock();
	}
	bool get(){
		//�ꌩ��Ɍ����邾�낤���A�ǂގ���lock(),unlock()���Ă����B�L���b�V���̖��ƁA�R���p�C���̂�����������}�����邽�߂��B
		mMutex.lock();
		bool r = mBool;
		mMutex.unlock();
		return r;
	}
private:
	bool mBool;
	Mutex mMutex;
};

int gX;
Bool gWrite;
Bool gRead;

//�X���b�h�Ŏ��s����֐������N���X
class MyThread : public Thread{
public:
	MyThread(){ start(); }
	~MyThread(){ wait(); }
	void operator()(){
		for ( int i = 0; i < 100; ++i ){
			while ( !gRead.get() ){ //�ǂ܂��̂�҂�
				;
			}
			gX += 2;
			gRead = false; //�܂��ǂ܂�ĂȂ�
			gWrite = true; //�����܂���
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
			while ( !gWrite.get() ){ //�������̂�҂�
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
