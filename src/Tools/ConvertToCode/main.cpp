#include <fstream>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

void displayHelp();
void write( bool* succeeded, ofstream&, const char* inFileName );

int main( int argc, const char** argv ){
	vector< const char* > inFiles;
	const char* outFile = 0;
	//�R�}���h���C������
	for ( int i = 1; i < argc; ++i ){
		const char* arg = argv[ i ];
		if ( arg[ 0 ] == '-' ){
			if ( arg[ 1 ] == 'o' ){
				if ( i + 1 < argc ){
					outFile = argv[ i + 1 ];
					++i;
				}
			}else if ( arg[ 1 ] == 'h' ){
				displayHelp();
				return 0;
			}
		}else{
			inFiles.push_back( arg );
		}
	}
	if ( inFiles.size() == 0 || !outFile ){
		displayHelp();
		return 0;
	}
	//�o�̓t�@�C���J����
	setlocale( LC_ALL, "" ); //���ꂪ�Ȃ��Ɠ��{��t�@�C�������󂯕t���Ȃ�
	ofstream o( outFile, ofstream::binary );
	if ( !o ){
		cerr << "can't open output file." << endl;
		return 1;
	}
	//���̓t�@�C���J���Ȃ���f���o�������{��
	bool succeeded;
	for ( unsigned i = 0; i < inFiles.size(); ++i ){
		write( &succeeded, o, inFiles[ i ] );
		if ( !succeeded ){
			cerr << "can't open/read input file." << endl;
			cerr << inFiles[ i ] << endl;
			return 2;
		}
	}
}

void write( bool* succeeded, ofstream& o, const char* inFileName ){
	setlocale( LC_ALL, "" ); //���ꂪ�Ȃ��Ɠ��{��t�@�C�������󂯕t���Ȃ�
	ifstream in( inFileName, ifstream::binary );
	if ( !in ){
		*succeeded = false;
		return;
	}
	//��C�ǂ�
	in.seekg( 0, ifstream::end );
	streamsize s = in.tellg();
	in.seekg( 0, ifstream::beg );
	char* b = new char[ s ];
	in.read( b, s );
	streamsize readSize = in.gcount();
	if ( readSize != s ){
		*succeeded = false;
		return;
	}
	//�ϐ����̓t�@�C�������琶���B
	//�X���b�V���A�o�b�N�X���b�V���A�s���I�h�A0x80�ȏ�͖������Ď��̕�����啶���ɕϊ�
	string name;
	bool capital = true;
	for ( unsigned i = 0; inFileName[ i ]; ++i ){
		if ( inFileName[ i ] == '/' ){
			capital = true;
		}else if ( inFileName[ i ] == '\\' ){
			capital = true;
		}else if ( inFileName[ i ] == '.' ){
			capital = true;
		}else if ( !isgraph( inFileName[ i ] ) ){
			capital = true;
		}else{
			char c = inFileName[ i ];
			if ( capital ){
				c = toupper( c );
				capital = false;
			}
			name += c;
		}
	}
	//�w�b�_�����o���J�n
	o << "namespace {\n";
	o << "const char g" << name << "[] = {\\\n";
	//�������[�v
	int rowCount = 0;
	for ( streamsize i = 0; i < s; ++i ){
		o << ( int )b[ i ] << ", ";
		++rowCount;
		if ( rowCount == 16 ){
			o << '\\' << '\n';
			rowCount = 0;
		}
	}
	//�Ō�ɉ��s��t��
	o << '\\' << '\n';
	//���ʕ�
	o << "};\n";
	o << "} //anonymous namespace\n";
	*succeeded = true;
}

void displayHelp(){
	cout << "convert contents of files to C constant string" << endl;
	cout << "usage : ConvertToCode.exe -o OUTFILE INFIle..." << endl;
}
