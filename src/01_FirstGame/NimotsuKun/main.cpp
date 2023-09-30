#include <iostream>
using namespace std;

//#�� _��� .�S�[�� o�u���b�N p�l
const char gStageData[] = "\
########\n\
# .. p #\n\
# oo   #\n\
#      #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;

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
void initialize( Object* state, int w, int h, const char* stageData );
void draw( const Object* state, int w, int h );
void update( Object* state, char input, int w, int h );
bool checkClear( const Object* state, int w, int h );

int main(){
	//�ꎟ���z��ł��闝�R�͖{���Q��
	Object* state = new Object[ gStageWidth * gStageHeight ]; //��Ԕz��m��

	initialize( state, gStageWidth, gStageHeight, gStageData ); //�X�e�[�W������
	//���C�����[�v
	while ( true ){
		//�܂��`��
		draw( state, gStageWidth, gStageHeight );
		//�N���A�`�F�b�N
		if ( checkClear(state, gStageWidth, gStageHeight ) ){
			break; //�N���A�`�F�b�N
		}
		//���͎擾
		cout << "a:left s:right w:up z:down. command?" << endl; //�������
		char input;
		cin >> input;
		//�X�V
		update( state, input, gStageWidth, gStageHeight ); 	
	}
	//�j���̃��b�Z�[�W
	cout << "Congratulation's! you won." << endl;
	//��n��
	delete[] state;
	state = 0;

	//Visual Studio������s����l�̂��߂ɖ������[�v�B�R�}���h���C�������Ctrl-C�ŏI���Ă��������B
	while( true ){
		;
	} 
	return 0;
}

//---------------------�ȉ��֐���`------------------------------------------


//�����g���������邾�낤�ƍ������n���d�l�ɂ������A����g���Ă��Ȃ��̂Ŗ��O����(height)�R�����g�A�E�g���Ă���B
void initialize( Object* state, int width, int /* height */, const char* stageData ){	
	const char* d = stageData; //�ǂݍ��݃|�C���^
	int x = 0;
	int y = 0;
	while ( *d != '\0' ){ //NULL�����łȂ���
		Object t; //���ɈӖ��͂Ȃ����g���񐔂������ϐ��Ɏ��͗ǂ�t���g���Btemporary�̗��B���Ԃ�悭�Ȃ��K�������A���ʂɒ������O�ɂ��ēǂ݂ɂ����̂����蕨���낤�B
		switch ( *d ){
			case '#': t = OBJ_WALL; break;
			case ' ': t = OBJ_SPACE; break;
			case 'o': t = OBJ_BLOCK; break;
			case 'O': t = OBJ_BLOCK_ON_GOAL; break;
			case '.': t = OBJ_GOAL; break;
			case 'p': t = OBJ_MAN; break;
			case 'P': t = OBJ_MAN_ON_GOAL; break;
			case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //���s����
			default: t = OBJ_UNKNOWN; break;
		}
		++d;
		if ( t != OBJ_UNKNOWN ){ //�m��Ȃ������Ȃ疳������̂ł���if��������
			state[ y*width + x ] = t; //��������
			++x;
		}
	}
}

void draw( const Object* state, int width, int height ){
	const char font[] = {' ', '#', '.', 'o', 'O', 'p', 'P'}; //Object�񋓂̏�
	for ( int y = 0; y < height; ++y ){
		for ( int x=0; x < width; ++x ){
			Object o = state[ y*width + x ];
			cout << font[ o ];
		}
		cout << endl;
	}
}

//�������͂ق��̊֐��ł�state�Ƃ��Ă��邪�A���܂�ɕp�ɂɎg���̂�
//�Z��s�ōς܂��Ă���Bw,h�����ꂼ��width,height�ł���B
void update( Object* s, char input, int w, int h ){
	//�ړ������ɕϊ�(d��difference�ł�delta�ł����D���ȕ��̗����Ǝv���ė~����)
	int dx = 0; 
	int dy = 0;
	switch ( input ){
		case 'a': dx = -1; break; //��
		case 's': dx = 1; break; //�E
		case 'w': dy = -1; break; //��BY�͉����v���X
		case 'z': dy = 1; break; //���B
	}
	//�l���W������
	int i = -1;
	for ( i = 0; i < w * h; ++i ){
		if ( s[ i ] == OBJ_MAN || s[ i ] == OBJ_MAN_ON_GOAL ){
			break;
		}
	}
	int x = i % w; //x�͕��Ŋ��������܂�
	int y = i / w; //y�͕��Ŋ�������

	//�ړ�
	//�ړ�����W(t�ɈӖ��͂Ȃ��B���߂�Ȃ���)
	int tx = x + dx;
	int ty = y + dy;
	//���W�̍ő�ŏ��`�F�b�N�B�O��Ă���Εs����
	if ( tx < 0 || ty < 0 || tx >= w || ty >= h ){
		return;
	}
	//A.���̕������󔒂܂��̓S�[���B�l���ړ��B
	int p = y*w + x; //�l�ʒu
	int tp = ty*w + tx; //�^�[�Q�b�g�ʒu(TargetPosition)
	if ( s[ tp ] == OBJ_SPACE || s[ tp ] == OBJ_GOAL ){
		s[ tp ] = ( s[ tp ] == OBJ_GOAL ) ? OBJ_MAN_ON_GOAL : OBJ_MAN; //�S�[���Ȃ�S�[����̐l��
		s[ p ] = ( s[ p ] == OBJ_MAN_ON_GOAL ) ? OBJ_GOAL : OBJ_SPACE; //���Ƃ��ƃS�[����Ȃ�S�[����
	//B.���̕��������B���̕����̎��̃}�X���󔒂܂��̓S�[���ł���Έړ��B
	}else if ( s[ tp ] == OBJ_BLOCK || s[ tp ] == OBJ_BLOCK_ON_GOAL ){
		//2�}�X�悪�͈͓����`�F�b�N
		int tx2 = tx + dx;
		int ty2 = ty + dy; 
		if ( tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h ){ //�����Ȃ�
			return;
		}

		int tp2 = ( ty + dy )*w + ( tx + dx ); //2�}�X��
		if ( s[ tp2 ] == OBJ_SPACE || s[ tp2 ] == OBJ_GOAL ){
			//��������ւ�
			s[ tp2 ] = ( s[ tp2 ] == OBJ_GOAL ) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			s[ tp ] = ( s[ tp ] == OBJ_BLOCK_ON_GOAL ) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			s[ p ] = ( s[ p ] == OBJ_MAN_ON_GOAL ) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}

//�u���b�N�݂̂��Ȃ���΃N���A���Ă���B
bool checkClear( const Object* s, int width, int height ){
	for ( int i = 0; i < width*height; ++i ){
		if ( s[ i ] == OBJ_BLOCK ){
			return false;
		}
	}
	return true;
}
