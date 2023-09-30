#include "XFile/Document.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "conversion.h"
#include "GameLib/GameLib.h"
using namespace std;
using namespace GameLib;
using namespace GameLib::PseudoXml;

bool convert( string* dst, const char* src, int size );

int main( int argc, char** argv ){
	bool verbose = false;
	bool help = false;
	vector< string > inFileNames;

	//�R�}���h���C������
	for ( int i = 1; i < argc; ++i ){
		if ( argv[ i ][ 0 ] == '-' ){ //-����͂��܂�΂Ȃɂ��̃I�v�V����
			char c = argv[ i ][ 1 ];
			if ( c == 'v' ){ //�`�ヂ�[�h
				verbose = true;
			}else if ( c == 'h' ){ //�w���v
				help = true;
			}
		}else{
			inFileNames.push_back( argv[ i ] );
		}
	}
	//�t�@�C������˂���
	if ( inFileNames.size() == 0 ){
		help = true;
	}
	//�w���v�\�����ďI���
	if ( help ){
		std::cout << "Usage: XFileConverter.exe [ -v ][ -h ] inputFiles..." << endl;
		std::cout << "<Options>" << endl;
		std::cout << "\t-v             : verbose mode." << endl;
		std::cout << "\t-h             : display this help." << endl;
#ifndef NDEBUG //�������Ă����ƃf�o�O�̎��������Ȃ�
	while( true ){;} //visual studio������s���鎞�ɃR���\�[����ǂ��݂����̂ŁB
#endif
		return 1;
	}
	//�����n�߂��[�B
	for ( unsigned i = 0; i < inFileNames.size(); ++i ){
		setlocale( LC_ALL, "" ); //���ꂪ�Ȃ��Ɠ��{��t�@�C�������󂯕t���Ȃ�
		ifstream in( inFileNames[ i ].c_str(), ifstream::binary );
		in.seekg( 0, ifstream::end );
		int size = static_cast< int >( in.tellg() );
		in.seekg( 0, ifstream::beg );
		char* src = NEW char[ size + 1 ];
		in.read( src, size );
		src[ size ] = '\0'; //NULL�I�[
		string dst;
		if ( !convert( &dst, src, size ) ){
			std::cout << "Conversion Failed : " << inFileNames[ i ] << endl;
		}else{
			string outName = inFileNames[ i ] + ".txt";
			setlocale( LC_ALL, "" ); //���ꂪ�Ȃ��Ɠ��{��t�@�C�������󂯕t���Ȃ�
			ofstream out( outName.c_str(), ofstream::binary );
			out.write( dst.c_str(), static_cast< streamsize >( dst.size() ) );
			if ( out ){
				std::cout << "Conversion Succeeded : " << inFileNames[ i ] << endl;
			}else{
				std::cout << "Output Failed : " << inFileNames[ i ] << endl;
			}
		}
	}
#ifndef NDEBUG //�������Ă����ƃf�o�O�̎��������Ȃ�
	while( true ){;} //visual studio������s���鎞�ɃR���\�[����ǂ��݂����̂ŁB
#endif
	return 0;
}

#include <ctime>
bool convert( string* dst, const char* src, int size ){
	//X�t�@�C���f�[�^�\�z
	XFile::Document xFile( src, size );
	if ( !xFile.isValid() ){
		return false;
	}
	string xml;
	xFile.convertToString( &xml );
	//�j�Zxml�Ƃ��ēǂݍ���
	Document srcDoc = Document::create( xml.c_str(), static_cast< int >( xml.size() ) );
	Document dstDoc = convert( srcDoc );
	
	dstDoc.convertToString( dst );
	return true;
}

#include "GameLib/Framework.h"
namespace GameLib{
	void Framework::configure( Configuration* ){}
	void Framework::update(){}
}


