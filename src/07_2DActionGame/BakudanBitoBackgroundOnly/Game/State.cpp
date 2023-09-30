#include "GameLib/Framework.h"

#include "Game/State.h"
#include "Game/StaticObject.h"

#include "Image.h"

using namespace std;
using namespace GameLib;

namespace{ //���O�Ȃ����O���

//�}�b�v�̍L��
const int WIDTH = 19;
const int HEIGHT = 15;

//�K���X�e�[�W�f�[�^
struct StageData{
	int mEnemyNumber; //�G�̐�
	int mBrickRate; //������(�p�[�Z���g)
	int mItemPowerNumber; //�����A�C�e���̐�
	int mItemBombNumber; //���e�A�C�e���̐�
};

StageData gStageData[] = {
	{ 2, 90, 4, 6, },
	{ 3, 80, 1, 0, },
	{ 6, 30, 0, 1, },
};

} //namespace

State::State( int stageID ) : 
mImage( 0 ),
mStageID( stageID ){
	Framework f = Framework::instance(); //��ŉ��x���g���̂�
	mStaticObjects.setSize( WIDTH, HEIGHT );

	mImage = new Image( "data/image/bakudanBitoImage.dds" );

	const StageData& stageData = gStageData[ mStageID ];
	int n = HEIGHT * WIDTH; //�}�X�ڂ̐�

	//�����̃u���b�N���L�^�B
	unsigned* brickList = new unsigned[ n ];
	int brickNumber = 0; //�{���Ƀ����K�ɂȂ��������J�E���g

	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			StaticObject& o = mStaticObjects( x, y );
			if ( x == 0 || y == 0 || ( x == WIDTH-1 ) || ( y == HEIGHT-1 ) ){
				o.setFlag( StaticObject::FLAG_WALL );
			}else if ( ( x % 2 == 0 ) && ( y % 2 == 0 ) ){ //�R���N���[�g
				o.setFlag( StaticObject::FLAG_WALL );
			}else if ( y + x < 4 ){
				//����3�}�X�͏�
			}else if ( ( stageID == 0 ) && ( y + x > ( WIDTH + HEIGHT - 6 ) ) ){
				//��l�p�Ȃ�E��3�}�X���󂯂�B
			}else{ //�c��͗��������B100�ʃT�C�R����U���Č��߂�
				if ( f.getRandom( 100 ) < stageData.mBrickRate  ){
					o.setFlag( StaticObject::FLAG_BRICK );
					//������������L�^���Ă����B
					brickList[ brickNumber ] = ( x << 16 ) + y;
					++brickNumber;
				}
			}
		}
	}
	//�����ɃA�C�e�����d����
	int powerNumber = stageData.mItemPowerNumber;
	int bombNumber = stageData.mItemBombNumber;
	//�����́A�������X�g��i�Ԗڂ�K���Ȃ��̂Ǝ��ւ��āA�����ɃA�C�e��������B
	for ( int i = 0; i < powerNumber + bombNumber; ++i ){
 		int swapped = f.getRandom( brickNumber - 1 - i ) + i; //�������A���������Ǝ��ւ���B�łȂ��Ƃ��łɓ��ꂽ�}�X���������o�Ă��Ă��܂��B
		unsigned t = brickList[ i ];
		brickList[ i ] = brickList[ swapped ];
		brickList[ swapped ] = t;

		int x = brickList[ i ] >> 16;
		int y = brickList[ i ] & 0xffff;
		StaticObject& o = mStaticObjects( x, y );
		if ( i < powerNumber ){
			o.setFlag( StaticObject::FLAG_ITEM_POWER );
		}else{
			o.setFlag( StaticObject::FLAG_ITEM_BOMB );
		}
	}
	SAFE_DELETE_ARRAY( brickList );
}

State::~State(){
	SAFE_DELETE( mImage );
}

void State::draw() const {
	//�w�i�`��
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			mStaticObjects( x, y ).draw( x, y, mImage );
		}
	}
	//TODO:�O�i�`��
	//TODO:�����`��
}

void State::update(){
	//TODO:
}

bool State::hasCleared() const {
	//TODO:
	return false;
}

bool State::isAlive1P() const {
	//TODO:
	return true;
}

bool State::isAlive2P() const {
	//TODO:
	return true;
}

