#ifndef INCLUDED_GAMELIB_FILEIO_INFILE_H
#define INCLUDED_GAMELIB_FILEIO_INFILE_H

namespace GameLib{
namespace FileIO{

///�ǂݍ��݃t�@�C���n���h��
class InFile{
public:
	///�t�@�C�����[�h���J�n����
	static InFile create( const char* filename );
	///���[�h�e�ʃQ�b�g
	int size() const;
	///���[�h���e�Q�b�g
	const char* data() const; 
	///���[�h�I������H
	bool isFinished() const;
	///�G���[�o�Ă܂��H
	bool isError() const;
	//�l�Q�b�g�֐��Q
	int getInt( int position ) const;
	unsigned getUnsigned( int position ) const;
	short getShort( int position ) const;
	unsigned short getUnsignedShort( int position ) const;

	//�ȉ����[�U�͈ӎ����Ȃ��֐��Q
	InFile();
	InFile( const InFile& );
	~InFile();
	operator void*() const;
	InFile& operator=( const InFile& );
	void release();
	bool operator==( const InFile& ) const;
	bool operator!=( const InFile& ) const;

	class Impl;
private:
	Impl* mImpl;
};

} //namespace FileIO
} //namespace GameLib

#endif
