#include "GameLib/Framework.h"

class Square{
public:
	void set( int x, int y, int halfSize ){
		mX = x;
		mY = y;
		mHalfSize = halfSize;
	}
	bool isIntersect( const Square& b ) const {
		int al = mX - mHalfSize; //left A
		int ar = mX + mHalfSize; //right A
		int bl = b.mX - b.mHalfSize; //left B
		int br = b.mX + b.mHalfSize; //right B
		if ( ( al < br ) && ( ar > bl ) ){
			int at = mY - mHalfSize; //top A
			int ab = mY + mHalfSize; //bottom A
			int bt = b.mY - b.mHalfSize; //top B
			int bb = b.mY + b.mHalfSize; //bottom B
			if ( ( at < bb ) && ( ab > bt ) ){
				return true;
			}
		}
		return false;
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
			dx = -7;
		}else if ( isKeyOn( 's' ) ){
			dx = 7;
		}
		if ( isKeyOn( 'w' ) ){
			dy = -7;
		}else if ( isKeyOn( 'z' ) ){
			dy = 7;
		}
		unsigned* vram = videoMemory();
		//�Փˏ���(�ő�4��)
		unsigned color = 0xffff0000;
		int numerator = 1; //���q
		int denominator = 1; //����
		int testDx = dx; //���ꂩ�玎��dx,dy
		int testDy = dy;
		int lastDx = 0; //���v�������ő��dx,dy
		int lastDy = 0;
		for ( int i = 0; i < 4; ++i ){
			Square tSquare; //�e���|����
			tSquare.set( gPlayer.mX + testDx, gPlayer.mY + testDy, gPlayer.mHalfSize );
			numerator *= 2;
			denominator *= 2;
			if ( tSquare.isIntersect( gWall ) ){
				color = 0xffffffff;
				numerator -= 1;
			}else{
				numerator += 1;
				lastDx = testDx; //�X�V
				lastDy = testDy;
			}
			testDx = dx * numerator / denominator;
			testDy = dy * numerator / denominator;
		}
		//�ړ�
		gPlayer.mX += lastDx;
		gPlayer.mY += lastDy;

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