#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "GameLib/GameLib.h"
#include "GameLib/PseudoXml/Document.h"
using namespace std;
using namespace GameLib;
using namespace GameLib::PseudoXml;

int main( int argc, char** argv ){
	bool help = false;
	vector< string > inFileNames;

	//�R�}���h���C������
	for ( int i = 1; i < argc; ++i ){
		if ( argv[ i ][ 0 ] == '-' ){ //-����͂��܂�΂Ȃɂ��̃I�v�V����
			char c = argv[ i ][ 1 ];
			if ( c == 'h' ){ //�w���v
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
		Document d = Document::create( src, size );

		size_t pos = inFileNames[ i ].rfind( '.' );
		inFileNames[ i ].erase( pos );
		inFileNames[ i ] += ".bin";

		Array< char > dst;
		d.convertToBinary( &dst );

		setlocale( LC_ALL, "" ); //���ꂪ�Ȃ��Ɠ��{��t�@�C�������󂯕t���Ȃ�
		ofstream out( inFileNames[ i ].c_str(), ofstream::binary );
		out.write( &dst[ 0 ], dst.size() );
	}
#ifndef NDEBUG //�������Ă����ƃf�o�O�̎��������Ȃ�
	while( true ){;} //visual studio������s���鎞�ɃR���\�[����ǂ��݂����̂ŁB
#endif
	return 0;
}

#include "GameLib/Framework.h"
namespace GameLib{
	void Framework::configure( Configuration* ){}
	void Framework::update(){}
}
