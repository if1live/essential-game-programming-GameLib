#include "GameLib/Framework.h"
#include "GameLib/Threading/Thread.h"
#include <sstream>
using namespace GameLib::Threading;

//足す変数
int gCount;
//鍵。というか使用中の札
bool gLock;

//関数クラス
class IncrementThread : public Thread{
public:
	IncrementThread(){
		start();
	}
	~IncrementThread(){
		wait();
	}
	//100万回インクリメント
	void operator()(){
		for ( int i = 0; i < 1000*1000; ++i ){
			while ( gLock ){ //使用中。そわそわしながら待て
				;
			}
			gLock = true; //鍵かけて
			gCount += 2; //用を足し、
			gLock = false; //すっきり。鍵を開ける。
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
		} //ここでスレッドデストラクト
		std::ostringstream o;
		o << gCount;
		drawDebugString( 0, 0, o.str().c_str() );
	}
} //namespace GameLib

