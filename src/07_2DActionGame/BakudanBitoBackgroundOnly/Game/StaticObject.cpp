#include "GameLib/GameLib.h"
using namespace GameLib;

#include "Game/StaticObject.h"
#include "Image.h"

//�f�t�H���g�ǁB�ǂɂ��Ă����Δ����邱�Ƃ͂Ȃ��B
StaticObject::StaticObject() : mFlags( 0 ){
}

void StaticObject::setFlag( unsigned f ){
	mFlags |= f;
}

void StaticObject::resetFlag( unsigned f ){
	mFlags &= ~f;
}

bool StaticObject::checkFlag( unsigned f ) const {
	return ( mFlags & f ) ? true : false;
}

void StaticObject::draw( int x, int y, const Image* image ) const {
	int srcX = -1;
	int srcY = -1;
	bool floor = false;
	if ( mFlags & FLAG_WALL ){
		srcX = 48; 
		srcY = 16;
	}else if ( mFlags & FLAG_BRICK ){
		srcX = 0; 
		srcY = 32;
	}else{
		srcX = 16; 
		srcY = 32; 
		floor = true;
	}
	image->draw( x*16, y*16, srcX, srcY, 16, 16 );
	//�I�v�V�����`��
	if ( floor ){
		srcX = -1; //�ԕ�
		if ( mFlags & FLAG_ITEM_BOMB ){
			srcX = 32;
			srcY = 0;
		}else if ( mFlags & FLAG_ITEM_POWER ){
			srcX = 48;
			srcY = 0;
		}
		if ( srcX != -1 ){
			image->draw( x*16, y*16, srcX, srcY, 16, 16 );
		}
	}
}

