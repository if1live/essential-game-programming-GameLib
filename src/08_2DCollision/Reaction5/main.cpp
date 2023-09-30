#include "GameLib/Framework.h"

class Square{
public:
	void set( int x, int y, int halfSize ){
		mX = x;
		mY = y;
		mHalfSize = halfSize;
	}
	bool solvePenetration( const Square& b ){
		int al = mX - mHalfSize; //left A
		int ar = mX + mHalfSize; //right A
		int bl = b.mX - b.mHalfSize; //left B
		int br = b.mX + b.mHalfSize; //right B
		int at = mY - mHalfSize; //top A
		int ab = mY + mHalfSize; //bottom A
		int bt = b.mY - b.mHalfSize; //top B
		int bb = b.mY + b.mHalfSize; //bottom B

		bool hitX = ( al < br ) && ( ar > bl ); //���E�����œ�����
		bool hitY = ( at < bb ) && ( ab > bt ); //�㉺�����œ�����

		if ( hitX && hitY ){ //�����œ�����Ȃ��Ə����͂��Ȃ�
			int px = 0;
			int py = 0; //penetrationX, penetrationY
			int directionX = 0;
			int directionY = 0;

			int pl = ar - bl; //������̂߂肱��
			int pr = br - al; //�E����̂߂肱��
			if ( pl < pr ){
				px = pl;
				directionX = -1;
			}else{
				px = pr;
				directionX = 1;
			}
			int pt = ab - bt; //�ォ��̂߂肱��
			int pb = bb - at; //������̂߂肱��
			if ( pt < pb ){
				py = pt;
				directionY = -1;
			}else{
				py = pb;
				directionY = 1;
			}
			if ( px < py ){
				mX += directionX * px;
			}else{
				mY += directionY * py;
			}
			return true;
		}else{
			return false;
		}
	}
	int mX;
	int mY;
	int mHalfSize;
};
bool gFirstFrame = true;
Square gPlayer; //���L�����̂���
Square gWall; //��

namespace GameLib{
	void Framework::update(){
		if ( gFirstFrame ){
			setFrameRate( 60 );
			gFirstFrame = false;
			gPlayer.set( 16, 16, 16 );
			gWall.set( 160, 120, 16 );
		}
		//�ړ��ʃQ�b�g
		int dx = 0;
		int dy = 0;
		if ( isKeyOn( 'a' ) ){
			dx = -1;
		}else if ( isKeyOn( 's' ) ){
			dx = 1;
		}
		if ( isKeyOn( 'w' ) ){
			dy = -1;
		}else if ( isKeyOn( 'z' ) ){
			dy = 1;
		}
		unsigned* vram = videoMemory();
		//�ړ�
		gPlayer.mX += dx;
		gPlayer.mY += dy;
		//�Փˏ���
		unsigned color = 0xffff0000;
		if ( gPlayer.solvePenetration( gWall ) ){
			color = 0xffffffff;
		}
		//�`��
		//��U�N���A
		for ( int i = 0; i < width() * height(); ++i ){
			vram[ i ] = 0;
		}
		//�����Ȃ��ق��`��
		for ( int y = 0; y < 32; ++y ){
			for ( int x = 0; x < 32; ++x ){
				int tx = x + gWall.mX - 16;
				int ty = y + gWall.mY - 16;
				vram[ ty * width() + tx ] = 0xff0000ff; 
			}
		}
		//�����ق��`��
		for ( int y = 0; y < 32; ++y ){
			for ( int x = 0; x < 32; ++x ){
				int tx = x + gPlayer.mX - 16;
				int ty = y + gPlayer.mY - 16;
				vram[ ty * width() + tx ] = color; 
			}
		}
	}
}