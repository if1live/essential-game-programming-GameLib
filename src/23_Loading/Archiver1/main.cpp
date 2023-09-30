#include <windows.h>
#include <string>
#include <list>
#include <fstream>
#include <iostream>
using namespace std;
/*
���̃T���v���ł�windows�̊֐���ASCII��(���ʂ�char*�����o�[�W����)
�𒼐ڌĂяo���Ďg���B�{����TCHAR�Ȃǂ̃}�N���𑽗p����Unicode��ASCII��
�ӎ������Ɏg���̂���@�Ȃ̂����A������Ȃ��R�[�h�͓ǂ݂ɂ������A
�ʂ�windows�v���O���~���O���w�Ԃ��Ƃ͖ړI�ł͂Ȃ��B
����perl��bash�AC#�Ȃǂ��o����΂��̎�̃c�[����C++�ŏ������Ƃ��Ȃ��Ȃ邾�낤�B
�����ł�windows��@�𖳎�����ASCII�ł𒼐ڌĂԁB

<�g���֐�>
GetFirstFileA
GetNextFileA
<�g���\����>
WIN32_FIND_DATAA
*/

//�t�@�C���ꗗ���擾
void enumerateFiles( 
list< string >& fileNameListOut, 
const string& directoryName );

//int�������݂֗̕��֐�
void write( ofstream* o, int a ){
   char str[ 4 ];
   str[ 0 ] = static_cast< char >( ( a & 0x000000ff ) >>  0 );
   str[ 1 ] = static_cast< char >( ( a & 0x0000ff00 ) >>  8 );
   str[ 2 ] = static_cast< char >( ( a & 0x00ff0000 ) >> 16 );
   str[ 3 ] = static_cast< char >( ( a & 0xff000000 ) >> 24 );
   o->write( str, 4 );
}


//�A�[�J�C�u�����
void createArchive( 
const string* fileNames, 
int fileNumber, 
const char* archiveName );

//���������o�̓t�@�C����
int main( int, char** argv ){
	//�悭�g�����̂ɕʖ���
	typedef list< string > List;
	typedef List::iterator It;

	//�A�[�J�C�u���͑���������
	string archiveName = argv[ 1 ];
	archiveName += ".bin"; //�o�C�i���t�@�C���ŏ���ɍ�������̂͂Ƃ肠����bin������̊g���q��t���邱�Ƃ������B

	List fileNameList;
	//�t�@�C�����X�g�����
	enumerateFiles( fileNameList, argv[ 1 ] );
	//�g���ɂ����̂Ŕz��ɕϊ�
	int fileNumber = static_cast< int >( fileNameList.size() );
	string* fileNames = new string[ fileNumber ];
	int j = 0;
	for ( It i = fileNameList.begin(); i != fileNameList.end(); ++i ){
		fileNames[ j ] = *i;
		++j;
	}

	//�A�[�J�C�u����
	createArchive( fileNames, fileNumber, archiveName.c_str() );

#ifndef NDEBUG //�������Ă����ƃf�o�O�̎��������Ȃ�
	while( true ){;} //visual studio������s���鎞�ɃR���\�[����ǂ��݂����̂ŁB
#endif
	return 0;
}

//------------------------�֐�----------------------------

void enumerateFiles( 
list< string >& fileNameList, 
const string& directoryName ){
	HANDLE iterator; //�C�e���[�^�݂����Ȃ��̂Ȃ̂�iterator�Ɩ���t���Ă݂�B
	WIN32_FIND_DATAA fileData; //�����Ƀt�@�C�������������B
	//�ŏ��̃t�@�C���擾
	string searchPath = directoryName;
	searchPath += "\\*.*"; //\�͓���ȕ����Ȃ̂œ�d�˂�B��������Ƃ��傤�ǈ����B
	//���t�@�C���B�f�B���N�g���͌�ŁB
	iterator = FindFirstFileA( searchPath.c_str(), &fileData );
	while ( true ){ //�Ƃ肠���������Ȃ��B���[�v�̌��Ŕ�����
		const char* name = fileData.cFileName;
		if ( !( fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ){ //�f�B���N�g���͖���
			//�t�@�C�����ɂ̓f�B���N�g�������݂ɂ��Ă��K�v������
			string filename = directoryName;
			filename += '\\';
			filename += name; 
			cout << "\tFile : " << filename << endl; //�\�����Ă�낤�B�f�o�O�ɂ��Ȃ邵�B
			fileNameList.push_back( filename ); //���X�g�ɒǉ�
		}
		if ( !FindNextFileA( iterator, &fileData ) ){ //���ցB�����Ȃ���Δ�����Biterator���ύX���ꂻ���ɂȂ����AHANDLE�^�̓|�C���^�Ȃ̂ŁA����ő��v�B
			break;
		}
	}
	//���x�̓f�B���N�g������
	iterator = FindFirstFileA( searchPath.c_str(), &fileData );
	while ( true ){ //�Ƃ肠���������Ȃ��B���[�v�̌��Ŕ�����
		string name = fileData.cFileName;
		//���������Ƃ�.�Ƃ�..�Ƃ����Ԃ��̂ŁA������͏��O�B�łȂ��Ɩ������[�v���Ă��܂��B
		if ( name == "." || name == ".." ){
			; //�������Ȃ�
		}else if ( fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ){ //�f�B���N�g���ł��B
			//�f�B���N�g�������p�������Ă��B
			string newDirectoryName = directoryName;
			newDirectoryName += '\\';
			newDirectoryName += name;
			cout << "Directory : " << newDirectoryName << endl; //�\�����Ă�낤�B�f�o�O�ɂ��Ȃ邵�B
			enumerateFiles( fileNameList, newDirectoryName.c_str() ); //�ċA�Ăяo��
		} //�f�B���N�g���łȂ���Ή������Ȃ�
		if ( !FindNextFileA( iterator, &fileData ) ){ //���ցB�����Ȃ���Δ�����Biterator���ύX���ꂻ���ɂȂ����AHANDLE�^�̓|�C���^�Ȃ̂ŁA����ő��v�B
			break;
		}
	}
}

void createArchive( const string* fileNames, int fileNumber, const char* archiveName ){
	//�������ݐ���J����
	ofstream out( archiveName, ofstream::binary );
	//�t�@�C���T�C�Y�z����m��
	int* fileSizes = new int[ fileNumber ];

	//���X�gfileNames�����ĊJ���Ă͓ǂݏo���ăR�s�[
	for ( int i = 0; i < fileNumber; ++i ){
		ifstream in( fileNames[ i ].c_str(), ifstream::binary );
		in.seekg( 0, ifstream::end );
		fileSizes[ i ] = in.tellg();
		in.seekg( 0, ifstream::beg );
		char* data = new char[ fileSizes[ i ] ];
		in.read( data, fileSizes[ i ] ); //�ǂ�
		out.write( data, fileSizes[ i ] ); //����
	}
	//�t�@�C���̖����̈ʒu���L�^
	int dataEnd = out.tellp(); //tellg��get�̈ʒu�ŁAtellp��put�̈ʒu��
	//�܂��t�@�C���̐�����������ł�낤
	write( &out, fileNumber );
	//�\����邽�߂ɃT�C�Y���I�t�Z�b�g�ɒ����Ȃ���A
	//�ꏊ�A�T�C�Y�A���O�̕������A���O���i�[���Ă����B
	int pos = 0;
	for ( int i = 0; i < fileNumber; ++i ){
		write( &out, pos );
		write( &out, fileSizes[ i ] );
		string name = fileNames[ i ].c_str();
		int nameLength = static_cast< int >( name.size() );
		write( &out, nameLength );
		out.write( name.c_str(), nameLength );
		cout << pos << '\t' << fileSizes[ i ] << '\t' << nameLength << '\t' << name << endl; //�f�o�O�p�ɕ\�����Ă�낤
		pos += fileSizes[ i ];
	}
	//�Ō�ɕ\�̊J�n�ʒu����������ł��B
	write( &out, dataEnd );
	//��n��
	delete[] fileSizes; //SAFE_DELETE_ARRAY�͂Ȃ��̂ŁB���̃��C�u��������Ȃ�����B
	fileSizes = 0;
}
