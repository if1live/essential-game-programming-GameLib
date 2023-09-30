#include <iostream>
using namespace std;

int main(){
	const int n = 2500 * 10000;
	int* a = new int[ n ];
	//�K���Ƀ\�[�g���ꂽ�f�[�^�𐶐�
	for ( int i = 0; i < n; ++i ){
		a[ i ] = i * 5;
	}
	cout << "SEARCH BEGIN" << endl;

	int loopCount = 0;
	//�T���l�͂���Ȋ�����
	int search = 89012345;
	//�ŏ��̌����͈�
	int first = 0;
	int last = n - 1;
	while ( last > first ){ //1�ȏ゠�邤���͌J��Ԃ�
		int middle = ( first + last ) / 2; //�^�񒆂̓Y����
		cout << loopCount << "\t" << first << "-" << last << "\tmiddle=" << middle << "\trange=" << last-first << endl;
		++loopCount;
		int t = a[ middle ];
		if ( t < search ){ //�������B���ɂ���
			first = middle + 1; //�擪��u��
		}else if ( t > search ){ //�傫���B�O�ɂ���B
			last = middle - 1; //������u��
		}else{
			first = last = middle;
		}
	}
	//�����܂ł����first == last���B
	if ( a[ first ] == search ){
		cout << "Find! " << first << endl;
	}else{
		cout << "Not Find!" << endl;
	}
	delete[] a;

	//�������[�v
	while( true );



	return 0;
}