#include "GameLib/GameLib.h"
#include "Loader.h"
#include <fstream>
using namespace std;

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

Loader* Loader::mInstance = 0;

Loader::Loader() : 
mCurrentFile( 0 ),
mStream( 0 ),
mFileSize( 0 ),
mFilePosition( 0 ),
mData( 0 ){
}

Loader::~Loader(){
	typedef list< File* >::iterator It;
	for ( It i = mFiles.begin(); i != mFiles.end(); ++i ){
		SAFE_DELETE( *i );
	}
	//�ǂ�ł�t�@�C��������Ȃ�j��
	if ( mStream ){
		SAFE_DELETE( mStream );
	}
	if ( mData ){
		SAFE_DELETE( mData );
	}
	mCurrentFile = 0; //�����delete���Ȃ��B���X�g�̒��ɂ���͂���
}

Loader* Loader::instance(){
	return mInstance;
}

void Loader::create(){
	ASSERT( !mInstance && "already created." );
	mInstance = new Loader();
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
	//���łɃ��[�h���Ȃ烍�[�h��i�߂�
	if ( mCurrentFile ){
		//�ł��A����{���Ƀ��X�g�ɂ���́H
		bool found = false;
		for ( It i = mFiles.begin(); i != mFiles.end(); ++i ){
			if ( *i == mCurrentFile ){
				found = true; //������
				break;
			}
		}
		if ( !found ){ //�����Ȃ�����Ȃ����B���Ă��Ⴄ�B�r����destroy()���ꂽ�悤���B
			if ( mData ){
				SAFE_DELETE_ARRAY( mData );
			}
			if ( mStream ){
				SAFE_DELETE( mStream );
			}
			mFileSize = mFilePosition = 0;
			mCurrentFile = 0;
		}else{ //�t�@�C��������B���Ⴀ���[�h�𑱂��邩�B
			int rest = mFileSize - mFilePosition;
			//����ǂރT�C�Y��READ_UNIT�ȏ�Ȃ�READ_UNIT�A�ȉ��Ȃ�rest���̂܂�
			int size = ( rest > READ_UNIT ) ? READ_UNIT : rest;
			//�ǂݍ���
			mStream->read( mData + mFilePosition, size );
			mFilePosition += size;
			//�I������Ȃ�Z�b�g���悤�B
			if ( size == rest ){ //����̓ǂݍ��ݗʂƎc�肪������=�I�����
				mCurrentFile->mData = mData;
				mCurrentFile->mSize = mFileSize;
				mData = 0; //���ׂ̉����肽�̂�0��
				mFileSize = mFilePosition = 0;
				SAFE_DELETE( mStream );
				mCurrentFile = 0;
			}
		}
	}else{ //���āA�ǂݍ��ݒ��̂��̂��Ȃ��Ȃ�t�@�C�����J���˂΂Ȃ�Ȃ��Ȃ��B
		for ( It i = mFiles.begin(); i != mFiles.end(); ++i ){
			File* f = *i;
			if ( !f->isReady() ){ //�I����Ă˂��B���[�h�����
				mStream = new ifstream( f->mFilename.c_str(), ifstream::binary );
				mStream->seekg( 0, ifstream::end );
				mFileSize = static_cast< int >( mStream->tellg() );
				mData = new char[ mFileSize ];
				mStream->seekg( 0, ifstream::beg ); //�ŏ��Ɋ����߂��Ă����B
				mFilePosition = 0;
				mCurrentFile = f;
				break; //������B
			}
		}
	}
}
