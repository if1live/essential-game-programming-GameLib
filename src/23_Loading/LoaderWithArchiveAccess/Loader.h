#ifndef INCLUDED_LOADER_H
#define INCLUDED_LOADDR_H

#include <list>
#include <string>
#include <map>
#include <fstream>
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
	friend class Archive;

	string mFilename;
	char* mData;
	int mSize;
};

//�A�[�J�C�u�N���X
class Archive{
public:
	Archive( const char* archiveName );
	~Archive();
	void read( File* ) ;
private:
	struct Entry{
		int mPosition;
		int mSize;
	};
	int mFileNumber;
	ifstream* mStream;
	map< string, Entry > mEntries;
};

class Loader{
public:
	static Loader* instance();
	//�A�[�J�C�u����ǂނȂ�A�[�J�C�u����n��
	static void create( const char* archiveName = 0 );
	static void destroy();

	void createFile( File**, const char* filename );
	void destroyFile( File** );
	void update();
private:
	Loader( const char* filename );
	Loader( const Loader& ); //����
	~Loader();

	list< File* > mFiles;
	Archive* mArchive;

	static Loader* mInstance;
};

#endif
