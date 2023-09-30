#ifndef INCLUDED_FILE_H
#define INCLUDED_FILE_H

class File{
public:
	File( const char* filename );
	~File();
	int size() const;
	const char* data() const;
	unsigned getUnsigned( int position ) const;

	//��������static�֐�
	static void write( const char* filename, const char* data, int size );
private:
	int mSize;
	char* mData;
};

#endif
