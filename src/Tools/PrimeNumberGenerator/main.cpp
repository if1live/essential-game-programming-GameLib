#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <list>
using namespace std;

int main( int argc, char** argv ){
	string outFileName;
	int m = 0;
	bool help = false;
	//�R�}���h���C������
	for ( int i = 1; i < argc; ++i ){
		if ( argv[ i ][ 0 ] == '-' ){ //-����͂��܂�΂Ȃɂ��̃I�v�V����
			char c = argv[ i ][ 1 ];
			if ( c == 'o' && ( ( i + 1 ) < argc ) ){ 
				outFileName = argv[ i + 1 ];
			}else if ( c == 'h' ){ //�w���v
				help = true;
			}
		}else{
			istringstream iss( argv[ i ] );
			iss >> m;
			if ( m < 2 ){
				cerr << "MAX_NUMBER must be >= 2.";
				help = true;
			}
		}
	}
	if ( m == 0 ){
		help = true;
	}
	//�w���v�\�����ďI���
	if ( help ){
		cout << "Usage: PrimeNumberGenerator.exe [ -h ][ -o outputFile ] MAX_NUMBER" << endl;
		cout << "<Arguments>" << endl;
		cout << "\t-h             : display this help." << endl;
		cout << "\t-o             : specify output file." << endl;
		cout << "\tMAX_NUMBER     : specify maximum number. must be >= 2" << endl;
#ifndef NDEBUG //�������Ă����ƃf�o�O�̎��������Ȃ�
	while( true ){;} //visual studio������s���鎞�ɃR���\�[����ǂ��݂����̂ŁB
#endif
		return 1;
	}
	ostream* out;
	if ( outFileName.size() > 0 ){
		setlocale( LC_ALL, "" ); //���ꂪ�Ȃ��Ɠ��{��t�@�C�������󂯕t���Ȃ�
		out = new ofstream( outFileName.c_str() );
	}else{
		out = &cout;
	}
	//�f������
	//1.2�ƁAmax�ȉ��̊��S�����X�g�ɓ����
	list< int > l;
	l.push_back( 2 );
	for ( int i = 3; i <= m; i += 2 ){
		l.push_back( i );
	}
	//2.���[�gmaximum�𐶐�
	int n = static_cast< int >( sqrt( static_cast< double >( m ) ) );
	//3�ӂ邢����
	for ( int i = 3; i < n; ++i ){
		list< int >::iterator j = l.begin();
		while ( j != l.end() ){
			list< int >::iterator next = j;
			++next;
			if ( *j != i ){
				if ( ( *j % i ) == 0 ){ //���ꂿ�Ⴄ
					l.erase( j );
				}
			}
			j = next;
		}
	}
	//4�o��
	*out << "const int gPrimeNumbers[] = {" << endl;
	list< int >::iterator i = l.begin();
	int j = 0;
	while ( i != l.end() ){
		if ( ( j % 8 ) == 0 ){
			*out << "\t";
		}
		*out << *i << ",\t";
		if ( ( j % 8 ) == 7 ){
			*out << endl;
		}
		++i;
		++j;
	}
	if ( ( j % 8 ) != 7 ){
		*out << endl;
	}
	*out << "};" << endl;
	*out << "const int gPrimeNumberTableSize = " << l.size() << ";" << endl;
	*out << "const int gPrimeNumberTableMaximum = " << m << ";" << endl;
#ifndef NDEBUG //�������Ă����ƃf�o�O�̎��������Ȃ�
	if ( outFileName.size() == 0 ){
		while( true ){;} //visual studio������s���鎞�ɃR���\�[����ǂ��݂����̂ŁB
	}
#endif
	return 0;
}
