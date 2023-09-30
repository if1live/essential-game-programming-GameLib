#ifndef INCLUDED_GAMELIB_SCENE_FONT_H
#define INCLUDED_GAMELIB_SCENE_FONT_H

namespace GameLib{

namespace Graphics{
	class Texture;
}
namespace Math{
	class Vector2;
}
using namespace Math;

namespace Scene{

///�t�H���g�N���X
class Font{
public:
	///�r�b�g�}�b�v�t�H���g�̕�����Âɕt���K�v�ȃf�[�^
	struct BitmapChar{
		unsigned short mCode; //�����R�[�h(SHIFT_JIS)
		unsigned short mX; //���[
		unsigned short mY; //��[
		unsigned char mTexturePage; //�e�N�X�`���y�[�W
		unsigned char mWidth; //�����(�s�N�Z��)
		unsigned char mHeight; //�������(�s�N�Z��)
	};
	///�e�N�X�`�����g�����r�b�g�}�b�v�t�H���g�𐶐��B�����̍����͍ő�l���̗p�B
	static Font create( 
		const char** textureFileNames, //�e�N�X�`���t�@�C�����z��
		int textureNumber,
		BitmapChar* BitmapChars,
		int bitmapCharNumber );
	///�e�N�X�`�����g�����r�b�g�}�b�v�t�H���g�𐶐��B�����̍����͍ő�l���̗p�B
	static Font create(
		const char** textureFileData, //�e�N�X�`��DDS�C���[�W�z��
		const int* textureFileSizes, //�e�N�X�`���f�[�^�T�C�Y�z��
		int textureNumber,
		BitmapChar* bitmapChars,
		int CharNumber );
	///OS���瓮�I�Ƀt�H���g�f�[�^���擾����B�����̍����A�����e�N�X�`�����Ȃǂ͎w��B
	static Font create(
		const char* osFontName,
		int charHeightHint, //����͖ڈ��B���ۂɂ�������getCharHeight���Q�Ƃ���
		bool bold = false,
		bool italic = false,
		bool proportional = false,
		bool antialias = false,
		int internalTextureWidth = 512,
		int internalTextureHeight = 512 );
	///���������擾�B������Ȃ���false��Ԃ��B
	bool getChar( 
		Graphics::Texture* textureOut,
		Vector2* uvTopLeftOut,
		Vector2* uvBottomRightOut,
		int code );
	///�����̍ő卂��
	int charHeight() const;
	///���[�h�I����Ă�H
	bool isReady();

	//�ȉ����[�U�͈ӎ����Ȃ��Ă���
	Font();
	~Font();
	Font( const Font& );
	operator void*() const;
	Font& operator=( const Font& );
	void release();
	bool operator==( const Font& ) const;
	bool operator!=( const Font& ) const;

	class Impl;
private:
	explicit Font( Impl* );
	Impl* mImpl;
};

} //namespace Scene
} //namespace GameLib

#endif
