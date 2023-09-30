#ifndef INCLUDED_GAMELIB_OSTRINGSTREAM_H
#define INCLUDED_GAMELIB_OSTRINGSTREAM_H

#include <string>

namespace GameLib{
using namespace std;

class RefString;

///std::ostringstream��u�����邽�߂̕��B���x�ɏd�_��u���A�����@�\���]���ɂȂ��Ă���B
/*!
std::ostringstream�Ƃ̑���_�́A
�Estr()���Ȃ��Bget()��Array��n���Ď��o���B
�Eclear()�őS����
�E�@�\�����Ȃ�
*/
class OStringStream{
public:
	OStringStream( int blockSize = 1024 ); //����new�T�C�Y�B�傫���قǕ��ׂ͌y�����A�������͐H���B
	~OStringStream();
	int precision( int );
	OStringStream& operator<<( char );
	OStringStream& operator<<( unsigned char );
	OStringStream& operator<<( int );
	OStringStream& operator<<( unsigned );
	OStringStream& operator<<( short );
	OStringStream& operator<<( unsigned short );
	OStringStream& operator<<( float );
	OStringStream& operator<<( double );
	OStringStream& operator<<( const string& );
	OStringStream& operator<<( const char* );
	OStringStream& operator<<( char* );
	OStringStream& operator<<( const RefString& );
	OStringStream& operator<<( OStringStream& ( *f )( OStringStream& ) );
	void write( const char*, int size );
	template< class T > void write( const T& ); //�o�C�i����������
	OStringStream& endl();
	OStringStream& hex();
	OStringStream& dec();

	void get( Array< char >* ) const;
	void get( string* ) const;
	int size() const;
	void clear();
private:
	OStringStream( const OStringStream& ); //�R�s�[�R���X�g���N�^�֎~
	void operator=( const OStringStream& ); //����֎~
	class Impl;
	Impl* mImpl;
};

OStringStream& endl( OStringStream& );
OStringStream& hex( OStringStream& );
OStringStream& dec( OStringStream& );

} //namespace GameLib
#include "GameLib/Base/Impl/OStringStreamImpl.h"

#endif
