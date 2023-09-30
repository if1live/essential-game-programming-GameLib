#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"

#include "Game/DynamicObject.h"
#include "Image.h"

using namespace GameLib;

namespace{

//�K���p�����[�^�Q
//���x�B�P�ʂ͓����P��/�t���[��
const int PLAYER_SPEED = 1000;
const int ENEMY_SPEED = 500;

} //namespace 

//�ł��邾���s���������l�����Ă����Bset���Ă΂�Ȃ��Ǝ��ʂ悤�ɁB
DynamicObject::DynamicObject() : 
mType( TYPE_NONE ),
mX( 0xffffffff ), 
mY( 0xffffffff ){
}

void DynamicObject::set( int x, int y, Type type ){
	//�������W�l�ɕϊ�
	mX = x * 16000 + 8000;
	mY = y * 16000 + 8000;
	mType = type;
	//�G��p�B�ړ�����������
	if ( mType == TYPE_ENEMY ){
		mDirectionX = mDirectionY = 0;
		Framework f = Framework::instance();
		switch ( f.getRandom( 4 ) ){
			case 0 : mDirectionX = 1; break;
			case 1 : mDirectionX = -1; break;
			case 2 : mDirectionY = 1; break;
			case 3 : mDirectionY = -1; break;
		}
	}
}

void DynamicObject::draw( const Image* image ) const {
	//�������W����f���W�ɕϊ�(+500�͎l�̌ܓ�)
	int dstX = ( mX - 8000 + 500 ) / 1000;
	int dstY = ( mY - 8000 + 500 ) / 1000;
	//�摜�؂�o���ʒu�̓���
	int srcX, srcY;
	srcX = srcY = -1;
	switch ( mType ){
		case TYPE_1P: srcX = 0; srcY = 0; break;
		case TYPE_2P: srcX = 16; srcY = 0; break;
		case TYPE_ENEMY: srcX = 32; srcY = 16; break;
		default: HALT( "arienai" ); break;
	}
	image->draw( dstX, dstY, srcX, srcY, 16, 16 );
}

void DynamicObject::update(){
	Framework f = Framework::instance();
	//�G����
	if ( mType == TYPE_ENEMY ){
		//�P�ʂ�ǂ��Ă݂悤�B
		//mDirectionX�͒P�ʂȂ��Bdt�̓~���b�BENEMY_SPEED�͉�f/�b�B
		//���̒i�K�ł̓~���b*��f/�b
		//�����1000��������ƕ���ƕ��q���~���b�ɂȂ��đ��E�B
		//��f��������W�ɕς���ɂ�1000�{����K�v������B���q�ƕ����1000�����E�B
		//�ȏォ��ȉ��̂悤�ɂȂ�B
		mX += mDirectionX * ENEMY_SPEED;
		mY += mDirectionY * ENEMY_SPEED;
	}else if ( mType == TYPE_1P ){ //�v���C���[����
		int dx, dy;
		dx = dy = 0;
		if ( f.isKeyOn( 'w' ) ){
			dy = -1;
		}else if ( f.isKeyOn( 'z' ) ){
			dy = 1;
		}else if ( f.isKeyOn( 'a' ) ){
			dx = -1;
		}else if ( f.isKeyOn( 's' ) ){
			dx = 1;
		}
		mX += dx * PLAYER_SPEED;
		mY += dy * PLAYER_SPEED;
	}else if ( mType == TYPE_2P ){ //�v���C���[����
		int dx, dy;
		dx = dy = 0;
		if ( f.isKeyOn( 'i' ) ){
			dy = -1;
		}else if ( f.isKeyOn( 'm' ) ){
			dy = 1;
		}else if ( f.isKeyOn( 'j' ) ){
			dx = -1;
		}else if ( f.isKeyOn( 'k' ) ){
			dx = 1;
		}
		mX += dx * PLAYER_SPEED;
		mY += dy * PLAYER_SPEED;
	}
	//���E����
	const int X_MIN = 8000;
	const int X_MAX = 320 * 1000 - 8000;
	const int Y_MIN = 8000;
	const int Y_MAX = 240 * 1000 - 8000;
	bool hit = false;
	if ( mX < X_MIN ){
		mX = X_MIN;
		hit = true;
	}else if ( mX > X_MAX ){
		mX = X_MAX;
		hit = true;
	}
	if ( mY < Y_MIN ){
		mY = Y_MIN;
		hit = true;
	}else if ( mY > Y_MAX ){
		mY = Y_MAX;
		hit = true;
	}
	//�G�Ȃ�����ς�
	if ( hit && mType == TYPE_ENEMY ){
		mDirectionX = mDirectionY = 0;
		switch ( f.getRandom( 4 ) ){
			case 0 : mDirectionX = 1; break;
			case 1 : mDirectionX = -1; break;
			case 2 : mDirectionY = 1; break;
			case 3 : mDirectionY = -1; break;
		}
	}
}