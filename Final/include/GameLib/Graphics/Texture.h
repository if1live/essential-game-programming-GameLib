#ifndef INCLUDED_GAMELIB_GRAPHICS_TEXTURE_H
#define INCLUDED_GAMELIB_GRAPHICS_TEXTURE_H

namespace GameLib{
namespace PseudoXml{
	class ConstElement;
}
namespace Graphics{

class Texture{
public:
	///�G�������g���琶���Bpath������΃t�@�C�����̑O�ɒǉ�����B
	static Texture create( PseudoXml::ConstElement, const char* path = 0 );
	//��̃e�N�X�`��
	static Texture create( int width, int height, bool createMipChain );
	///TGA��������DDS�B�g���q�Ŕ��ʂ���B
	static Texture create( const char* filename );
	///TGA��������DDS�̒��g��n���B�ŏ���DDS�Ə����Ă����DDS�Ƃ݂Ȃ��A����ȊO��TGA�Ƃ݂Ȃ��B
	static Texture create( const char* fileData, int fileSize );
	///���O�Q�b�g(�j�Zxml���烍�[�h����΂��̖��O�B���͋�B)
	const char* name() const;

	///���[�h�I������H(const���Ȃ��͓̂����ŏ��������邩��)
	bool isReady(); 
	///�ǂݍ��݃G���[�N�����ĂȂ��H
	bool isError() const;

	///�������݊֐�
	void lock( unsigned** address, int* pitch, int mipLevel = 0 );
	///�����������݊֐�
	void lock( 
		unsigned** address, 
		int* pitch, 
		int x,
		int y,
		int width,
		int height,
		int mipLevel = 0 );
	void unlock( unsigned** address, int mipLevel = 0 );
	//���擾
	int width() const;
	int height() const;
	///2�p�ɒ������O�̌��̕�
	int originalWidth() const;
	///2�p�ɒ������O�̌��̍���
	int originalHeight() const;
	int mipmapNumber() const;
	///�t�@�C���ɏ����o���B�������s�̓`�F�b�N���Ȃ��B�f�o�O�p�B
	void write( const char* filename );

	//�ȉ����[�U�͈ӎ����Ȃ��֐��Q
	Texture();
	Texture( const Texture& );
	~Texture();
	operator void*() const;
	Texture& operator=( const Texture& );
	void release();
	bool operator==( const Texture& ) const;
	bool operator!=( const Texture& ) const;
	bool operator<( const Texture& ) const;

	class Impl;
	Texture( const Impl* );
private:
	friend class Manager;
	Impl* mImpl;
};

} //namespace Graphics
} //namespace GameLib

#endif
