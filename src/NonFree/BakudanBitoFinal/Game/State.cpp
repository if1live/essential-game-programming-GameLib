#include "GameLib/Framework.h"

#include "Game/State.h"
#include "Game/StaticObject.h"
#include "Game/DynamicObject.h"
#include "SoundManager.h"
#include "Image.h"

using namespace GameLib;

namespace {

//�}�b�v�̍L��
static const int WIDTH = 19;
static const int HEIGHT = 15;
//���e�p�����[�^
static const int EXPLOSION_TIME = 180; //3�b
static const int EXPLOSION_LIFE = 60; //1�b

//�K���X�e�[�W�f�[�^
struct StageData{
	int mEnemyNumber; //�G�̐�
	int mBrickRate; //������(�p�[�Z���g)
	int mItemPowerNumber; //�����A�C�e���̐�
	int mItemBombNumber; //���e�A�C�e���̐�
};

static StageData gStageData[] = {
	{ 2, 50, 10, 10, },
	{ 3, 70, 1, 0, },
	{ 5, 30, 0, 1, },
};

} //namespace{}

State::State( int stageID ) : 
mImage( 0 ),
mDynamicObjects( 0 ),
mDynamicObjectNumber( 0 ),
mStageID( stageID ){
	Framework f = Framework::instance(); //��ŉ��x���g���̂�
	mStaticObjects.setSize( WIDTH, HEIGHT );

	mImage = new Image( "data/image/bakudanBitoImage.tga" );

	const StageData& stageData = gStageData[ mStageID ];
	int n = HEIGHT * WIDTH; //�}�X�ڂ̐�

	//�����̃u���b�N���L�^�B
	unsigned* brickList = new unsigned[ n ];
	int brickNumber = 0; //�{���Ƀ����K�ɂȂ��������J�E���g
	unsigned* floorList = new unsigned[ n ];
	int floorNumber = 0;

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
				}else{
					floorList[ floorNumber ] = ( x << 16 ) + y;
					++floorNumber;
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

	//���I�I�u�W�F�N�g���m��
	int playerNumber = ( mStageID == 0 ) ? 2 : 1;
	int enemyNumber = stageData.mEnemyNumber;
	mDynamicObjectNumber = playerNumber + enemyNumber;
	mDynamicObjects = new DynamicObject[ mDynamicObjectNumber ];

	//�v���C���[�z�u
	mDynamicObjects[ 0 ].set( 1, 1, DynamicObject::TYPE_PLAYER );
	mDynamicObjects[ 0 ].mPlayerID = 0;
	if ( mStageID == 0 ){
		mDynamicObjects[ 1 ].set( WIDTH-2, HEIGHT-2, DynamicObject::TYPE_PLAYER );
		mDynamicObjects[ 1 ].mPlayerID = 1;
	}
	//���ɓG���d���ށB�����̓A�C�e���ƂقƂ�Ǔ���
	for ( int i = 0; i < enemyNumber; ++i ){
 		int swapped = f.getRandom( floorNumber - 1 - i ) + i;
		unsigned t = floorList[ i ];
		floorList[ i ] = floorList[ swapped ];
		floorList[ swapped ] = t;

		int x = floorList[ i ] >> 16;
		int y = floorList[ i ] & 0xffff;
		mDynamicObjects[ playerNumber + i ].set( x, y, DynamicObject::TYPE_ENEMY );
	}
	SAFE_DELETE_ARRAY( floorList );
}

State::~State(){
	SAFE_DELETE( mImage );
	SAFE_DELETE_ARRAY( mDynamicObjects );
}

void State::draw() const {
	//�w�i�`��
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			mStaticObjects( x, y ).draw( x, y, mImage );
		}
	}
	//�O�i�`��
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		mDynamicObjects[ i ].draw( mImage );
	}
	//�����`��
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			mStaticObjects( x, y ).drawExplosion( x, y, mImage );
		}
	}
}

void State::update(){
	//���e�̏���
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			StaticObject& o = mStaticObjects( x, y );
			if ( o.checkFlag( StaticObject::FLAG_BOMB ) ){
				//1.���e�̃J�E���g���X�V
				++o.mCount;
				//2.���j�J�n�A�I������
				if ( o.checkFlag( StaticObject::FLAG_EXPLODING ) ){ //���Δ���
					if ( o.mCount == EXPLOSION_LIFE ){ //�����I�������ɂȂ���
						o.resetFlag( StaticObject::FLAG_EXPLODING | StaticObject::FLAG_BOMB );
						o.mCount = 0;
					}
				}else{ //���j����
					if ( o.mCount == EXPLOSION_TIME ){ //���j�����ɂȂ���
						o.setFlag( StaticObject::FLAG_EXPLODING );
						o.mCount = 0;
						SoundManager::instance()->playSe( SoundManager::SE_EXPLOSION );
					}else if ( o.checkFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y ) ){ //�U��
						o.setFlag( StaticObject::FLAG_EXPLODING );
						o.mCount = 0;
						SoundManager::instance()->playSe( SoundManager::SE_EXPLOSION );
					}
				}
			}else if ( o.checkFlag( StaticObject::FLAG_BRICK ) ){ //�����K�̏ꍇ�Ă��������肪�K�v
				if ( o.checkFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y ) ){ //�΂����Ă���
					++o.mCount; //�O�̃t���[���ł����΂Ȃ̂Ŕ���O�ɃC���N�������g
					if ( o.mCount == EXPLOSION_LIFE ){
						o.mCount = 0;
						o.resetFlag( StaticObject::FLAG_BRICK ); //�Ă�������
					}
				}
			}
			//3.�����͖��t���[���u���Ȃ����̂ŁA�������B
			o.resetFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y );
		}
	}
	//���ݒu
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			if ( mStaticObjects( x, y ).checkFlag( StaticObject::FLAG_EXPLODING ) ){
				setFire( x, y );
			}
		}
	}

	//1P,2P�̐ݒu���e�����J�E���g
	int bombNumber[ 2 ];
	bombNumber[ 0 ] = bombNumber[ 1 ] = 0;
	for ( int y = 0; y < HEIGHT; ++y ){
		for ( int x = 0; x < WIDTH; ++x ){
			const StaticObject& o = mStaticObjects( x, y );
			if ( o.checkFlag( StaticObject::FLAG_BOMB ) ){
				++bombNumber[ o.mBombOwner->mPlayerID ];
			}
		}
	}
	//�_�C�i�~�b�N�I�u�W�F�N�g�Ń��[�v
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		DynamicObject& o = mDynamicObjects[ i ];
		if ( o.isDead() ){ //����ł�B�I���B
			continue;
		}
		//�u�������e�ƐڐG���Ă��邩�`�F�b�N
		for ( int j = 0; j < 2; ++j ){
			if ( o.mLastBombX[ j ] >= 0 ){ //0�ȏ�Ȃ牽�������Ă���B
				if ( !o.isIntersectWall( o.mLastBombX[ j ], o.mLastBombY[ j ] ) ){
					o.mLastBombX[ j ] = o.mLastBombY[ j ] = -1;
				}
			}
		}
		//���݃Z�����擾
		int x, y;
		o.getCell( &x, &y );
		//����𒆐S�Ƃ���Z���̒�����ǂ�T���Ĕz��Ɋi�[
		int wallsX[ 9 ];
		int wallsY[ 9 ];
		int wallNumber = 0;
		for ( int i = 0; i < 3; ++i ){
			for ( int j = 0; j < 3; ++j ){
				int tx = x + i - 1;
				int ty = y + j - 1;
				const StaticObject& so = mStaticObjects( tx, ty );
				if ( so.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //�ǂ�
					bool myBomb0 = ( o.mLastBombX[ 0 ] == tx ) && ( o.mLastBombY[ 0 ] == ty );
					bool myBomb1 = ( o.mLastBombX[ 1 ] == tx ) && ( o.mLastBombY[ 1 ] == ty );
					if ( !myBomb0 && !myBomb1 ){ //�������u�������e����Ȃ�
						wallsX[ wallNumber ] = x + i - 1;
						wallsY[ wallNumber ] = y + j - 1;
						++wallNumber;
					}
				}
			}
		}
		//�ǃ��X�g��n���Ĉړ�����
		o.move( wallsX, wallsY, wallNumber );
		//�ړ���̈ʒu�Ŏ���9�}�X�ƏՓ˔��肵�Ă��낢��Ȕ���
		for ( int i = 0; i < 3; ++i ){
			for ( int j = 0; j < 3; ++j ){
				StaticObject& so = mStaticObjects( x + i - 1, y + j - 1 );
				if ( o.isIntersectWall( x + i - 1, y + j - 1 ) ){ //�G���Ă܂�
					if ( so.checkFlag( StaticObject::FLAG_FIRE_X | StaticObject::FLAG_FIRE_Y ) ){
						o.die(); //�Ă��ꂽ
					}else if ( !so.checkFlag( StaticObject::FLAG_BRICK ) ){ //�����ɂȂ��Ă���A�C�e���������
						if ( so.checkFlag( StaticObject::FLAG_ITEM_POWER ) ){
							so.resetFlag( StaticObject::FLAG_ITEM_POWER );
							++o.mBombPower;
						}else if ( so.checkFlag( StaticObject::FLAG_ITEM_BOMB ) ){
							so.resetFlag( StaticObject::FLAG_ITEM_BOMB );
							++o.mBombNumber;
						}
					}
				}
			}
		}
		//�ړ���Z���ԍ����擾
		o.getCell( &x, &y );
		//���e��u��
		if ( o.hasBombButtonPressed() ){ //���e�ݒu�{�^����������Ă���
			if ( bombNumber[ o.mPlayerID ] < o.mBombNumber ){ //���e�ő�l������
				StaticObject& so = mStaticObjects( x, y );
				if ( !so.checkFlag( StaticObject::FLAG_BOMB ) ){ //���e���Ȃ�
					so.setFlag( StaticObject::FLAG_BOMB );
					so.mBombOwner = &o;
					so.mCount = 0;

					//�u�������e�ʒu���X�V
					if ( o.mLastBombX[ 0 ] < 0 ){
						o.mLastBombX[ 0 ] = x;
						o.mLastBombY[ 0 ] = y;
					}else{
						o.mLastBombX[ 1 ] = x;
						o.mLastBombY[ 1 ] = y;
					}
					SoundManager::instance()->playSe( SoundManager::SE_SET_BOMB );
				}
			}
		}
	}
	//���B�G�ƃv���C���[�̐ڐG����B
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		for ( int j = i + 1; j < mDynamicObjectNumber; ++j ){
			mDynamicObjects[ i ].doCollisionReactionToDynamic( &mDynamicObjects[ j ] );
		}
	}
}

void State::setFire( int x, int y ){
	StaticObject& o = mStaticObjects( x, y );
	int power = o.mBombOwner->mBombPower;
	int end;
	
	//���[�v���̏����͂قƂ��4���[�v�ňꏏ�B
	//��낤�Ǝv���΋��ʉ��͂ł���B�������A����ɂ���ĉ������̂��킩��ɂ����֐����ł��Ă��܂����_������B
	//�����ł�4�񓯂����̂��������B

	//��
	end = ( x - power < 0 ) ? 0 : ( x - power );
	for ( int i = x - 1; i >= end; --i ){
		StaticObject& to = mStaticObjects( i, y );
		to.setFlag( StaticObject::FLAG_FIRE_X );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //�����u���Ă���Ή΂͎~�܂�
			break;
		}else{
			//�����A�C�e��������Ζ��E
			to.resetFlag( StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER );
		}
	}
	//�E
	end = ( x + power >= WIDTH ) ? ( WIDTH - 1 ) : ( x + power );
	for ( int i = x + 1; i <= end; ++i ){
		StaticObject& to = mStaticObjects( i, y );
		to.setFlag( StaticObject::FLAG_FIRE_X );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //�����u���Ă���Ή΂͎~�܂�
			break;
		}else{
			//�����A�C�e��������Ζ��E
			to.resetFlag( StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER );
		}
	}
	//��
	end = ( y - power < 0 ) ? 0 : ( y - power );
	for ( int i = y - 1; i >= end; --i ){
		StaticObject& to = mStaticObjects( x, i );
		to.setFlag( StaticObject::FLAG_FIRE_Y );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //�����u���Ă���Ή΂͎~�܂�
			break;
		}else{
			//�����A�C�e��������Ζ��E
			to.resetFlag( StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER );
		}
	}
	//��
	end = ( y + power >= HEIGHT ) ? ( HEIGHT - 1 ) : ( y + power );
	for ( int i = y + 1; i <= end; ++i ){
		StaticObject& to = mStaticObjects( x, i );
		to.setFlag( StaticObject::FLAG_FIRE_Y );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK | StaticObject::FLAG_BOMB ) ){ //�����u���Ă���Ή΂͎~�܂�
			break;
		}else{
			//�����A�C�e��������Ζ��E
			to.resetFlag( StaticObject::FLAG_ITEM_BOMB | StaticObject::FLAG_ITEM_POWER );
		}
	}

	//[�������牺�͗���������]

	//�������牺��
	//�A�������̃^�C�~���O�Y���̂��߂ɗ������Ă�������^�C�~���O���Y���Ă��܂������������邽�߂ɂ���B
	//��������
	//�Ƃ����ā������e�A���������Ƃ���B�E���珇�ɔ�������ƁA�܂�
	//��������
	//�ƂȂ�B�����������A�����Ă��������B���̌�A�����Ă����āA������������Ă������A
	//���@�@��
	//�ƒ������������B����ƁA���̔������Ղ���̂��Ȃ��Ȃ��āA
	//��������
	//�ƏĂ���Ă��܂��̂ł���B�����h���ɂ́A�����͘A�����锚�e�̒��ōł��x�����̂ɍ��킹�ďĂ������˂΂Ȃ�Ȃ��B
	//���̂��߂ɁA���������Ă̔������͂��͈̗͂����̃J�E���g��0�ɏ��������Ă��K�v������̂ł���B
	//���낢��Ȃ��������邪�A���΃R�s�y�ōςނ��̂������̗p���Ă݂��B
	//�ł��A�{���͔����J�n���ɔ����̋y�ԃ}�X���Œ肷����@�̕����������B
	//���e�N���X��ʌp�ӂ��āA�������Ă��}�X�̃��X�g�����̂��f�����낤�B
	//����������͑�����ɂȂ��Ă��܂��̂ŁA�T���v���Ƃ��Ă͂�낵���Ȃ��Ǝv���A�������Ă���B

	//��
	end = ( x - power < 0 ) ? 0 : ( x - power );
	for ( int i = x - 1; i >= end; --i ){
		StaticObject& to = mStaticObjects( i, y );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK ) ){ //���e�͑f�ʂ肷��B�ǂ����A�����ď����邩�炾�B
			if ( ( o.mCount == 0 ) && to.checkFlag( StaticObject::FLAG_BRICK ) ){ //�����K�Ȃ�Ă������J�E���g�J�n
				to.mCount = 0;
			}
			break;
		}
	}
	//�E
	end = ( x + power >= WIDTH ) ? ( WIDTH - 1 ) : ( x + power );
	for ( int i = x + 1; i <= end; ++i ){
		StaticObject& to = mStaticObjects( i, y );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK ) ){ 
			if ( ( o.mCount == 0 ) && to.checkFlag( StaticObject::FLAG_BRICK ) ){ //�����K�Ȃ�Ă������J�E���g�J�n
				to.mCount = 0;
			}
			break;
		}
	}
	//��
	end = ( y - power < 0 ) ? 0 : ( y - power );
	for ( int i = y - 1; i >= end; --i ){
		StaticObject& to = mStaticObjects( x, i );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK ) ){ 
			if ( ( o.mCount == 0 ) && to.checkFlag( StaticObject::FLAG_BRICK ) ){ //�����K�Ȃ�Ă������J�E���g�J�n
				to.mCount = 0;
			}
			break;
		}
	}
	//��
	end = ( y + power >= HEIGHT ) ? ( HEIGHT - 1 ) : ( y + power );
	for ( int i = y + 1; i <= end; ++i ){
		StaticObject& to = mStaticObjects( x, i );
		if ( to.checkFlag( StaticObject::FLAG_WALL | StaticObject::FLAG_BRICK ) ){
			if ( ( o.mCount == 0 ) && to.checkFlag( StaticObject::FLAG_BRICK ) ){ //�����K�Ȃ�Ă������J�E���g�J�n
				to.mCount = 0;
			}
			break;
		}
	}
}

bool State::hasCleared() const {
	//�G���c���Ă��Ȃ���΃N���A
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		if ( mDynamicObjects[ i ].isEnemy() ){
			return false;
		}
	}
	return true;
}

bool State::isAlive( int playerID ) const {
	//����ΐ����Ă���
	for ( int i = 0; i < mDynamicObjectNumber; ++i ){
		if ( mDynamicObjects[ i ].mType == DynamicObject::TYPE_PLAYER ){
			if ( mDynamicObjects[ i ].mPlayerID == playerID ){
				return true;
			}
		}
	}
	return false;
}

