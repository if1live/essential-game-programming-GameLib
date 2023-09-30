#ifndef INCLUDED_LOADER_H
#define INCLUDED_LOADDR_H

#include <list>
#include <string>
#include <fstream>
#include "GameLib/Threading/Thread.h"
#include "GameLib/Threading/Mutex.h"
using namespace std;

class File{
public:
	bool isReady() const; //���[�h�I����Ă�H
	int getSize() const; //�t�@�C���T�C�Y�擾
	const char* getData() const; //���g�擾
private:
	File( const char* filename );
	~File();
	friend class Loader;

	string mFilename;
	char* mData;
	int mSize;
};

//Loader::update���ĂԂ����̐l
class LoadingThread : public GameLib::Threading::Thread{
public:
	LoadingThread(){ start(); }
	~LoadingThread(){ wait(); }
	void operator()();
};

class Loader{
public:
	static Loader* instance();
	static void create();
	static void destroy();

	void createFile( File**, const char* filename );
	void destroyFile( File** );
	void update( bool* endOut ); //�I��鎞��endOut��true������
private:
	Loader();
	Loader( const Loader& ); //����
	~Loader();

	//�X���b�h�̓s���ɂ��z��ɂ���B
	static const int MAX_FILE_NUMBER = 100;
	File* mFiles[ MAX_FILE_NUMBER ];
	
	LoadingThread* mThread; //�X���b�h
	GameLib::Threading::Mutex mLock;
	bool mEndRequest; //�I���Ƃ���true�ɂ���B
	static const int READ_UNIT = 1 * 1024 * 1024; //1MB

	static Loader* mInstance;
};

#endif
