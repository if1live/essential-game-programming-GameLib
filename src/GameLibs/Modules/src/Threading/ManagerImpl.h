#ifndef INCLUDED_GAMELIB_THREADING_MANAGERIMPL_H
#define INCLUDED_GAMELIB_THREADING_MANAGERIMPL_H

#include <windows.h>
#include <process.h>
#undef min
#undef max
#include <list>

#include "GameLib/GameLib.h"
#include "GameLib/Threading/Manager.h"
#include "GameLib/Threading/Thread.h"
#include "GameLib/Threading/Event.h"
#include "GameLib/Threading/Mutex.h"
#include "GameLib/Threading/Semaphore.h"

namespace GameLib{
namespace Threading{
using namespace std;

class ManagerImpl{
public:
	struct Entry{
		Entry() : mThread( 0 ), mFinished( 0 ){}
		Entry( Thread* t, Event* e ) : mThread( t ), mFinished( e ){}
		~Entry(){ 
			mThread = 0;
			mFinished = 0;
		}
		Thread* mThread;
		Event* mFinished;
	};
	ManagerImpl( int additionalThreadNumber ) : 
	mThreads( 0 ), 
	mThreadNumber( 0 ), 
	mEndRequest( false ),
	mCoreNumber( 1 ){ //�Œ�ł�1
		mQueueSemaphore = Semaphore::create( 0, 0x7fffffff ); //�ő�l�͖���
		mLock = Mutex::create();

		//�R�A���擾
#ifdef NDEBUG //Debug�łł͎����ł͈�������Ȃ��B�J���}�V���œ����đ��œ����Ȃ��A��h�����߁B
		HANDLE process = GetCurrentProcess();
		DWORD processMask;
		DWORD systemMask;
		BOOL succeeded = GetProcessAffinityMask( process, &processMask, &systemMask );
		if ( succeeded != 0 ){ //���s����Ă����邯�ǂ�...
			for ( int i = 1; i < 32; ++i ){
				if ( processMask & ( 1 << i ) ){
					++mCoreNumber;
				}
			}
		}
#endif
		mThreadNumber = additionalThreadNumber + mCoreNumber + 1; //�Œ�ł��R�A��+1(���[�_�p)�͍��
		mThreads = new HANDLE[ mThreadNumber ];
		//�N��
		for ( int i = 0; i < mThreadNumber; ++i ){
			unsigned dummy;
			mThreads[ i ] = reinterpret_cast< HANDLE >( _beginthreadex( NULL, 0, execute, this, 0, &dummy ) );
			SetThreadPriority( mThreads[ i ], THREAD_PRIORITY_BELOW_NORMAL ); //�D��x������Ɖ�
		}
	}
	~ManagerImpl(){
		//�I���҂�
		mLock.lock();
		mEndRequest = true;
		mLock.unlock();

		//�X���b�h�I���҂�
		for ( int i = 0; i < mThreadNumber; ++i ){
			while ( true ){
				//�I���Ȃ��ԃZ�}�t�H�𑝂₵�Ȃ��炮�邮��҂Bdecrease�ő҂��Ă�̂�increase���Ȃ��ƋN���Ȃ��B
				mQueueSemaphore.increase(); //�L���[�ɉ��������Ă���悤�Ɍ��������ċN����
				if ( WAIT_OBJECT_0 == WaitForSingleObject( mThreads[ i ], 1 ) ){ 
					CloseHandle( mThreads[ i ] );
					mThreads[ i ] = 0;
					break;
				}
			}
		}
		SAFE_DELETE_ARRAY( mThreads );
	}
	static unsigned WINAPI execute( void* p ){
		ManagerImpl* m = reinterpret_cast< ManagerImpl* >( p );
		m->execute();
		return 0;
	}
	void execute(){
		while ( true ){
			mQueueSemaphore.decrease(); //�L���[�ǉ���҂�

			mLock.lock();
			if ( mEndRequest ){
				mLock.unlock();
				break;
			}
			Entry e = mQueue.front();
			mQueue.pop_front(); //�L���[������o��
			mLock.unlock();

			( *( e.mThread ) )(); //���s
			e.mFinished->set(); //�I���t���O���Ă�
		}
	}
	void add( Thread* t, Event* finishFlag ){
		Entry e( t, finishFlag );

		mLock.lock();
		mQueue.push_back( e );
		mLock.unlock();

		mQueueSemaphore.increase(); //�����܂���
	}
	int getCoreNumber() const {
		return mCoreNumber;
	}
private:
	HANDLE* mThreads; //�X���b�h�n���h���z��
	int mThreadNumber;
	bool mEndRequest;
	Semaphore mQueueSemaphore;
	Mutex mLock;
	list< Entry > mQueue; //�v���҂��s��
	int mCoreNumber;
};
extern ManagerImpl* gManagerImpl;

} //namespace Threading
} //namespace GameLib
 
#endif
