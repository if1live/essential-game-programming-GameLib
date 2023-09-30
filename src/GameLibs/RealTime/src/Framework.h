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
	//�������擾����(�~���b�P��)
	unsigned time() const;
	//�Q��(�~���b�P��)
	void sleep( int milliSeconds ) const;
	//����
	bool isKeyOn( int c ) const;


	//�ȉ����C�u�����g�p�҂͒m��Ȃ��Ă���
	void start( void* windowHandle );
	void preUpdate();
	void postUpdate();
	static void create();
	static void destroy();
};

} //namespace GameLib

#include "GameLib/Base/DebugStream.h"

#endif
