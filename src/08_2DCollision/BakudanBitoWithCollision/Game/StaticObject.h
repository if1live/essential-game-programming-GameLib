#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;
class DynamicObject;

class StaticObject{
public:
	enum Flag{
		FLAG_WALL = ( 1 << 0 ), //�R���N���ł�
		FLAG_BRICK = ( 1 << 1 ), //�����K�ł�
		FLAG_BOMB = ( 1 << 2 ), //���e�ł�
		FLAG_ITEM_BOMB = ( 1 << 3 ), //���e�A�C�e��
		FLAG_ITEM_POWER = ( 1 << 4 ), //�����A�C�e��
		FLAG_FIRE_X = ( 1 << 5 ), //��������
		FLAG_FIRE_Y = ( 1 << 6 ), //�c������
		FLAG_EXPLODING = ( 1 << 7 ), //������
	};
	StaticObject();

	bool checkFlag( unsigned ) const;
	void setFlag( unsigned );
	void resetFlag( unsigned );
	//���A�ǁA������`�悵�A���e��A�C�e��������΂�����`��
	void draw( int x, int y, const Image* ) const;
	//������`��
	void drawExplosion( int x, int y, const Image* ) const ;

	int mCount;	//�����̃J�E���g(���e�ݒu�A�����A�Ă���n��)
	//���e��p
	DynamicObject* mBombOwner; //���e�̎�����
private:
	unsigned mFlags; //�t���O
};

#endif
