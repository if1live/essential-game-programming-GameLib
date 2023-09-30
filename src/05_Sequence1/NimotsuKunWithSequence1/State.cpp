#include "State.h"
#include "Image.h"

#include "GameLib/Framework.h"
using namespace GameLib;

//�I�u�W�F�N�g�N���X
class State::Object{
public:
	enum Type{
		OBJ_SPACE,
		OBJ_WALL,
		OBJ_BLOCK,
		OBJ_MAN,

		OBJ_UNKNOWN,
	};
	//�}�X�`��֐�
	enum ImageID{
		IMAGE_ID_PLAYER,
		IMAGE_ID_WALL,
		IMAGE_ID_BLOCK,
		IMAGE_ID_GOAL,
		IMAGE_ID_SPACE,
	};
	Object() : mType( OBJ_WALL ), mGoalFlag( false ), mMoveX( 0 ), mMoveY( 0 ){}
	//�X�e�[�W�f�[�^�̕����Ŏ�����������
	void set( char c ){
		switch ( c ){
			case '#': mType = OBJ_WALL; break;
			case ' ': mType = OBJ_SPACE; break;
			case 'o': mType = OBJ_BLOCK; break;
			case 'O': mType = OBJ_BLOCK; mGoalFlag = true; break;
			case '.': mType = OBJ_SPACE; mGoalFlag = true; break;
			case 'p': mType = OBJ_MAN; break;
			case 'P': mType = Object::OBJ_MAN; mGoalFlag = true; break;
		}
	}
	//�`��B�w�i�`��
	void drawBackground( int x, int y, const Image* image ) const {
		ImageID id = IMAGE_ID_SPACE;
		//�ǂȂ��
		if ( mType == OBJ_WALL ){
			drawCell( x, y, IMAGE_ID_WALL, image );
		}else{
			if ( mGoalFlag ){
				drawCell( x, y, IMAGE_ID_GOAL, image );
			}else{
				drawCell( x, y, IMAGE_ID_SPACE, image );
			}
		}
	}
	void drawForeground( int x, int y, const Image* image, int moveCount ) const {
		//�����̂͐l�Ɖו������B
		ImageID id = IMAGE_ID_SPACE; //�O�i���Ȃ��t���O�Ƃ��Ďg��
		if ( mType == OBJ_BLOCK ){
			id = IMAGE_ID_BLOCK;
		}else if ( mType == OBJ_MAN ){
			id = IMAGE_ID_PLAYER;
		}
		if ( id != IMAGE_ID_SPACE ){ //�w�i�ȊO�Ȃ�
			const int m = State::MAX_MOVE_COUNT; //�����̂ŕʖ�
			//�ړ����v�Z
			int dx = ( mMoveX * ( m - moveCount ) * 32 ) / m;
			int dy = ( mMoveY * ( m - moveCount ) * 32 ) / m;
			image->draw( x*32 - dx, y*32 - dy, id*32, 0, 32, 32 );
		}
	}
	static void drawCell( 
	int x, 
	int y, 
	int id, 
	const Image* image ){
		image->draw( x*32, y*32, id*32, 0, 32, 32 );
	}
	//�ړ����Z�b�g�B��3�����͒u�������^�C�v
	void move( int dx, int dy, Type replaced ){
		mMoveX = dx;
		mMoveY = dy;
		mType = replaced;
	}
	Type mType;
	bool mGoalFlag;
	int mMoveX;
	int mMoveY;
};

State::State( const char* stageData, int size ) : mImage( 0 ),mMoveCount( 0 ){	
	//�T�C�Y����
	setSize( stageData, size );
	//�z��m��
	mObjects.setSize( mWidth, mHeight );
	//�X�e�[�W�����ݒ�
	int x = 0;
	int y = 0;
	for ( int i = 0; i < size; ++i ){
		Object t;
		bool goalFlag = false;
		switch ( stageData[ i ] ){
			case '#': case ' ': case 'o': case 'O':
			case '.': case 'p': case 'P':
				mObjects( x, y ).set( stageData[ i ] );
				++x;
				break;
			case '\n': x = 0; ++y; break; //���s����
		}
	}
	//�摜�ǂݍ���
	mImage = new Image( "nimotsuKunImage2.dds" );
}

State::~State(){
	SAFE_DELETE( mImage );
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
				mWidth = ( mWidth > x ) ? mWidth : x;
				mHeight = ( mHeight > y ) ? mHeight : y;
				x = 0; 
				break;
		}
	}
}

void State::draw() const {
	//��i�K�ɕ����ĕ`�悷��B�܂��w�i��`��B
	for ( int y = 0; y < mHeight; ++y ){
		for ( int x = 0; x < mWidth; ++x ){
			mObjects( x, y ).drawBackground( x, y, mImage );
		}
	}
	//���ɑO�i��`��
	for ( int y = 0; y < mHeight; ++y ){
		for ( int x = 0; x < mWidth; ++x ){
			mObjects( x, y ).drawForeground( x, y, mImage, mMoveCount );
		}
	}
}

void State::update( int dx, int dy ){
	//�ړ����J�E���g��MAX_MOVE_COUNT�ɒB������
	if ( mMoveCount >= MAX_MOVE_COUNT ){
		mMoveCount = 0; //�����߂��āA
		//�ړ���������
		for ( int y = 0; y < mHeight; ++y ){
			for ( int x = 0; x < mWidth; ++x ){
				mObjects( x, y ).mMoveX = 0;
				mObjects( x, y ).mMoveY = 0;
			}
		}
	}
	//�ړ����͍X�V���Ȃ��B
	if ( mMoveCount > 0 ){
		++mMoveCount;
		return;
	}
	//�Z���ϐ���������B
	int w = mWidth;
	int h = mHeight;
	Array2D< Object >& o = mObjects;
	//�l���W������
	int x, y;
	bool found = false;
	for ( y = 0; y < mHeight; ++y ){
		for ( x = 0; x < mWidth; ++x ){
			if ( o( x, y ).mType == Object::OBJ_MAN ){
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
	if ( o( tx, ty ).mType == Object::OBJ_SPACE ){
		o( tx, ty ).move( dx, dy, Object::OBJ_MAN );
		o( x, y ).move( dx, dy, Object::OBJ_SPACE );
		mMoveCount = 1; //�ړ��J�n
	//B.���̕��������B���̕����̎��̃}�X���󔒂܂��̓S�[���ł���Έړ��B
	}else if ( o( tx, ty ).mType == Object::OBJ_BLOCK ){
		//2�}�X�悪�͈͓����`�F�b�N
		int tx2 = tx + dx;
		int ty2 = ty + dy; 
		if ( tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h ){ //�����Ȃ�
			return;
		}
		if ( o( tx2, ty2 ).mType == Object::OBJ_SPACE ){
			//��������ւ�
			o( tx2, ty2 ).move( dx, dy, Object::OBJ_BLOCK );
			o( tx, ty ).move( dx, dy, Object::OBJ_MAN );
			o( x, y ).move( dx, dy, Object::OBJ_SPACE );
			mMoveCount = 1; //�ړ��J�n
		}
	}
}

//�u���b�N�̂Ƃ����goalFlag����ł�false�Ȃ�
//�܂��N���A���ĂȂ�
bool State::hasCleared() const {
	for ( int y = 0; y < mHeight; ++y ){
		for ( int x = 0; x < mWidth; ++x ){
			if ( mObjects( x, y ).mType == Object::OBJ_BLOCK ){
				if ( mObjects( x, y ).mGoalFlag == false ){
					return false;
				}
			}
		}
	}
	return true;
}
