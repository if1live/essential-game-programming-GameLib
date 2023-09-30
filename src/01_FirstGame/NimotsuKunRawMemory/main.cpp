#include <iostream> //���ꂾ�������ė~�����B
using namespace std;
/*

���̃T���v���ł͉���m�ȊO�ɕϐ�����؂���Ȃ��B
�R���p�C���������̐��E�ŉ����N�����Ă���̂����v���m���ė~�����B
�ϐ������R�ɍ��邱�Ƃ��ǂ�قǂ��肪�������Ƃ����������A
�܂��A���ꂪ�ǂ�����ĉ\�ɂȂ��Ă���̂�����������Ă݂悤�B

[�������̎g����]

0     : �֐��̈����Ɩ߂�l�Ɏg���B
1-17  : �֐��̒��ōD���Ɏg���Ă����̈�Ƃ���B�֐����ĂԂƉ󂳂��ƍl����B
18    : 8 //��
19    : 5 //����
20-59 : 8x5�̏�Ԕz��
60-99 : ������`���̃X�e�[�W�f�[�^
*/

char m[ 100 ]; //�������B100�o�C�g�����g��Ȃ�

enum Object{
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,

	OBJ_UNKNOWN,
};

//�֐��v���g�^�C�v
void initializeGlobalVariables();
void initialize();
void draw();
void update();
void checkClear();

int main(){
	//�O���[�o���ϐ�������
	initializeGlobalVariables();

	initialize(); //�X�e�[�W������
	//���C�����[�v
	while ( true ){
		//�܂��`��
		draw();
		//�N���A�`�F�b�N ���ʂ�m[ 0 ]�ɓ����Ă���
		checkClear();
		if ( m[ 0 ] == 1 ){
			break; //�N���A�`�F�b�N
		}
		//���͎擾
		cout << "a:left s:right w:up z:down. command?" << endl; //�������
		//���͂�m[ 0 ]�ɓ���悤
		cin >> m[ 0 ];
		//�X�V
		update();
	}
	//�j���̃��b�Z�[�W
	cout << "Congratulation! you win." << endl;

	//Visual Studio������s����l�̂��߂ɖ������[�v�B�R�}���h���C�������Ctrl-C�ŏI���Ă��������B
	while( true ){
		;
	} 
	return 0;
}

//---------------------�ȉ��֐���`------------------------------------------

//m[60]����n�܂镶��������߂��āAm[20]����n�܂��Ԕz����\�z����֐��B
void initialize(){
	m[ 0 ] = 0; //0�Ԃ�ǂݎ�葤�̓Y�����Ɏg����
	m[ 1 ] = 0; //1�Ԃ�����x���W
	m[ 2 ] = 0; //2�Ԃ�����y���W

	while ( m[ 60 + m[ 0 ] ] != '\0' ){ //NULL�����łȂ���
		//3�Ԃɕ�������
		switch ( m[ 60 + m[ 0 ] ] ){
			case '#': m[ 3 ] = OBJ_WALL; break;
			case ' ': m[ 3 ] = OBJ_SPACE; break;
			case 'o': m[ 3 ] = OBJ_BLOCK; break;
			case 'O': m[ 3 ] = OBJ_BLOCK_ON_GOAL; break;
			case '.': m[ 3 ] = OBJ_GOAL; break;
			case 'p': m[ 3 ] = OBJ_MAN; break;
			case 'P': m[ 3 ] = OBJ_MAN_ON_GOAL; break;
			case '\n': m[ 1 ] = 0; ++m[ 2 ]; m[ 3 ] = OBJ_UNKNOWN; break; //���s����(++y)
			default: m[ 3 ] = OBJ_UNKNOWN; break;
		}
		++m[ 0 ];
		if ( m[ 3 ] != OBJ_UNKNOWN ){ //�m��Ȃ������Ȃ疳������̂ł���if��������
			m[ 20 + m[ 2 ]*m[ 18 ] + m[ 1 ] ] = m[ 3 ]; //�������� m[18]�͕�
			++m[ 1 ]; //++x
		}
	}
}

void draw(){
	for ( m[ 0 ] = 0; m[ 0 ] < m[ 19 ]; ++m[ 0 ] ){
		for ( m[ 1 ] = 0; m[ 1 ] < m[ 18 ]; ++m[ 1 ] ){
			m[ 2 ] = m[ 20 + m[ 0 ]*m[ 18 ] + m[ 1 ] ];
			switch ( m[ 2 ] ){
				case OBJ_SPACE : cout << ' '; break;
				case OBJ_WALL : cout << '#'; break;
				case OBJ_GOAL : cout << '.'; break;
				case OBJ_BLOCK : cout << 'o'; break;
				case OBJ_BLOCK_ON_GOAL : cout << 'O'; break;
				case OBJ_MAN : cout << 'p'; break;
				case OBJ_MAN_ON_GOAL : cout << 'P'; break;
			}
		}
		cout << endl;
	}
}

void update(){
	//�ړ������ɕϊ�
	m[ 1 ] = 0; //dx
	m[ 2 ] = 0; //dy
	switch ( m[ 0 ] ){ //input��m[100]�ɓ���Ă���
		case 'a': m[ 1 ] = -1; break; //��
		case 's': m[ 1 ] = 1; break; //�E
		case 'w': m[ 2 ] = -1; break; //��BY�͉����v���X
		case 'z': m[ 2 ] = 1; break; //���B
	}
	//�l���W������
	for ( m[ 0 ] = 0; m[ 0 ] < m[ 18 ]*m[ 19 ]; ++m[ 0 ] ){
		if ( m[ 20 + m[ 0 ] ] == OBJ_MAN || m[ 20 + m[ 0 ] ] == OBJ_MAN_ON_GOAL ){
			break;
		}
	}
	m[ 3 ] = m[ 0 ] % m[ 18 ]; //x�͕��Ŋ��������܂�
	m[ 4 ] = m[ 0 ] / m[ 18 ]; //y�͕��Ŋ�������

	//�ړ�
	//�ړ�����W
	m[ 5 ] = m[ 3 ] + m[ 1 ]; //tx
	m[ 6 ] = m[ 4 ] + m[ 2 ]; //ty
	//���W�̍ő�ŏ��`�F�b�N�B�O��Ă���Εs����
	if ( m[ 5 ] < 0 || m[ 6 ] < 0 || m[ 5 ] >= m[ 18 ] || m[ 6 ] >= m[ 19 ] ){
		return;
	}
	//A.���̕������󔒂܂��̓S�[���B�l���ړ��B
	m[ 7 ] = 20 + m[ 4 ]*m[ 18 ] + m[ 3 ]; //�l�ʒu
	m[ 8 ] = 20 + m[ 6 ]*m[ 18 ] + m[ 5 ]; //�ړ��ڕW�ʒu
	if ( m[ m[ 8 ] ] == OBJ_SPACE || m[ m[ 8 ] ] == OBJ_GOAL ){ //�ړ���ɋ�Ԃ������
		m[ m[ 8 ] ] = ( m[ m[ 8 ] ] == OBJ_GOAL ) ? OBJ_MAN_ON_GOAL : OBJ_MAN; //�S�[���Ȃ�S�[����̐l��
		m[ m[ 7 ] ] = ( m[ m[ 7 ] ] == OBJ_MAN_ON_GOAL ) ? OBJ_GOAL : OBJ_SPACE; //���Ƃ��ƃS�[����Ȃ�S�[����
	//B.���̕��������B���̕����̎��̃}�X���󔒂܂��̓S�[���ł���Έړ��B
	}else if ( m[ m[ 8 ] ] == OBJ_BLOCK || m[ m[ 8 ] ] == OBJ_BLOCK_ON_GOAL ){
		//2�}�X�悪�͈͓����`�F�b�N
		m[ 9 ] = m[ 5 ] + m[ 1 ];
		m[ 10 ] = m[ 6 ] + m[ 2 ];
		if ( m[ 9 ] < 0 || m[ 10 ] < 0 || m[ 9 ] >= m[ 18 ] || m[ 10 ] >= m[ 19 ] ){ //�����Ȃ�
			return;
		}

		m[ 11 ] = 20 + ( m[ 6 ] + m[ 2 ] )*m[ 18 ] + ( m[ 5 ] + m[ 1 ] ); //2�}�X��
		if ( m[ m[ 11 ] ] == OBJ_SPACE || m[ m[ 11 ] ] == OBJ_GOAL ){
			//��������ւ�
			m[ m[ 11 ] ] = ( m[ m[ 11 ] ] == OBJ_GOAL ) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			m[ m[ 8 ] ] = ( m[ m[ 8 ] ] == OBJ_BLOCK_ON_GOAL ) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			m[ m[ 7 ] ] = ( m[ m[ 7 ] ] == OBJ_MAN_ON_GOAL ) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}

//�u���b�N�݂̂��Ȃ���΃N���A���Ă���B
void checkClear(){
	for ( m[ 1 ] = 20; m[ 1 ] < 20+m[ 18 ]*m[ 19 ]; ++m[ 1 ] ){
		if ( m[ m[ 1 ] ] == OBJ_BLOCK ){
			m[ 0 ] = 0; //�߂�l��m[ 0 ]
			return;
		}
	}
	m[ 0 ] = 1; //�߂�l��m[ 0 ]
	return;
}


//#�� _��� .�S�[�� o�u���b�N p�l
//########\n\
//# .. p #\n\
//# oo   #\n\
//#      #\n\
//########";
void initializeGlobalVariables(){
	//����18
	m[ 18 ] = 8;
	//������19
	m[ 19 ] = 5;
	//2�s��
	m[ 68 ] = '#';
	m[ 69 ] = ' ';
	m[ 70 ] = '.';
	m[ 71 ] = '.';
	m[ 72 ] = ' ';
	m[ 73 ] = 'p';
	m[ 74 ] = ' ';
	m[ 75 ] = '#';
	//3�s��
	m[ 76 ] = '#';
	m[ 77 ] = ' ';
	m[ 78 ] = 'o';
	m[ 79 ] = 'o';
	m[ 80 ] = ' ';
	m[ 81 ] = ' ';
	m[ 82 ] = ' ';
	m[ 83 ] = '#';
	//4�s��
	m[ 84 ] = '#';
	m[ 85 ] = ' ';
	m[ 86 ] = ' ';
	m[ 87 ] = ' ';
	m[ 88 ] = ' ';
	m[ 89 ] = ' ';
	m[ 90 ] = ' ';
	m[ 91 ] = '#';
	//1�s�ڂ�5�s�ڂ͑S��#
	for ( m[ 0 ] = 0; m[ 0 ] < m[ 18 ]; ++m[ 0 ] ){
		m[ 60 + m[ 0 ] ] = '#';
		m[ 92 + m[ 0 ] ] = '#';
	}
}
