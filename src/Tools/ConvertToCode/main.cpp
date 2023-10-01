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
	//コマンドライン解釈
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
	//出力ファイル開ける
	setlocale( LC_ALL, "" ); //これがないと日本語ファイル名を受け付けない
	ofstream o( outFile, ofstream::binary );
	if ( !o ){
		cerr << "can't open output file." << endl;
		return 1;
	}
	//入力ファイル開けながら吐き出す処理本体
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
	setlocale( LC_ALL, "" ); //これがないと日本語ファイル名を受け付けない
	ifstream in( inFileName, ifstream::binary );
	if ( !in ){
		*succeeded = false;
		return;
	}
	//一気読み
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
	//変数名はファイル名から生成。
	//スラッシュ、バックスラッシュ、ピリオド、0x80以上は無視して次の文字を大文字に変換
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
	//ヘッダ書き出し開始
	o << "namespace {\n";
	o << "const char g" << name << "[] = {\\\n";
	//文字ループ
	int rowCount = 0;
	for ( streamsize i = 0; i < s; ++i ){
		o << ( int )b[ i ] << ", ";
		++rowCount;
		if ( rowCount == 16 ){
			o << '\\' << '\n';
			rowCount = 0;
		}
	}
	//最後に改行を付加
	o << '\\' << '\n';
	//括弧閉じ
	o << "};\n";
	o << "} //anonymous namespace\n";
	*succeeded = true;
}

void displayHelp(){
	cout << "convert contents of files to C constant string" << endl;
	cout << "usage : ConvertToCode.exe -o OUTFILE INFIle..." << endl;
}
