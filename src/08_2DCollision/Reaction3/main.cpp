#include "GameLib/Framework.h"

class Square{
public:
	void set( int x, int y, int halfSize ){
		mX = x;
		mY = y;
		mHalfSize = halfSize;
	}
	//1000����1�t���[���P�ʂ̓�������������Ԃ��B0����1000�܂ł̐�����Ԃ��A
	//0�Ȃ�ړ��J�n�̏u�Ԃɓ����������Ƃ��A1000�͓�����Ȃ��������Ƃ��Ӗ�����B
	int getIntersectionTime( int dx, int dy, const Square& b ) const {
		//���������_�ɂ��邩�̂悤�ɓ���ړ�����B
		int x = b.mX - mX;
		int y = b.mY - mY;
		//�����̑��x��0�ɂȂ�悤�ɁA����ɑ��x�������t����B
		int vx = -dx;
		int vy = -dy;
		//�傫���������ɏW�߂�B�����`�Ȃ̂ŒP���ɑ����Ηǂ��B
		int s = mHalfSize + b.mHalfSize;
		//���̏󋵂�(x,y)+t(vx,vy)��4�{�̒���
		//x = -s, x = s, y = -s, y = s
		//�ɓ����鎞�����v�Z����B

		int tx0, tx1, ty0, ty1; //4�̎���
		//x+t*vx=-s��x+t*vx=s���A
		if ( vx == 0 ){
			//��������Ԃ��ǂ����𒲂ׂ�
			if ( -s < x && x < s ){
				tx0 = -1000; //�ߋ��ɂ�����A�����ɔ�����A�̈Ӗ�
				tx1 = 1000;
			}else{ //�����łȂ���Ζ����i��������Ȃ�
				tx0 = tx1 = 1000;
			}
		}else{
			tx0 = ( -s - x ) * 1000 / vx;
			tx1 = ( s - x ) * 1000 / vx;
		}
		//y+t*vy=-s��y+t*vy=s���A
		if ( vy == 0 ){
			//��������Ԃ��ǂ����𒲂ׂ�
			if ( -s < y && y < s ){
				ty0 = -1000; //�ߋ��ɂ�����A�����ɔ�����A�̈Ӗ�
				ty1 = 1000;
			}else{ //�����łȂ���Ζ����i��������Ȃ�
				ty0 = ty1 = 1000;
			}
		}else{
			ty0 = ( -s - y ) * 1000 / vy;
			ty1 = ( s - y ) * 1000 / vy;
		}
		//tx0,ty0���������ق��ɂ���B�オ�y�ɂȂ�B
		int t;
		if ( tx0 > tx1 ){
			t = tx0;
			tx0 = tx1;
			tx1 = t;
		}
		if ( ty0 > ty1 ){
			t = ty0;
			ty0 = ty1;
			ty1 = t;
		}
		t = 1000; //�Փˎ���
		//tx0�ɂē����邩�`�F�b�N
		if ( tx0 >= 0 && tx0 < 1000 ){
			//�\�莞���ɂ�����y����������Ԃɂ���΂�����ł���B
			//�����ty0<=tx0<=ty1�𖞂����΂����B
			if ( ty0 <= tx0 && tx0 <= ty1 && tx0 < t ){
				t = tx0;
			}
		}
		//tx1
		if ( tx1 >= 0 && tx1 < 1000 ){
			if ( ty0 <= tx0 && tx0 <= ty1 && tx1 < t ){
				t = tx1;
			}
		}
		//ty0
		if ( ty0 >= 0 && ty0 < 1000 ){
			if ( tx0 <= ty0 && ty0 <= tx1 && ty0 < t ){
				t = ty0;
			}
		}
		//ty1
		if ( ty1 >= 0 && ty1 < 1000 ){
			if ( tx0 <= ty0 && ty0 <= tx1 && ty1 < t ){
				t = ty1;
			}
		}
		return t;
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
		//�Փˏ���
		unsigned color = 0xffff0000;
		int time = gPlayer.getIntersectionTime( dx, dy, gWall );
		if ( time < 1000 ){
			color = 0xffffffff;
		}
		//�ړ�
		gPlayer.mX += ( dx * time ) / 1000;
		gPlayer.mY += ( dy * time ) / 1000;
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