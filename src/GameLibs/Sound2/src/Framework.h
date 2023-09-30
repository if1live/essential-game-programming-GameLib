#ifndef INCLUDED_GAMELIB_FRAMEWORK_H
#define INCLUDED_GAMELIB_FRAMEWORK_H

namespace GameLib{

class Framework{
public:
	Framework();
	///���[�U��`�֐��B����������B
	void update();
	///�C���X�^���X�擾
	static Framework instance();
	///��ʕ��擾
	int width() const;
	///��ʍ����擾
	int height() const;
	///VRAM�擾
	unsigned* videoMemory();
	///Framework�ɏI�����߂��o��
	void requestEnd();
	///Framework�ɏI�����߂��o�Ă��邩���ׂ�
	bool isEndRequested() const;
	///�~���b�P�ʂŐQ��
	void sleep( int milliSeconds ) const;
	///�������擾����(�~���b�P��)
	unsigned time() const;
	///�t���[�����[�g�擾
	int frameRate() const;
	///�t���[�����[�g�ݒ�(�����ő҂����[�v���񂵂Ă����)
	void setFrameRate( int );
	///���O�t���[�����Ԏ擾(�~���b�P��)
	int previousFrameInterval() const;
	///�����_�����Q�b�g
	int getRandom( int max = 0 );
	///����������
	void drawDebugString( 
		int column, //��(x)
		int row, //�s(y)
		const char* string, 
		unsigned color = 0xffffffff );
	///�����o��
	void playSound( const short* data, int size );

	//�ȉ����C�u�����g�p�҂͒m��Ȃ��Ă���
	void start( void* windowHandle );
	void preUpdate();
	void postUpdate();
	static void create();
	static void destroy();
};

} //namespace GameLib

#include "GameLib/GameLib.h"

#endif
