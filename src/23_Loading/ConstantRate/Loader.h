#ifndef INCLUDED_LOADER_H
#define INCLUDED_LOADDR_H

#include <list>
#include <string>
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
	
	File* mCurrentFile; //���������Ă���File
	ifstream* mStream; //�ǂ�ł�t�@�C��
	int mFileSize; //���ݓǂݍ��ݒ��̃t�@�C���̃T�C�Y
	int mFilePosition; //���ݓǂݍ��ݒ��̃t�@�C���̏ꏊ
	char* mData; //�ǂݍ��ݒ��o�b�t�@
	static const int READ_UNIT = 128 * 1024; //128KB

	static Loader* mInstance;
};

#endif
