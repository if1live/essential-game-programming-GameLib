#ifndef INCLUDED_GAMELIB_FRAMEWORK_H
#define INCLUDED_GAMELIB_FRAMEWORK_H

#include <string>
using namespace std;

namespace GameLib{

namespace Graphics{
	class Texture;
}

namespace Math{
	class Vector3;
	class Vector2;
	class Matrix44;
}

class Framework{
public:
	enum LoadMode{
		LOAD_ARCHIVE_FIRST,
		LOAD_DIRECT_FIRST,
		LOAD_ARCHIVE_ONLY,
	};
	class Configuration{
	public:
		///���ݒ�
		void setWidth( int );
		///�����ݒ�
		void setHeight( int );
		///�E�B���h�E�^�C�g���ݒ�
		void setTitle( const char* title );
		///���������L�����ݒ�
		void enableVSync( bool = true );
		///�t���X�N���[���A���`�G�C���A�X�ݒ�
		void enableAntiAlias( bool = true );
		///�N�����̃t���X�N���[���t���O�ݒ�
		void enableFullScreen( bool = true );
		///�t���X�N���[�������֎~����
		void forbidFullScreen( bool = true );
		///�h���b�O�A���h�h���b�v�L����
		void enableDragAndDrop( bool = true );
		///�A�[�J�C�u������
		void setArchiveNumber( int );
		///�A�[�J�C�u���ݒ�
		void setArchiveName( int index, const char* );
		///���[�h���[�h�ݒ�
		void setLoadMode( LoadMode );
		///�����t�@�C�������[�h
		void loadFile( string* out, const char* filename ) const;
		///�R�}���h���C���I�v�V�������擾
		const char* commandLineString() const;
		///HALT,ASSERT�ő��������ɗ�O�𓊂���@�\�̗L�����B���蕨�ɑ����R�[�h�͓�����Ȃ��A�Ƃ����l�̂��߂̂��́B
		void enableThrowWhenHalt( bool = true );
		///�ǉ��X���b�h���ݒ�
		void setExtraThreadNumber( int );
	};
	Framework();
	///�C���X�^���X�擾
	static Framework instance();

	///���[�U��`�֐��B���Ȃ��ƃ����N�G���[
	void configure( Configuration* );
	///���[�U��`�֐��B���̒��łȂɂ��������
	void update(); 

	///�t���X�N���[���t���O�ݒ�B
	void enableFullScreen( bool );
	///��ʕ��擾
	int width() const;
	///��ʍ����擾
	int height() const;
	///�A�v�����擾
	const char* title() const;
	///�t���X�N���[���ł����H
	bool isFullScreen() const;
	///�t���X�N���[�����͋֎~����Ă܂����H
	bool isFullScreenForbidden() const;
	///�h���b�O�A���h�h���b�v�͗L���ł����H
	bool isDragAndDropEnabled() const;
	///Framework�ɏI�����߂��o��
	void requestEnd();
	///Framework�ɏI�����߂��o�Ă��邩���ׂ�
	bool isEndRequested() const;
	///�������擾����(�~���b�P��)
	unsigned time() const;
	///�t���[�����[�g�擾
	int frameRate() const;
	///�t���[�����[�g�ݒ�(�����ő҂����[�v���񂵂Ă����)
	void setFrameRate( int );
	///���O�t���[�����Ԏ擾(�~���b�P��)
	int previousFrameInterval() const;

	//�ȉ����C�u�����g�p�҂͒m��Ȃ��Ă���
	void start( void* windowHandle );
	void preUpdate();
	void postUpdate();
	static void create();
	static void destroy();
};

} //namespace GameLib

//�x�[�X�T�[�r�X�L����
#include "GameLib/GameLib.h"

#endif
