#ifndef INCLUDED_GAME_STATIC_OBJECT_H
#define INCLUDED_GAME_STATIC_OBJECT_H

class Image;
class DynamicObject;

class StaticObject{
public:
	enum Flag{
		FLAG_WALL = ( 1 << 0 ), //コンクリです
		FLAG_BRICK = ( 1 << 1 ), //レンガです
		FLAG_BOMB = ( 1 << 2 ), //爆弾です
		FLAG_ITEM_BOMB = ( 1 << 3 ), //爆弾アイテム
		FLAG_ITEM_POWER = ( 1 << 4 ), //爆風アイテム
		FLAG_FIRE_X = ( 1 << 5 ), //横方向炎
		FLAG_FIRE_Y = ( 1 << 6 ), //縦方向炎
		FLAG_EXPLODING = ( 1 << 7 ), //爆発中
	};
	StaticObject();

	bool checkFlag( unsigned ) const;
	void setFlag( unsigned );
	void resetFlag( unsigned );
	//床、壁、煉瓦を描画し、爆弾やアイテムがあればそれも描画
	void draw( int x, int y, const Image* ) const;
	//爆風を描画
	void drawExplosion( int x, int y, const Image* ) const ;

	int mCount;	//何かのカウント(爆弾設置、爆発、焼かれ始め)
	//爆弾専用
	DynamicObject* mBombOwner; //爆弾の持ち主
private:
	unsigned mFlags; //フラグ
};

#endif
