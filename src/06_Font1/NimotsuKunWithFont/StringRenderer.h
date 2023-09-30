#ifndef INCLUDED_STRING_RENDERER_H
#define INCLUDED_STRING_RENDERER_H

class Image;

//������`��N���X
//�V���O���g���p�^�[���̃T���v���ł�����B
class StringRenderer{
public:
	static StringRenderer* instance();
	//�`��֐��B�f�t�H���g��
	void draw( int x, int y, const char* string, unsigned color = 0xffffff ) const;
	static void create( const char* fontFileName ); //������Ă�ŏ�����
	static void destroy(); //������Ă�ŏI��
private:
	StringRenderer( const char* fontFileName ); //�v���C�x�[�g�ɂ���B
	StringRenderer( StringRenderer& ); //�R�s�[�R���X�g���N�^���E�Bcpp�ɒ��g�����Ȃ��B
	~StringRenderer(); //�v���C�x�[�g�ɂ���B

	Image* mImage;
	static StringRenderer* mInstance; //�B��̃C���X�^���X
};

#endif