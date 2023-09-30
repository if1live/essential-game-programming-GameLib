#include "GameLib/GameLib.h"
#include "Loader.h"
#include <fstream>

#include <sstream>
#include "GameLib/Framework.h"
using namespace std;

namespace{ //����namespace�̎g�p�Ɋ���悤�B
	int getInt( ifstream& in ){
		unsigned char buffer[ 4 ];
		in.read( reinterpret_cast< char* >( buffer ), 4 );
		int r = buffer[ 0 ];
		r |= ( buffer[ 1 ] << 8 );
		r |= ( buffer[ 2 ] << 16 );
		r |= ( buffer[ 3 ] << 24 );
		return r;
	}
} 

File::File( const char* filename ) : 
mFilename( filename ),
mData( 0 ),
mSize( 0 ){
}

File::~File(){
	if ( mData ){
		SAFE_DELETE_ARRAY( mData );
	}
	mSize = 0;
}

bool File::isReady() const {
	return ( mData != 0 ); //���[�h���I���܂Ńf�[�^�ɒl�͓���Ȃ��̂ł���ŃI�[�P�[
}

int File::getSize() const {
	ASSERT( mData && "Loading is not finished. " );
	return mSize;
}

const char* File::getData() const {
	ASSERT( mData && "Loading is not finished. " );
	return mData;
}

Archive::Archive( const char* name ){
	//�t�@�C�����J���ă����o�Ɏ����Ă����B
	mStream = new ifstream( name, ifstream::binary );
	//��������4�o�C�g�O�ֈړ�
	mStream->seekg( -4, ifstream::end );
	//getInt()��4�o�C�g�ǂݍ����int��Ԃ��֐��Ƃ��悤�B
	int tableBegin = getInt( *mStream );
	//�e�[�u���擪�ֈړ�
	mStream->seekg( tableBegin, ifstream::beg );
	//4�o�C�g�ǂނƃt�@�C����
	mFileNumber = getInt( *mStream );
	//��̓��[�v�ŉ񂵂Ȃ���ǂ�ł����B
	for ( int i = 0; i < mFileNumber; ++i ){
		Entry e;
		e.mPosition = getInt( *mStream );
		e.mSize = getInt( *mStream );
		int nameLength = getInt( *mStream );
		//���O�͈�U�ꎞ�z��ɂ����B����delete���邪�B
		char* name = new char[ nameLength + 1 ]; //�I�[NULL��+1
		mStream->read( name, nameLength );
		name[ nameLength ] = '\0'; //�I�[NULL
		//mEntries��map< char*, Entry >
		mEntries.insert( make_pair( name, e ) ); //map�Ɋi�[
		//����f���o���Ă݂悤�B���������H
//		GameLib::cout << e.mPosition << " " << e.mSize << " " << nameLength << " " << name << GameLib::endl;
		SAFE_DELETE_ARRAY( name );
	}
}

Archive::~Archive(){
	SAFE_DELETE( mStream );
}

void Archive::read( File* f )  {
	typedef map< string, Entry >::const_iterator It;
	It it = mEntries.find( f->mFilename );
	if ( it != mEntries.end() ){
		const Entry& e = it->second;
		f->mData = new char[ e.mSize ]; 
		f->mSize = e.mSize;
		//�ꏊ�ړ�
		mStream->seekg( e.mPosition, ifstream::beg );
		//�ǂݍ���
		mStream->read( f->mData, e.mSize );
	}else{ //�Ȃ�
		ASSERT( false );
	}
}

Loader* Loader::mInstance = 0;

Loader::Loader( const char* archiveName ) : mArchive( 0 ){
	if ( archiveName ){ //�A�[�J�C�u������悤�ł��B�J���ď���������Ƃ��悤�B
		mArchive = new Archive( archiveName );
	}
}

Loader::~Loader(){
	typedef list< File* >::iterator It;
	for ( It i = mFiles.begin(); i != mFiles.end(); ++i ){
		SAFE_DELETE( *i );
	}
	if ( mArchive ){
		SAFE_DELETE( mArchive );
	}
}

Loader* Loader::instance(){
	return mInstance;
}

void Loader::create( const char* archiveName ){
	ASSERT( !mInstance && "already created." );
	mInstance = new Loader( archiveName );
}

void Loader::destroy(){
	SAFE_DELETE( mInstance );
}

void Loader::createFile( File** f, const char* filename ){
	ASSERT( !( *f ) && "pointer must be NUL. " ); 
	*f = new File( filename );
	mFiles.push_back( *f );
}

void Loader::destroyFile( File** f ){
	if ( !( *f ) ){ //���ł�0�B��邱�ƂȂ��B
		return;
	}
	typedef list< File* >::iterator It;
	for ( It i = mFiles.begin(); i != mFiles.end(); ++i ){
		if ( *i == *f ){ //���������B
			SAFE_DELETE( *f ); //����������
			mFiles.erase( i ); //���X�g�����������
			*f = 0; //�󂯎��|�C���^��0��
			break;
		}
	}
	ASSERT( !( *f ) && "can't find this pointer in the list. there must be a BUG!" );
}

void Loader::update(){
	typedef list< File* >::iterator It;
	for ( It i = mFiles.begin(); i != mFiles.end(); ++i ){
		File* f = *i;
		if ( !f->isReady() ){ //�I����Ă˂��B���[�h�����
			if ( mArchive ){ //�A�[�J�C�u����
				mArchive->read( f );
			}else{
				ifstream in( f->mFilename.c_str(), ifstream::binary );
				in.seekg( 0, ifstream::end );
				f->mSize = static_cast< int >( in.tellg() );
				in.seekg( 0, ifstream::beg );
				f->mData = new char[ f->mSize ];
				in.read( f->mData, f->mSize );
			}
		}
	}
}
