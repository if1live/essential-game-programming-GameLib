#include <iostream>
#include <fstream>
using namespace std;

//�֐��v���g�^�C�v
void readFile( char** buffer, int* size, const char* filename );

//�񎟌��z��N���X
//�e���v���[�g�ɂȂ��݂͂��邾�낤���H�Ȃ���Ί�b�����ł��׋����Ă������B
//���̃N���X�錾�̒��ł�T�Ƃ����N���X�����邩�̂悤�Ɉ����A
//������g�����ɂ�T�̂Ƃ����int�Ƃ�bool�Ƃ�����Ďg���B
template< class T > class Array2D{
public:
	Array2D() : mArray( 0 ){}
	~Array2D(){
		delete[] mArray;
		mArray = 0;  //�|�C���^��0������̂̓N�Z�ɂ��悤�B
	}
	void setSize( int size0, int size1 ){
		mSize0 = size0;
		mSize1 = size1;
		mArray = new T[ size0 * size1 ];
	}
	T& operator()( int index0, int index1 ){
		return mArray[ index1 * mSize0 + index0 ];
	}
	const T& operator()( int index0, int index1 ) const {
		return mArray[ index1 * mSize0 + index0 ];
	}
private:
	T* mArray;
	int mSize0;
	int mSize1;
};

//��ԃN���X
class State{
public:
	State( const char* stageData, int size );
	void update( char input );
	void draw() const;
	bool hasCleared() const;
private:
	enum Object{
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_BLOCK,
		OBJ_MAN,
		OBJ_UNKNOWN,

		OBJ_GOAL_FLAG = ( 1 << 7 ), //�S�[���t���O
	};
	void setSize( const char* stageData, int size );

	int mWidth;
	int mHeight;
	Array2D< unsigned char > mObjects; //�r�b�g���Z����̂�unsigned char�B�����܂ł�����Ȃ��Ă��������B
};

int main( int argc, char** argv ){
	const char* filename = "stageData.txt";
	if ( argc >= 2 ){
		filename = argv[ 1 ];
	}
	char* stageData;
	int fileSize;
	readFile( &stageData, &fileSize, filename );
	if ( !stageData ){
		cout << "stage file could not be read." << endl;
		return 1;
	}
	State* state = new State( stageData, fileSize );

	//���C�����[�v
	while ( true ){
		//�܂��`��
		state->draw();
		//�N���A�`�F�b�N
		if ( state->hasCleared() ){
			break; //�N���A�`�F�b�N
		}
		//���͎擾
		cout << "a:left s:right w:up z:down. command?" << endl; //�������
		char input;
		cin >> input;
		//�X�V
		state->update( input ); 	
	}
	//�j���̃��b�Z�[�W
	cout << "Congratulation's! you won." << endl;
	//��n��
	delete[] stageData;
	stageData = 0;

	//�������[�v(ctrl-C�ȊO�ŏ���ɏI���Ȃ����߂̂���)
	while ( true ){
		;
	}
	return 0;
}

//---------------------�ȉ��֐���`------------------------------------------

void readFile( char** buffer, int* size, const char* filename ){
	ifstream in( filename );
	if ( !in ){
		*buffer = 0;
		*size = 0;
	}else{
		in.seekg( 0, ifstream::end );
		*size = static_cast< int >( in.tellg() );
		in.seekg( 0, ifstream::beg );
		*buffer = new char[ *size ];
		in.read( *buffer, *size );
	}
}

State::State( const char* stageData, int size ){	
	//�T�C�Y����
	setSize( stageData, size );
	//�z��m��
	mObjects.setSize( mWidth, mHeight );
	//�����l�Ŗ��߂Ƃ�
	for ( int y = 0; y < mHeight; ++y ){
		for ( int x = 0; x < mWidth; ++x ){
			mObjects( x, y ) = OBJ_WALL; //���܂��������͕�
		}
	}
	int x = 0;
	int y = 0;
	for ( int i = 0; i < size; ++i ){
		unsigned char t;
		switch ( stageData[ i ] ){
			case '#': t = OBJ_WALL; break;
			case ' ': t = OBJ_SPACE; break;
			case 'o': t = OBJ_BLOCK; break;
			case 'O': t = OBJ_BLOCK | OBJ_GOAL_FLAG; break;
			case '.': t = OBJ_SPACE | OBJ_GOAL_FLAG; break;
			case 'p': t = OBJ_MAN; break;
			case 'P': t = OBJ_MAN | OBJ_GOAL_FLAG; break;
			case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //���s����
			default: t = OBJ_UNKNOWN; break;
		}
		if ( t != OBJ_UNKNOWN ){ //�m��Ȃ������Ȃ疳������̂ł���if��������
			mObjects( x, y ) = t; //��������
			++x;
		}
	}
}

void State::setSize( const char* stageData, int size ){
	mWidth = mHeight = 0; //������
	//���݈ʒu
	int x = 0;
	int y = 0;
	for ( int i = 0; i < size; ++i ){
		switch ( stageData[ i ] ){
			case '#': case ' ': case 'o': case 'O':
			case '.': case 'p': case 'P':
				++x;
				break;
			case '\n': 
				++y;
				//�ő�l�X�V
				mWidth = max( mWidth, x );
				mHeight = max( mHeight, y );
				x = 0; 
				break;
		}
	}
}

void State::draw() const {
	for ( int y = 0; y < mHeight; ++y ){
		for ( int x = 0; x < mWidth; ++x ){
			switch ( mObjects( x, y ) ){
				case ( OBJ_SPACE | OBJ_GOAL_FLAG ): cout << '.'; break;
				case ( OBJ_WALL | OBJ_GOAL_FLAG ): cout << '#'; break;
				case ( OBJ_BLOCK | OBJ_GOAL_FLAG ): cout << 'O'; break;
				case ( OBJ_MAN | OBJ_GOAL_FLAG ): cout << 'P'; break;
				case OBJ_SPACE: cout << ' '; break;
				case OBJ_WALL: cout << '#'; break;
				case OBJ_BLOCK: cout << 'o'; break;
				case OBJ_MAN: cout << 'p'; break;
			}
		}
		cout << endl;
	}
}

void State::update( char input ){
	//�ړ������ɕϊ�
	int dx = 0;
	int dy = 0;
	switch ( input ){
		case 'a': dx = -1; break; //��
		case 's': dx = 1; break; //�E
		case 'w': dy = -1; break; //��BY�͉����v���X
		case 'z': dy = 1; break; //���B
	}
	//�Z���ϐ���������B�Q�Ƃ𖳗����g���Ă݂��Bconst�t���ł��邱�Ƃɒ���
	const int& w = mWidth;
	const int& h = mHeight;
	Array2D< unsigned char >& o = mObjects;
	//�l���W������
	int x, y;
	x = y = -1; //�댯�Ȓl
	bool found = false;
	for ( y = 0; y < h; ++y ){
		for ( x = 0; x < w; ++x ){
			if ( ( o( x, y ) & ~OBJ_GOAL_FLAG ) == OBJ_MAN ){ //�S�[���t���O�ȊO�����o�����߂�& ~OBJ_GOAL_FLAG
				found = true;
				break;
			}
		}
		if ( found ){
			break;
		}
	}
	//�ړ�
	//�ړ�����W
	int tx = x + dx;
	int ty = y + dy;
	//���W�̍ő�ŏ��`�F�b�N�B�O��Ă���Εs����
	if ( tx < 0 || ty < 0 || tx >= w || ty >= h ){
		return;
	}
	//A.���̕������󔒂܂��̓S�[���B�l���ړ��B
	if ( ( o( tx, ty ) & ~OBJ_GOAL_FLAG ) == OBJ_SPACE ){
		o( tx, ty ) = ( o( tx, ty ) & OBJ_GOAL_FLAG ) | OBJ_MAN; //�S�[���t���O��ۑ����邽�߂ɂ���Ȗʓ|�Ȃ��Ƃ��K�v�B�ȉ����قړ���
		o( x, y ) = ( o( x, y ) & OBJ_GOAL_FLAG ) | OBJ_SPACE;
	//B.���̕��������B���̕����̎��̃}�X���󔒂܂��̓S�[���ł���Έړ��B
	}else if ( o( tx, ty ) == OBJ_BLOCK ){
		//2�}�X�悪�͈͓����`�F�b�N
		int tx2 = tx + dx;
		int ty2 = ty + dy; 
		if ( tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h ){ //�����Ȃ�
			return;
		}
		if ( ( o( tx2, ty2 ) & ~OBJ_GOAL_FLAG ) == OBJ_SPACE ){
			//��������ւ�
			o( tx2, ty2 ) = ( o( tx2, ty2 ) & OBJ_GOAL_FLAG ) | OBJ_BLOCK;
			o( tx, ty ) = ( o( tx, ty ) & OBJ_GOAL_FLAG ) | OBJ_MAN;
			o( x, y ) = ( o( x, y ) & OBJ_GOAL_FLAG ) | OBJ_SPACE;
		}
	}
}

//�u���b�N�̂Ƃ����goalFlag����ł�false�Ȃ�
//�܂��N���A���ĂȂ�
bool State::hasCleared() const {
	for ( int y = 0; y < mHeight; ++y ){
		for ( int x = 0; x < mWidth; ++x ){
			if ( mObjects( x, y ) == OBJ_BLOCK ){ //�S�[���t���O���Ȃ������̃u���b�N
				return false;
			}
		}
	}
	return true;
}

