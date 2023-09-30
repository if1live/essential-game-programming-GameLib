#ifndef INCLUDED_GAMELIB_FRAMEWORK_H
#define INCLUDED_GAMELIB_FRAMEWORK_H

namespace GameLib{

class Texture;

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
	//����������
	void drawDebugString( 
		int column, //��(x)
		int row, //�s(y)
		const char* string, 
		unsigned color = 0xffffffff );
	///�O�p�`��`���Bp,t��2�v�f�̔z��
	void drawTriangle2D(
		const double* p0,
		const double* p1,
		const double* p2,
		const double* t0 = 0,
		const double* t1 = 0,
		const double* t2 = 0,
		unsigned c0 = 0xffffffff,
		unsigned c1 = 0xffffffff,
		unsigned c2 = 0xffffffff );
	//�e�N�X�`�������
	void createTexture(
		Texture** textureOut,
		int textureWidth, 
		int textureHeight, 
		const unsigned* imageData,
		int imageWidth,
		int imageHeight );
	//�e�N�X�`����j������
	void destroyTexture( Texture** );
	//�e�N�X�`�����Z�b�g����
	void setTexture( Texture* );
	//�u�����h���[�h
	enum BlendMode{ 
		BLEND_LINEAR,
		BLEND_ADDITIVE,
		BLEND_OPAQUE,
	};
	void setBlendMode( BlendMode );

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
