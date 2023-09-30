#ifndef INCLUDED_GAMELIB_FILEIO_ARCHIVE_H
#define INCLUDED_GAMELIB_FILEIO_ARCHIVE_H

#include "GameLib/Base/HashMap.h"
#include "GameLib/base/Compressor.h"
#include <fstream>

namespace GameLib{
namespace FileIO{
using namespace std;

class Archive{
public:
	Archive( const char* name ) : 
	mFileNumber( 0 ),
	mBlockSize( 0 ),
	mStream( 0 ),
	mDummy( false ){
		if ( !name ){
			mDummy = true; //�_�~�[�A�[�J�C�u�B���ڃt�@�C������ǂݍ��݂܂��B
			return;
		}
		//�t�@�C�����J���ă����o�Ɏ����Ă����B
		setlocale( LC_ALL, "" ); //���ꂪ�Ȃ��Ɠ��{��t�@�C�������󂯕t���Ȃ�
		mStream = NEW ifstream( name, ifstream::binary );
		if ( !mStream->good() ){
			cout << "can't open archive: " << name << endl;
			SAFE_DELETE( mStream );
			return;
		}
		//��������4�o�C�g�O�ֈړ�
		//�t�@�C���T�C�Y����񑪂낤���B�G���[�`�F�b�N�̂��߂ɂȁB
		mStream->seekg( 0, ifstream::end );
		streamsize fileSize = mStream->tellg();
		if ( fileSize < 8 ){ //�܂����肦��
			cout << "archive size is illegal ( too small ): " << name << endl;
			SAFE_DELETE( mStream );
			return;
		}
		mStream->seekg( -8, ifstream::end );
		mBlockSize = getUnsigned();
		streamsize tableBegin = getUnsigned() * mBlockSize;
		//�e�[�u���擪�ֈړ�
		if ( tableBegin + 12 >= fileSize ){ //���肦��
			cout << "archive size is illegal ( wrong table ): " << name << endl;
			SAFE_DELETE( mStream );
			return;
		}
		mStream->seekg( tableBegin, ifstream::beg );
		//4�o�C�g�ǂނƃt�@�C����
		mFileNumber = getUnsigned();
		if ( fileSize < mFileNumber * 16 ){ //���肦��
			cout << "archive size is illegal ( wrong file number ): " << name << endl;
			SAFE_DELETE( mStream );
			return;
		}
		//�n�b�V���}�b�v�T�C�Y�m��
		mEntries.setCapacity( mFileNumber );
		//��̓��[�v�ŉ񂵂Ȃ���ǂ�ł����B
		for ( int i = 0; i < mFileNumber; ++i ){
			Entry e;
			e.mPosition = getUnsigned();
			e.mSize = getUnsigned();
			e.mOriginalSize = getUnsigned();
			e.mNecessaryBufferSize = getUnsigned();
			int nameLength = getUnsigned();
			//���O�͈�U�ꎞ�z��ɂ����B����delete���邪�B
			char* name = NEW char[ nameLength + 1 ]; //�I�[NULL��+1
			mStream->read( name, nameLength );
			name[ nameLength ] = '\0'; //�I�[NULL
			//mEntries��map< char*, Entry >
			mEntries.add( name, e ); //map�Ɋi�[
			//����f���o���Ă݂悤�B���������H
			SAFE_DELETE_ARRAY( name );
		}
	}
	~Archive(){
		SAFE_DELETE( mStream );
	}
	void open( 
	ifstream** streamOut,
	int* entryIndex,
	const char* name ){
		if ( mDummy ){
			*entryIndex = -1;
			setlocale( LC_ALL, "" ); //���ꂪ�Ȃ��Ɠ��{��t�@�C�������󂯕t���Ȃ�
			*streamOut = NEW ifstream( name, ifstream::binary );
			if ( !( ( *streamOut )->good() ) ){
				SAFE_DELETE( *streamOut ); //�Ȃ�
			}
		}else{
			if ( mFileNumber == 0 ){ //�Ȃ��A�[�J�C�u�Ȃ��Ɍ�����Ȃ�
				*entryIndex = -1;
				*streamOut = 0;
			}else{
				//�X���b�V���ƃo�b�N�X���b�V���̗������󂯕t���邽�߂Ƀo�b�N�X���b�V����
				string tName = name;
				for ( size_t i = 0; i < tName.size(); ++i ){
					if ( tName[ i ] == '/' ){
						tName[ i ] = '\\';
					}
				}
				int it = mEntries.find( tName );
				if ( !mEntries.isEnd( it ) ){
					*entryIndex = it;
					*streamOut = mStream;
					const Entry& e = *mEntries.value( *entryIndex );
					streamsize pos = e.mPosition * mBlockSize;
					mStream->seekg( pos, ifstream::beg );
				}else{
					*entryIndex = -1;
					*streamOut = 0;
				}
			}
		}
	}
	void getFileSize( streamsize* readSize, streamsize* originalSize, int entryIndex, ifstream* stream ) const {
		if ( mDummy ){
			stream->seekg( 0, ifstream::end );
			streamsize r = stream->tellg();
			stream->seekg( 0, ifstream::beg );
			*readSize = *originalSize = r;
		}else{
			STRONG_ASSERT( entryIndex >= 0 );
			STRONG_ASSERT( stream == mStream );
			const Entry& e = *mEntries.value( entryIndex );
			*readSize = e.mSize;
			*originalSize = e.mOriginalSize;
		}
	}
	void allocate( char** dataOut, int size, int entryIndex ){
		if ( mDummy ){
			STRONG_ASSERT( entryIndex == -1 );
			*dataOut = NEW char[ size + 1 ]; //�e�ؐ݌vNULL�I�[�B
		}else{
			STRONG_ASSERT( entryIndex >= 0 );
			const Entry& e = *mEntries.value( entryIndex );
			int allocSize = max( e.mNecessaryBufferSize, e.mOriginalSize + 1 ); //�W�J�ɕK�v�ȃo�b�t�@�T�C�Y������Ȃ炻��������B
			*dataOut = NEW char[ allocSize ];
		}
	}
	void read(
	bool* errorOut,
	char* data,
	int size,
	int entryIndex, 
	ifstream* stream ){
		bool compressed = false;
		int originalSize = size;
		int necessaryBufferSize = size;
		if ( !mDummy ){
			STRONG_ASSERT( stream == mStream );
			STRONG_ASSERT( entryIndex >= 0 );
			const Entry& e = *mEntries.value( entryIndex );
			STRONG_ASSERT( e.mSize == size );
			if ( e.mOriginalSize != size ){
				compressed = true;
				originalSize = e.mOriginalSize;
				necessaryBufferSize = e.mNecessaryBufferSize;
			}
		}
		int readOffset = 0;
		if ( compressed ){
			readOffset = necessaryBufferSize - size; //���Ƀ��[�h�B�W�J�ɗ]���ɕK�v�ȏꍇ������̂�originalSize�ł͂Ȃ��B
		}
		stream->read( data + readOffset, size );
		if ( stream->gcount() != size ){ //�G���[��
			*errorOut = true;
			return;
		}
		if ( compressed ){
			int outSize;
			Compressor::decompress(
				data,
				&outSize,
				data + readOffset,
				size );
			if ( outSize != originalSize ){
				cout << "Decompression Failed!" << endl;
				*errorOut = true;
			}
		}
	}
	void close(	ifstream** stream ){
		if ( mDummy ){
			SAFE_DELETE( *stream );
		}else{
			*stream = 0;
		}
	}
private:
	class Entry{
	public:
		unsigned mPosition;
		int mSize;
		int mOriginalSize;
		int mNecessaryBufferSize;
	};
	unsigned getUnsigned(){
		unsigned char buffer[ 4 ];
		mStream->read( reinterpret_cast< char* >( buffer ), 4 );
		unsigned r = buffer[ 0 ];
		r |= ( buffer[ 1 ] << 8 );
		r |= ( buffer[ 2 ] << 16 );
		r |= ( buffer[ 3 ] << 24 );
		return r;
	}
	int mFileNumber;
	unsigned mBlockSize;
	ifstream* mStream;
	HashMap< string, Entry > mEntries;
	bool mDummy;
};

} //namespace FileIO
} //namespace GameLib

#endif
