#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;

class StaticObject{
public:
	enum Flag{
		FLAG_WALL = ( 1 << 0 ), //�R���N���ł�
		FLAG_BRICK = ( 1 << 1 ), //�����K�ł�
		FLAG_ITEM_BOMB = ( 1 << 2 ), //���e�A�C�e��
		FLAG_ITEM_POWER = ( 1 << 3 ), //�����A�C�e��
	};
	StaticObject();

	//���A�ǁA������`�悵�A���e��A�C�e��������΂�����`��
	void draw( int x, int y, const Image* ) const;

	bool checkFlag( unsigned ) const;
	void setFlag( unsigned );
	void resetFlag( unsigned );

private:
	unsigned mFlags;
};

#endif
