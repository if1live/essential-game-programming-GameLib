#ifndef INCLUDED_GAME_DYNAMIC_OBJECT_H
#define INCLUDED_GAME_DYNAMIC_OBJECT_H

class Image;
class StaticObject;

class DynamicObject{
public:
	enum Type{
		TYPE_PLAYER,
		TYPE_ENEMY,

		TYPE_NONE, //����ł܂�
	};
	DynamicObject();
	void set( int x, int y, Type );
	void draw( const Image* ) const;
	//�ړ�
	void move( const int* wallsX, int* wallsY, int wallNumber );
	//�ǂ��̃}�X�ɒ��S������̂������˂�B
	void getCell( int* x, int* y ) const;
	//dx,dy�����ړ������ۂɏd�Ȃ��Ă���}�X���W��z��ɓ���ĕԂ��B
	//������int[4]����B�߂�l�͓��ꂽ���B1,2,4�̂ǂꂩ�B
	int getIntersectionCell( int* xArray, int* yArray, int dx, int dy ) const;
	//�G���Ă���ő�4�}�X���󂯎���ĉ������s���B
	void doCollisionReactionToStatic( StaticObject** o, int cellNumber );
	void doCollisionReactionToDynamic( DynamicObject* );

	//DynamicObject�Ƃ̌�������
	bool isIntersect( const DynamicObject& ) const;
	//�Ǘp
	bool isIntersectWall( int wallCellX, int wallCellY );

	//�֗��֐��Q
	bool hasBombButtonPressed() const;	//���e�{�^���������ꂽ�����ׂ�
	bool isPlayer() const;
	bool isEnemy() const;
	void die(); //���ɂ܂�(mType��NONE�ɂ��邱�Ƃŕ\��)
	bool isDead() const; //����ł܂����H

	//���ڂ����肽�����Ȃ��̂�private�ɂ͂��Ȃ��B�p�ɂ�State����g�����̂����炾�B
	Type mType;
	//�v���C���[��p
	int mBombPower; //������
	int mBombNumber; //���e��
	int mPlayerID; //�v���C���[�ԍ�
	//���e��u���l��p
	int mLastBombX[ 2 ];
	int mLastBombY[ 2 ];
private:
	//�Ǘp
	static bool isIntersectWall( int x, int y, int wallCellX, int wallCellY );
	//���t���[���̈ړ��ʂ��擾
	void getVelocity( int* dx, int* dy ) const;
	//�ړ��������擾
	void getDirection( int* dx, int* dy ) const;
	//���W(�����P��)
	int mX;
	int mY;
	//�G��p
	int mDirectionX;
	int mDirectionY;
};

#endif
