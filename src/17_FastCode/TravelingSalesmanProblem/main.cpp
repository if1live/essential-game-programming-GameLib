#include <iostream>
#include <cstdlib> //rand(),atoi()
#include <limits>
#include <ctime> //clock()
using namespace std;

//�ꏊ�\����
struct Position{
	double x;
	double y;
};

//�ċA�����{��
void search( 
int* minPath, 
int* path, 
int cn, 
int n, 
const Position* p, 
double currentLength, 
const Position& currentPos );

//�ŒZ����
double gMinLength = numeric_limits< double >::max(); //�ƂĂ��傫�Ȑ��ŏ�����

int main( int argc, char** argv ){
	if ( argc < 2 ){
		return 1; //�G���[
	}
	int n = atoi( argv[ 1 ] );

	//�Ƃ̏ꏊ��������
	Position* positions = new Position[ n ];
	for ( int i = 0; i < n; ++i ){
		positions[ i ].x = rand();
		positions[ i ].y = rand();
		cout << i << " : " << positions[ i ].x << "," << positions[ i ].y << endl;
	}
	//�ċA�Ăяo���J�n
	int* minPath = new int[ n ];
	int* path = new int[ n ];
	Position currentPos;
	currentPos.x = currentPos.y = 0.0; //�����ʒu
	double t0 = clock(); //�J�n����
	search( minPath, path, 0, n, positions, 0.0, currentPos );
	double t1 = clock(); //�I������

	cout << "Length : " << gMinLength << endl;
	for ( int i = 0; i < n; ++i ){
		cout << i << " " << minPath[ i ] << endl;
	}
	cout << "Calculation Time : " << ( t1 - t0 ) / CLOCKS_PER_SEC << endl;
	while(1);
	return 0;
}

void search( 
int* minPath, 
int* path, 
int cn, 
int n, 
const Position* p, 
double currentLength, 
const Position& currentPos ){
	for ( int i = 0; i < n; ++i ){
		//�s���������Ȃ��ꏊ���H
		bool found = false;
		for ( int j = 0; j < cn; ++j ){
			if ( path[ j ] == i ){
				found = true;
				break;
			}
		}
		if ( !found ){ //�s���������Ȃ��̂Ŏ��͂����֍s����
			path[ cn ] = i;
			double dx = currentPos.x - p[ i ].x;
			double dy = currentPos.y - p[ i ].y;
			double l = currentLength + sqrt( dx * dx + dy * dy );
			if ( cn == n - 1 ){ //�Ō�܂ŗ����̂ōċA�Ăяo�������O���[�o���Ɣ�r�B�I���B
				if ( l < gMinLength ){
					gMinLength = l;
					//�o�H���ۑ�
					for ( int j = 0; j < n; ++j ){
						minPath[ j ] = path[ j ];
					}
					//���݂̉���\��
					cout << l << " : ";
					for ( int j = 0; j < n; ++j ){
						cout << path[ j ] << ',';
					}
					cout << endl;
				}
			}else{
				search( minPath, path, cn + 1, n, p, l, p[ i ] );
			}
		}
	}
}
