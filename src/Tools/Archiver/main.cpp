#include <windows.h>
#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cassert>
using namespace std;
/*
���̃T���v���ł�windows�̊֐���ASCII��(���ʂ�char*�����o�[�W����)
�𒼐ڌĂяo���Ďg���B�{����TCHAR�Ȃǂ̃}�N���𑽗p����Unicode��ASCII��
�ӎ������Ɏg���̂���@�Ȃ̂����A������Ȃ��R�[�h�͓ǂ݂ɂ������A
�ʂ�windows�v���O���~���O���w�Ԃ��Ƃ͖ړI�ł͂Ȃ��B
����perl��bash�AC#�Ȃǂ��o����΂��̎�̃c�[����C++�ŏ������Ƃ��Ȃ��Ȃ邾�낤�B
�����ł�windows��@�𖳎�����ASCII�ł𒼐ڌĂ�

<�g���֐�>
GetFirstFileA
GetNextFileA
<�g���\����>
WIN32_FIND_DATAA
*/

//�t�@�C���ꗗ���擾
void enumerateFiles( 
	list< string >& fileNameListOut, 
	const string& directoryName,
	bool verbose );

//�A�[�J�C�u�����
bool createArchive( 
	const string* fileNames, 
	int fileNumber, 
	const char* archiveName,
	int blockSize,
	bool verbose,
	double compressionThreshold );

//���k�{��
bool compress( 
	int* outSize, 
	int* inPlaceDecompressionBufferSize,
	bool* compressed,
	ofstream* oStream, 
	ifstream* iStream,
	double threshold );

//int�������݂֗̕��֐�
void write( ofstream* o, unsigned a ){
   char str[ 4 ];
   str[ 0 ] = static_cast< char >( ( a & 0x000000ff ) >>  0 );
   str[ 1 ] = static_cast< char >( ( a & 0x0000ff00 ) >>  8 );
   str[ 2 ] = static_cast< char >( ( a & 0x00ff0000 ) >> 16 );
   str[ 3 ] = static_cast< char >( ( a & 0xff000000 ) >> 24 );
   o->write( str, 4 );
}

#include "GameLib/GameLib.h"

//���������o�̓t�@�C����
int main( int argc, char** argv ){
	int blockSize = 4;
	string archiveDir;
	string archiveName;
	bool verbose = false;
	bool help = false;
	double threshold = 100.0;
	//�R�}���h���C������
	for ( int i = 1; i < argc; ++i ){
		if ( argv[ i ][ 0 ] == '-' ){ //-����͂��܂�΂Ȃɂ��̃I�v�V����
			char c = argv[ i ][ 1 ];
			if ( c == 'b' ){ //�T�C�Y�w��
				if ( ( i + 1 ) < argc ){
					istringstream iss( argv[ i + 1 ] );
					iss >> blockSize;
				}
				++i;
			}else if ( c == 'o' ){ //�o�̓t�@�C�����w��
				if ( ( i + 1 ) < argc ){
					archiveName = argv[ i + 1 ];
				}
				++i;
			}else if ( c == 'r' ){ //臒l
				if ( ( i + 1 ) < argc ){
					istringstream iss( argv[ i + 1 ] );
					iss >> threshold;
					if ( threshold > 100.0 ){
						threshold = 100.0; //100�ȏ�͋����Ȃ�
					}
				}
				++i;
			}else if ( c == 'v' ){ //�`�ヂ�[�h
				verbose = true;
			}else if ( c == 'h' ){ //�w���v
				help = true;
			}
		}else{
			archiveDir = argv[ i ];
		}
	}
	//�A�[�J�C�u�f�B���N�g���w�肪�Ȃ��ƑʖځB
	if ( archiveDir.size() == 0 ){
		help = true;
	}
	//�w���v�\�����ďI���
	if ( help ){
		cout << "Usage: Archiver.exe rootDir [ -o outfilename ][ -s blockSize ][ -r thresholdRatio ][ -v ][ -h ]" << endl;
		cout << "<Options>" << endl;
		cout << "\t-o outfilename : specify output archive name. default is \"rootDir\".bin" << endl;
		cout << "\t-s blocksize   : specify block size. \"4GB * blockSize\" is maximum archivesize. default is 4." << endl;
		cout << "\t-r percent     : specify threshold. if compressed size is less than this, non-compressed data is written." << endl;
		cout << "\t-v             : verbose mode." << endl;
		cout << "\t-h             : display this help." << endl;
#ifndef NDEBUG //�������Ă����ƃf�o�O�̎��������Ȃ�
	while( true ){;} //visual studio������s���鎞�ɃR���\�[����ǂ��݂����̂ŁB
#endif
		return 1;
	}
	//�A�[�J�C�u�����Ȃ���΃A�[�J�C�u�f�B���N�g����������Ă��
	if ( archiveName.size() == 0 ){
		archiveName = archiveDir;
		archiveName += ".bin";
	}

	//�悭�g�����̂ɕʖ���
	typedef list< string > List;
	typedef List::iterator It;
	List fileNameList;
	//�t�@�C�����X�g�����
	enumerateFiles( fileNameList, archiveDir.c_str(), verbose );
	//�g���ɂ����̂Ŕz��ɕϊ�
	int fileNumber = static_cast< int >( fileNameList.size() );
	string* fileNames = new string[ fileNumber ];
	int j = 0;
	for ( It i = fileNameList.begin(); i != fileNameList.end(); ++i ){
		fileNames[ j ] = *i;
		++j;
	}

	//�A�[�J�C�u����
	bool succeeded = createArchive( 
		fileNames, 
		fileNumber, 
		archiveName.c_str(), 
		blockSize, 
		verbose, 
		threshold );
	if ( !succeeded ){ //���s����������͂��Ȃ��B�������������񂾂낤���B
		cerr << "creating failed. delete output file manually." << endl;
#ifndef NDEBUG //�������Ă����ƃf�o�O�̎��������Ȃ�
	while( true ){;} //visual studio������s���鎞�ɃR���\�[����ǂ��݂����̂ŁB
#endif
		return 1;
	}

#ifndef NDEBUG //�������Ă����ƃf�o�O�̎��������Ȃ�
	while( true ){;} //visual studio������s���鎞�ɃR���\�[����ǂ��݂����̂ŁB
#endif
	return 0;
}

//------------------------�֐�----------------------------

void enumerateFiles( 
list< string >& fileNameList, 
const string& directoryName,
bool verbose ){
	HANDLE iterator; //�C�e���[�^�݂����Ȃ��̂Ȃ̂�iterator�Ɩ���t���Ă݂�B
	WIN32_FIND_DATAA fileData; //�����Ƀt�@�C�������������B
	//�ŏ��̃t�@�C���擾
	string searchPath = directoryName;
	searchPath += "\\*.*"; //\�͓���ȕ����Ȃ̂œ�d�˂�B��������Ƃ��傤�ǈ����B

	//�t�@�C������
	iterator = FindFirstFileA( searchPath.c_str(), &fileData );
	if ( iterator == INVALID_HANDLE_VALUE ){
		//�������s�����B
		return;
	}
	while ( true ){ //�Ƃ肠���������Ȃ��B���[�v�̌��Ŕ�����
		if ( !( fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) ){
			//�t�@�C�����ɂ̓f�B���N�g�������݂ɂ��Ă��K�v������
			string filename = directoryName;
			filename += '\\';
			filename += fileData.cFileName; 
			if ( verbose ){
				cout << "\tFile : " << filename << endl; //�\�����Ă�낤�B�f�o�O�ɂ��Ȃ邵�B
			}
			fileNameList.push_back( filename ); //���X�g�ɒǉ�
		}
		if ( !FindNextFileA( iterator, &fileData ) ){ //���ցB�����Ȃ���Δ�����Biterator���ύX���ꂻ���ɂȂ����AHANDLE�^�̓|�C���^�Ȃ̂ŁA����ő��v�B
			break;
		}
	}
	//�f�B���N�g������
	iterator = FindFirstFileA( searchPath.c_str(), &fileData );
	if ( iterator == INVALID_HANDLE_VALUE ){
		//�������s�����B
		return;
	}
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
			if ( verbose ){
				cout << "Directory : " << newDirectoryName << endl;
			}
			enumerateFiles( fileNameList, newDirectoryName.c_str(), verbose ); //�ċA�Ăяo��
		}
		if ( !FindNextFileA( iterator, &fileData ) ){ //���ցB�����Ȃ���Δ�����Biterator���ύX���ꂻ���ɂȂ����AHANDLE�^�̓|�C���^�Ȃ̂ŁA����ő��v�B
			break;
		}
	}
}

bool createArchive( 
const string* fileNames, 
int fileNumber, 
const char* archiveName, 
int blockSize, 
bool verbose,
double threshold ){
	//�������ݐ���J����
	ofstream out( archiveName, ofstream::binary );
	if ( !out ){
		cerr << "can't create output file." << endl;
		return false;
	}
	//�t�@�C���T�C�Y�z����m��
	int* originalSizes = new int[ fileNumber ];
	int* outputSizes = new int[ fileNumber ];
	int* inPlaceDecompressionBufferSizes = new int[ fileNumber ];

	//���X�gfileNames�����ĊJ���Ă͓ǂݏo���ăR�s�[
	for ( int i = 0; i < fileNumber; ++i ){
		setlocale( LC_ALL, "" ); //���ꂪ�Ȃ��Ɠ��{��t�@�C�������󂯕t���Ȃ�
		ifstream in( fileNames[ i ].c_str(), ifstream::binary );
		if ( !in ){
			cerr << "can't open file : " << fileNames[ i ] << endl;
			return false;
		}
		in.seekg( 0, ifstream::end );
		streamsize sizeBig = in.tellg();
		if ( sizeBig >= 0x7fffffff ){
			cerr << "can't handle files larger than 2GB!" << endl;
			delete[] outputSizes;
			outputSizes = 0;
			delete[] originalSizes;
			originalSizes = 0;
			out.close();
			out.open( archiveName ); //�J���Ȃ������Œ��g�����
			return false;
		}
		in.seekg( 0, ifstream::beg );
		originalSizes[ i ] = static_cast< int >( sizeBig );
		bool compressed;
		compress( &outputSizes[ i ], &inPlaceDecompressionBufferSizes[ i ], &compressed, &out, &in, threshold );
		if ( verbose ){
			double ratio = ( sizeBig == 0 ) ? 100.0 : outputSizes[ i ] * 100.0 / sizeBig;
			cout << fileNames[ i ] << " ) : " << sizeBig << " -> " << outputSizes[ i ] << " ( " << ratio << "% )" << endl;
		}
		//�u���b�N�T�C�Y���܂�Ƀ_�~�[����������
		int mod = outputSizes[ i ] % blockSize;
		int padding = ( mod > 0 ) ? ( blockSize - mod ) : 0;
		for ( int j = 0; j < padding; ++j ){
			out.put( '\0' );
		}
	}
	if ( verbose ){
		cout << "<Table>" << endl;
		cout << "FileNumber : " << fileNumber << endl;
		cout << "BlockSize : " << blockSize << endl;
	}
	//�t�@�C���̖����̈ʒu���L�^
	streamsize dataEnd = out.tellp(); //tellg��get�̈ʒu�ŁAtellp��put�̈ʒu��
	//�܂��t�@�C���̐�����������ł�낤
	out.write( reinterpret_cast< char* >( &fileNumber ), 4 );
	//�\����邽�߂ɃT�C�Y���I�t�Z�b�g�ɒ����Ȃ���A
	//�ꏊ�A�T�C�Y�A���O�̕������A���O���i�[���Ă����B
	unsigned pos = 0; //�u���b�N�P�ʂ̈ʒu
	for ( int i = 0; i < fileNumber; ++i ){
		write( &out, pos );
		write( &out, outputSizes[ i ] );
		write( &out, originalSizes[ i ] );
		write( &out, inPlaceDecompressionBufferSizes[ i ] );
		string name = fileNames[ i ].c_str();
		int nameLength = static_cast< int >( name.size() );
		write( &out, nameLength );
		out.write( name.c_str(), nameLength );
		if ( verbose ){
			cout << pos << '\t' << outputSizes[ i ] << '\t' << originalSizes[ i ] << '\t' << inPlaceDecompressionBufferSizes[ i ] << '\t' << nameLength << '\t' << name << endl; //�f�o�O�p�ɕ\�����Ă�낤
		}
		pos += ( outputSizes[ i ] + blockSize - 1 ) / blockSize;
	}
	//�Ō�ɏ������ނ̂��u���b�N�T�C�Y
	write( &out, blockSize );
	//�\�̊J�n�ʒu����������ł��B
	assert( dataEnd % blockSize == 0 );
	unsigned dataEndInBlock = static_cast< unsigned >( dataEnd / blockSize );
	write( &out, dataEndInBlock );
	//��n��
	delete[] outputSizes;
	outputSizes = 0;
	delete[] originalSizes;
	originalSizes = 0;
	delete[] inPlaceDecompressionBufferSizes;
	inPlaceDecompressionBufferSizes = 0;

	return true;
}
