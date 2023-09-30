#ifndef INCLUDED_GAMELIB_FILEIO_OUTFILE_H
#define INCLUDED_GAMELIB_FILEIO_OUTFILE_H

namespace GameLib{
namespace FileIO{

class OutFile{
public:
	static OutFile create( const char* filename, const char* data, int size );
	///�Z�[�u�I������H
	bool isFinished() const;
	///�G���[�o�Ă܂��H
	bool isError() const;

	//�ȉ����[�U�͈ӎ����Ȃ��֐��Q
	OutFile();
	OutFile( const OutFile& );
	~OutFile();
	operator void*() const;
	OutFile& operator=( const OutFile& );
	void release();
	bool operator==( const OutFile& ) const;
	bool operator!=( const OutFile& ) const;

	class Impl;
private:
	Impl* mImpl;
};

} //namespace FileIO
} //namespace GameLib

#endif
