#include <algorithm> //random_shuffle()�̂��߂�
#include <iostream>
#include <ctime> //clock()�̂��߂�
using namespace std;

int main( int argc, char** ){
	cout << "PREPARING... " << endl;

	const int N = 1000 * 1000 * 20; //10MB��
	int* a = new int[ N ]; 
	int** p0 = new int*[ N ];
	int** p1 = new int*[ N ];
	for ( int i = 0; i < N; ++i ){
		a[ i ] = i;
		p0[ i ] = &a[ i ];
		p1[ i ] = &a[ i ];
	}
	//p1���o���o����
	random_shuffle( p1, p1 + N );
	//argc�����Ăǂ����̃|�C���^�z����g�����I��
	int** p = ( argc == 1 ) ? p0 : p1;

	double t0 = clock();	//���Ԏn�_
	//100��A�N�Z�X
	for ( int j = 0; j < 100; ++j ){
		for ( int i = 0; i < N; ++i ){
			++( *p[ i ] );
		}
		cout << j << endl;
	}
	double t1 = clock(); //���ԏI�_

	cout << "TIME: " << ( t1 - t0 ) / CLOCKS_PER_SEC << endl; //����ŕb�ɂȂ�

	while( true ); //�������[�v

	return 0;
}
