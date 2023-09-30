#ifndef INCLUDED_GAMELIB_IBINARYSTREAM_H
#define INCLUDED_GAMELIB_IBINARYSTREAM_H

#include "GameLib/Base/RefString.h"

namespace GameLib{

class RefString;

///�o�C�i���t�@�C���̓ǂݍ��݂��y�ɂ��邽�߂̃N���X
/*!
��{�I�ɃR�s�[�͍s��Ȃ��B���������āA���̕�����͕ێ�����Ă���K�v������B
*/
class IBinaryStream{
public:
	IBinaryStream( const char* );
	IBinaryStream( const char*, int size );
	IBinaryStream( const RefString& );
	~IBinaryStream();
	///�߂�l�͓ǂ߂��o�C�g��
	int read( char*, int size );
	///�e��g�ݍ��ݐ����^�p�̃o�[�W�����B�������s�͈����ɕԂ邪�A�n���Ȃ���Ή������Ȃ��B
	int readInt( bool* succeeded = 0 );
	unsigned readUnsigned( bool* succeeded = 0 );
	short readShort( bool* succeeded = 0 );
	unsigned short readUnsignedShort( bool* succeeded = 0 );
	char readChar( bool* succeeded = 0 );
	unsigned char readUnsignedChar( bool* succeeded = 0 );

	bool isEnd() const;
	void setPosition( int = 0 );
	int position() const;
	int size() const;
private:
	const char* mBuffer;
	int mSize;
	int mPosition;
};

} //namespace GameLib

#endif