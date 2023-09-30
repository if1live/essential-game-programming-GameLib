#include <windows.h>
#undef min
#undef max
#include "GameLib/GameLib.h"
#include "GameLib/Threading/Mutex.h"
#include "GameLib/Base/Impl/ReferenceType.h"

namespace GameLib{
namespace Threading{

//���d�A�����b�N�A���d���b�N���X�����o���邽�߂ɁA���g�̓Z�}�t�H�ɂ��Ă����B
//Release�ł�CriticalSection�ɐ؂�ւ���̂�������������Ȃ��B
class Mutex::Impl : public ReferenceType{
public:
	Impl() : mHandle( 0 ){
		mHandle = CreateSemaphore( NULL, 1, 1, NULL );
	}
	~Impl(){
		CloseHandle( mHandle );
	}
	void lock(){
		WaitForSingleObject( mHandle, INFINITE );
	}
	void unlock(){
		LONG prev;
		ReleaseSemaphore( mHandle, 1, &prev );
		STRONG_ASSERT( prev == 0 && "unlock() is called twice! Check lock-unlock pair." );
	}
	HANDLE mHandle;
};

Mutex Mutex::create(){
	Mutex r;
	r.mImpl = NEW Impl();
	return r;
}

void Mutex::lock(){
	mImpl->lock();
}

void Mutex::unlock(){
	mImpl->unlock();
}

#define TYPE Mutex
#include "GameLib/Base/Impl/ReferenceTypeTemplate.h"


} //namespace Threading
} //namespace GameLib
