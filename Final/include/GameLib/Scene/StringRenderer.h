#ifndef INCLUDED_GAMELIB_SCENE_STRINGRENDERER_H
#define INCLUDED_GAMELIB_SCENE_STRINGRENDERER_H

namespace GameLib{

namespace Graphics{
	class Texture;
}
namespace Math{
	class Vector2;
}
using namespace Math;

namespace Scene{

class Font;

///������`��N���X�B�D��x�w�肠��B�����Ń\�[�g���܂��B
class StringRenderer{
public:
	///�ő升�v�������A�ő�v�����s�����w��B�������assert
	static StringRenderer create( int charCapacity, int requestCapacity );
	///�s�N�Z�����W�Ŏw��Bz����float��[0,1]�ł��邱�Ƃɒ���
	void add( 
		int x, 
		int y, 
		const char* string, 
		unsigned color = 0xffffffff, 
		bool wrapAround = false,
		float z = 0.f );
	///�s�N�Z������float�ŏꏊ���w��(x=[0,w], y=[0,h], z=[0,1] )
	void add( 
		const Vector2& position, 
		const char* string, 
		unsigned color = 0xffffffff, 
		bool wrapAroung = false, 
		float z = 0.f );
	///���ߍ��񂾗v�����ꋓ�ɕ`��
	void draw();
	///�t�H���g���Z�b�g�B�Œ���͂�������Ȃ��Ǝ��͏����Ȃ��B
	void setFont( Font& );

	//�ȉ��ӎ����Ȃ��Ă���
	StringRenderer();
	~StringRenderer();
	StringRenderer( const StringRenderer& );
	operator void*() const;
	StringRenderer& operator=( const StringRenderer& );
	void release();
	bool operator==( const StringRenderer& ) const;
	bool operator!=( const StringRenderer& ) const;
private:
	class Impl;
	Impl* mImpl;
};

} //namespace Scene
} //namespace GameLib

#endif
