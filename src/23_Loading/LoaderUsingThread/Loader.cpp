#include "GameLib/GameLib.h"
#include "GameLib/Threading/Functions.h"
#include "Loader.h"
#include <fstream>
using namespace std;
using namespace GameLib;
using namespace GameLib::Threading;

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

void LoadingThread::operator()(){
	bool end = false;
	while ( !end ){ //�������[�v
		Threading::sleep( 1 ); //������Ƃɂ��΂炭�Q��B�O���O�����̂�h�������B
		Loader::instance()->update( &end );
	}
}

Loader* Loader::mInstance = 0;

Loader::Loader() : mThread( 0 ), mEndRequest( false ){
	mThread = new LoadingThread;
	mLock = Mutex::create();

	//0����
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		mFiles[ i ] = 0;
	}
}

Loader::~Loader(){
	//�X���b�h���~�߂�
	mLock.lock();
	mEndRequest = true;
	mLock.unlock();
	//�X���b�h��delete�B����wait()���ďI����҂�
	SAFE_DELETE( mThread );

	//���X�g�𖕎E
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		if ( mFiles[ i ] ){
			SAFE_DELETE( mFiles[ i ] );
		}
	}
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
	//�󂢂Ă�ꏊ��T��
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		if ( !mFiles[ i ] ){
			*f = new File( filename );
			mLock.lock(); //�����O�ɓ���
			mFiles[ i ] = *f;
			mLock.unlock();
			break;
		}
	}
}

void Loader::destroyFile( File** f ){
	if ( !( *f ) ){ //���ł�0�B��邱�ƂȂ��B
		return;
	}
	//�T��
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		if ( mFiles[ i ] == *f ){
			mLock.lock(); //�����O�ɓ���
			SAFE_DELETE( mFiles[ i ] );
			mLock.unlock();
			*f = 0; //�󂯎��|�C���^��0��
			break;
		}
	}
	ASSERT( !( *f ) && "can't find this pointer in the list. there must be a BUG!" );
}

void Loader::update( bool* endOut ){
	for ( int i = 0; i < MAX_FILE_NUMBER; ++i ){
		string filename; //�t�@�C���������o�����߂ɁB

		mLock.lock();
		if ( mEndRequest ){ //�I���ƌ����Ă���B���̃��[�v�Ŕ����܂��B
			*endOut = true;
		}
		if ( mFiles[ i ] && !mFiles[ i ]->isReady() ){
			filename = mFiles[ i ]->mFilename;
		}
		mLock.unlock(); //���[�h�I�����ǂ����������Ĉ��o��

		if ( filename.size() > 0 ){ //�t�@�C�����������Ă�B���[�h����Ƃ������Ƃ��B
			int size;
			char* data = 0;
			//��U���[�J���̕ϐ��Ƀ��[�h����B��������΃N���e�B�J���Z�N�V�����ɓ��炸�ɍςށB
			ifstream in( filename.c_str(), ifstream::binary );
			in.seekg( 0, ifstream::end );
			size = static_cast< int >( in.tellg() ); 
			in.seekg( 0, ifstream::beg );
			data = new char[ size ];
			in.read( data, size );
			//���[�h���I����Ă���N���e�B�J���Z�N�V�����ɓ���B������ςȂ����ƌ��������\��������B
			mLock.lock();
			if ( mFiles[ i ] ){ //�����Ȃ���������Ȃ��B�`�F�b�N�B
				mFiles[ i ]->mData = data;
				data = 0; //�Ǘ�����n�����̂�����0�ɁB
				mFiles[ i ]->mSize = size;
			}
			mLock.unlock();
			if ( data ){ //�����f�[�^���܂�����Ƃ������Ƃ́A���if�ɓ���Ȃ������Ƃ������Ƃ�����j���B
				//delete�͒x���̂ŃN���e�B�J���Z�N�V�����ɓ��ꂽ���Ȃ������̂��B
				SAFE_DELETE_ARRAY( data );
			}
		}
	}
}
