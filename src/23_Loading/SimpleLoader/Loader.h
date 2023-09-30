#ifndef INCLUDED_LOADER_H
#define INCLUDED_LOADDR_H

#include <list>
#include <string>
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

class Loader{
public:
	static Loader* instance();
	static void create();
	static void destroy();

	void createFile( File**, const char* filename );
	void destroyFile( File** );
	void update();
private:
	Loader();
	Loader( const Loader& ); //����
	~Loader();

	list< File* > mFiles;
	static Loader* mInstance;
};

#endif
