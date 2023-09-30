#include "State.h"
#include "Image.h"

State::State( const char* stageData, int size ) : mImage( 0 ){	
	//�T�C�Y����
	setSize( stageData, size );
	//�z��m��
	mObjects.setSize( mWidth, mHeight );
	mGoalFlags.setSize( mWidth, mHeight );
	//�����l�Ŗ��߂Ƃ�
	for ( int y = 0; y < mHeight; ++y ){
		for ( int x = 0; x < mWidth; ++x ){
			mObjects( x, y ) = OBJ_WALL; //���܂��������͕�
			mGoalFlags( x, y ) = false; //�S�[������Ȃ�
		}
	}
	int x = 0;
	int y = 0;
	for ( int i = 0; i < size; ++i ){
		Object t;
		bool goalFlag = false;
		switch ( stageData[ i ] ){
			case '#': t = OBJ_WALL; break;
			case ' ': t = OBJ_SPACE; break;
			case 'o': t = OBJ_BLOCK; break;
			case 'O': t = OBJ_BLOCK; goalFlag = true; break;
			case '.': t = OBJ_SPACE; goalFlag = true; break;
			case 'p': t = OBJ_MAN; break;
			case 'P': t = OBJ_MAN; goalFlag = true; break;
			case '\n': x = 0; ++y; t = OBJ_UNKNOWN; break; //���s����
			default: t = OBJ_UNKNOWN; break;
		}
		if ( t != OBJ_UNKNOWN ){ //�m��Ȃ������Ȃ疳������̂ł���if��������
			mObjects( x, y ) = t; //��������
			mGoalFlags( x, y ) = goalFlag; //�S�[�����
			++x;
		}
	}
	//�摜�ǂݍ���
	mImage = new Image( "nimotsuKunImage2.dds" );
}

State::~State(){
	delete mImage;
	mImage = 0;
}

void State::setSize( const char* stageData, int size ){
	const char* d = stageData; //�ǂݍ��݃|�C���^
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
	for ( int y = 0; y < mHeight; ++y ){
		for ( int x = 0; x < mWidth; ++x ){
			Object o = mObjects( x, y );
			bool goalFlag = mGoalFlags( x, y );
			ImageID id = IMAGE_ID_SPACE;
			if ( o != OBJ_WALL ){ //�ǈȊO�Ȃ珰��`��
				if ( goalFlag ){
					drawCell( x, y, IMAGE_ID_GOAL );
				}else{
					drawCell( x, y, IMAGE_ID_SPACE );
				}
			}
			switch ( o ){
				case OBJ_SPACE: id = IMAGE_ID_GOAL; break;
				case OBJ_WALL: id = IMAGE_ID_WALL; break;
				case OBJ_BLOCK: id = IMAGE_ID_BLOCK; break;
				case OBJ_MAN: id = IMAGE_ID_PLAYER; break;
			}
			if ( o != OBJ_SPACE ){ //���͂����`�����̂ŕs�v
				drawCell( x, y, id );
			}
		}
	}
}

void State::drawCell( int x, int y, ImageID id ) const {
	mImage->draw( x*32, y*32, id*32, 0, 32, 32 );
}

void State::update( int dx, int dy ){
	//�Z���ϐ���������B
	int w = mWidth;
	int h = mHeight;
	Array2D< Object >& o = mObjects;
	//�l���W������
	int x, y;
	bool found = false;
	for ( y = 0; y < mHeight; ++y ){
		for ( x = 0; x < mWidth; ++x ){
			if ( o( x, y ) == OBJ_MAN ){
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
	if ( o( tx, ty ) == OBJ_SPACE ){
		o( tx, ty ) = OBJ_MAN;
		o( x, y ) = OBJ_SPACE;
	//B.���̕��������B���̕����̎��̃}�X���󔒂܂��̓S�[���ł���Έړ��B
	}else if ( o( tx, ty ) == OBJ_BLOCK ){
		//2�}�X�悪�͈͓����`�F�b�N
		int tx2 = tx + dx;
		int ty2 = ty + dy; 
		if ( tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h ){ //�����Ȃ�
			return;
		}
		if ( o( tx2, ty2 ) == OBJ_SPACE ){
			//��������ւ�
			o( tx2, ty2 ) = OBJ_BLOCK;
			o( tx, ty ) = OBJ_MAN;
			o( x, y ) = OBJ_SPACE;
		}
	}
}

//�u���b�N�̂Ƃ����goalFlag����ł�false�Ȃ�
//�܂��N���A���ĂȂ�
bool State::hasCleared() const {
	for ( int y = 0; y < mHeight; ++y ){
		for ( int x = 0; x < mWidth; ++x ){
			if ( mObjects( x, y ) == OBJ_BLOCK ){
				if ( mGoalFlags( x, y ) == false ){
					return false;
				}
			}
		}
	}
	return true;
}
