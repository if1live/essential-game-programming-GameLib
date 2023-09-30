#ifndef INCLUDED_GAMELIB_DEBUGSCREEN_H
#define INCLUDED_GAMELIB_DEBUGSCREEN_H

#include <string>

namespace GameLib{
using namespace std;

class DebugScreen{
public:
	struct Color{
		Color( unsigned c ) : mColor( c ){}
		unsigned mColor;
	};
	DebugScreen();
	static DebugScreen instance();

	int precision( int );
	DebugScreen& operator<<( char );
	DebugScreen& operator<<( unsigned char );
	DebugScreen& operator<<( int );
	DebugScreen& operator<<( unsigned );
	DebugScreen& operator<<( short );
	DebugScreen& operator<<( unsigned short );
	DebugScreen& operator<<( float );
	DebugScreen& operator<<( double );
	DebugScreen& operator<<( const string& );
	DebugScreen& operator<<( const char* );
	DebugScreen& operator<<( char* );
	DebugScreen& operator<<( const Color& );
	DebugScreen& operator<<( DebugScreen& ( *f )( DebugScreen& ) );
	DebugScreen& endl();
	DebugScreen& hex();
	DebugScreen& dec();
	//�F��ύX����B�A���t�@��0���Ə����255�ɂ���̂ŁA0xff4477�̂悤�ɓn���Ċy
	DebugScreen& setColor( unsigned );
	//�ʒu���ړ�������Brow�͍s���̈ʒu��\���A���s�����row����n�܂�B
	DebugScreen& setPosition( int row, int column );
	//�F�A�ʒu�̌��ݒl�������炸�ɒP���ŕ`��
	void draw( int col, int row, const char* s, unsigned color = 0xffffffff );
};

DebugScreen& endl( DebugScreen& );
DebugScreen& hex( DebugScreen& );
DebugScreen& dec( DebugScreen& );

} //namespace GameLib

#endif
