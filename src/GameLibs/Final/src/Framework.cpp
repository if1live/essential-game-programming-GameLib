#include "GameLib/GameLib.h"
#include "GameLib/WindowCreator/WindowCreator.h"
#include "GameLib/Input/Manager.h"
#include "GameLib/FileIO/Manager.h"
#include "GameLib/Sound/Manager.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Scene/StringRenderer.h"
#include "GameLib/Scene/Font.h"
#include "GameLib/Threading/Functions.h"
#include "GameLib/Threading/Manager.h"

#include "Framework.h"
#include "DebugScreenImpl.h"

#include <string>
#include <fstream>
using namespace std;

namespace GameLib{

namespace { //����

class Impl{
public:
	Impl() :
	mWidth( 640 ),
	mHeight( 480 ),
	mFullScreen( false ),
	mFullScreenForbidden( false ),
	mDragAndDropEnabled( false ),
	mVSync( true ),
	mAntiAlias( true ),
	mTitle( "A GameLib Framework Application" ),
	mArchiveNames( 0 ),
	mArchiveNumber( 0 ),
	mLoadMode( FileIO::Manager::MODE_DIRECT_FIRST ),
	mExtraThreadNumber( 0 ),
	mPreviousFrameInterval( 0 ),
	mPreviousFrameTime( 0 ),
	mFrameRate( 0 ),
	mIdealFrameInterval( 0 ),
	mEndRequested( false ),
	mStarted( false ){
		//cout������
		cout.begin();
		//�t���[���������Z�b�g
		unsigned t = time();
		for ( int i = 0; i < TIME_HISTORY_SIZE; ++i ){
			mTimeHistory[ i ] = t;
		}
	}
	~Impl(){
		if ( mArchiveNames ){
			SAFE_DELETE_ARRAY( mArchiveNames );
		}
		SAFE_DELETE( gDebugScreenImpl );

		Graphics::Manager::destroy();
		Input::Manager::destroy();
		Sound::Manager::destroy();
		FileIO::Manager::destroy();
		Threading::Manager::destroy();
		//cout�I��
		cout.end();
	}
	void start( void* windowHandle ){
		//�X���b�h�V�X�e��������
		Threading::Manager::create( mExtraThreadNumber );
		//�t�@�C�����[�_������
		if ( mArchiveNumber > 0 ){
			Array< const char* > names( mArchiveNumber );
			for ( int i = 0; i < mArchiveNumber; ++i ){
				names[ i ] = mArchiveNames[ i ].c_str();
			}
			FileIO::Manager::create( &names[ 0 ], mArchiveNumber, mLoadMode );
			//�A�[�J�C�u���z��͕s�v�Ȃ̂Ŕp��
			SAFE_DELETE_ARRAY( mArchiveNames );
			mArchiveNumber = 0;
		}else{
			FileIO::Manager::create();
		}
		//��������
		Sound::Manager::create( windowHandle );
		//���͏�����
		Input::Manager::create( windowHandle );
		//�`�揉����
		Graphics::Manager::create( windowHandle, mWidth, mHeight, mFullScreen, mVSync, mAntiAlias );
		//�����`��
		gDebugScreenImpl = NEW DebugScreenImpl;

		mStarted = true;
	}
	unsigned time() const {
		return WindowCreator().time();
	}
	void preUpdate(){
		unsigned currentTime = time();
		while ( ( currentTime - mPreviousFrameTime ) < mIdealFrameInterval ){
			Threading::sleep( 1 );
			currentTime = time();
		}
		mPreviousFrameTime = currentTime;

		//�t���[�����ԍX�V
		mPreviousFrameInterval = currentTime - mTimeHistory[ TIME_HISTORY_SIZE - 1 ];
		unsigned frameIntervalSum = currentTime - mTimeHistory[ 0 ];
		mFrameRate = TIME_HISTORY_SIZE * 1000 / frameIntervalSum;
		//�����X�V
		for ( int i = 0; i < TIME_HISTORY_SIZE - 1; ++i ){
			mTimeHistory[ i ] = mTimeHistory[ i + 1 ]; 
		}
		mTimeHistory[ TIME_HISTORY_SIZE - 1 ] = currentTime;

		Graphics::Manager().beginDraw();
		float pointerScale;
		Vector2 pointerOffset;
		Graphics::Manager().getPointerModifier( &pointerScale, &pointerOffset );
		Input::Manager().update( pointerScale, pointerOffset );
	}
	void postUpdate(){
		gDebugScreenImpl->draw(); //�����`��

		Graphics::Manager().endDraw();
	}
	int mWidth;
	int mHeight;
	bool mFullScreen;
	bool mFullScreenForbidden;
	bool mDragAndDropEnabled;
	bool mVSync;
	bool mAntiAlias;
	string mTitle;
	string* mArchiveNames;
	int mArchiveNumber;
	FileIO::Manager::AccessMode mLoadMode;
	int mExtraThreadNumber;
	static const int TIME_HISTORY_SIZE = 60;
	unsigned mTimeHistory[ TIME_HISTORY_SIZE ];
	int mPreviousFrameInterval;
	unsigned mPreviousFrameTime;
	int mFrameRate;
	unsigned mIdealFrameInterval;
	bool mEndRequested;
	bool mStarted;
};

Impl* gImpl = 0;

} //namespace{}

Framework::Framework(){
	//�ʃX���b�h����̌Ăяo���͋����Ȃ�
	ASSERT( WindowCreator::isMainThread() && "you must call from MAIN thread" );
}

void Framework::create(){
	ASSERT( !gImpl );
	gImpl = NEW Impl();
}

void Framework::destroy(){
	ASSERT( gImpl );
	SAFE_DELETE( gImpl );
}

Framework Framework::instance(){
	return Framework();
}

void Framework::start( void* windowHandle ){
	gImpl->start( windowHandle );
}

void Framework::preUpdate(){
	gImpl->preUpdate();
}

void Framework::postUpdate(){
	gImpl->postUpdate();
}

int Framework::width() const {
	return gImpl->mWidth;
}

int Framework::height() const {
	return gImpl->mHeight;
}

const char* Framework::title() const {
	return gImpl->mTitle.c_str();
}

bool Framework::isFullScreen() const {
	return gImpl->mFullScreen;
}

bool Framework::isFullScreenForbidden() const {
	return gImpl->mFullScreenForbidden;
}

bool Framework::isDragAndDropEnabled() const {
	return gImpl->mDragAndDropEnabled;
}

void Framework::requestEnd(){
	gImpl->mEndRequested = true;
}

bool Framework::isEndRequested() const {
	return gImpl->mEndRequested;
}

unsigned Framework::time() const {
	return gImpl->time();
}

int Framework::previousFrameInterval() const {
	return gImpl->mPreviousFrameInterval;
}

int Framework::frameRate() const {
	return gImpl->mFrameRate;
}

void Framework::setFrameRate( int fr ){
	gImpl->mIdealFrameInterval = 1000 / fr;
}

void Framework::enableFullScreen( bool f ){ 
	if ( gImpl->mFullScreenForbidden ){ //�֎~����Ă���
		return;
	}
	WindowCreator wc;
	Graphics::Manager gm;
	if ( gImpl->mStarted ){
		//���r�ύX
		if ( gImpl->mFullScreen != f ){
			//�㗬��
			wc.enableFullScreen( f );
			//������
			gm.enableFullScreen( f );
		}
	}
	gImpl->mFullScreen = f;
}

//Configuration
void Framework::Configuration::setWidth( int width ){
	gImpl->mWidth = width;
}

void Framework::Configuration::setHeight( int height ){
	gImpl->mHeight = height;
}

void Framework::Configuration::setTitle( const char* title ){
	gImpl->mTitle = title;
}

void Framework::Configuration::setArchiveNumber( int n ){
	ASSERT( !gImpl->mArchiveNames && "you can't call twice." );
	gImpl->mArchiveNumber = n;
	gImpl->mArchiveNames = NEW string[ n ];
}

void Framework::Configuration::setArchiveName( int index, const char* name ){
	ASSERT( gImpl->mArchiveNames && "you must call setArchiveNumber() before this call." );
	ASSERT( index >= 0 && index < gImpl->mArchiveNumber );
	gImpl->mArchiveNames[ index ] = name;
}

void Framework::Configuration::setLoadMode( LoadMode lm ){
	using namespace FileIO;
	Manager::AccessMode am = Manager::MODE_DIRECT_FIRST;
	switch ( lm ){
		case LOAD_ARCHIVE_FIRST: am = Manager::MODE_ARCHIVE_FIRST; break;
		case LOAD_DIRECT_FIRST: am = Manager::MODE_DIRECT_FIRST; break;
		case LOAD_ARCHIVE_ONLY: am = Manager::MODE_ARCHIVE_ONLY; break;
		default: ASSERT( false ); break;
	}
	gImpl->mLoadMode = am;
}

void Framework::Configuration::enableFullScreen( bool f ){
	gImpl->mFullScreen = f;
}

void Framework::Configuration::enableVSync( bool f ){
	gImpl->mVSync = f;
}

void Framework::Configuration::enableAntiAlias( bool f ){
	gImpl->mAntiAlias = f;
}

void Framework::Configuration::forbidFullScreen( bool f ){
	gImpl->mFullScreenForbidden = f;
}

void Framework::Configuration::enableDragAndDrop( bool f ){
	gImpl->mDragAndDropEnabled = f;
}

void Framework::Configuration::loadFile( string* out, const char* filename ) const {
	ifstream in( filename, ifstream::binary );
	in.seekg( 0, ifstream::end );
	streamsize size = in.tellg();
	in.seekg( 0, ifstream::beg );
	char* s = new char[ static_cast< unsigned >( size ) + 1 ];
	in.read( s, size );
	s[ size ] = '\0';
	*out = s;
}

const char* Framework::Configuration::commandLineString() const {
	return WindowCreator().commandLineString();
}

void Framework::Configuration::enableThrowWhenHalt( bool f ){
	extern bool gThrowWhenHalt;
	gThrowWhenHalt = f;
}

void Framework::Configuration::setExtraThreadNumber( int n ){
	STRONG_ASSERT( n >= 0 && "extraThreadNumber must be >= 0" );
	gImpl->mExtraThreadNumber = n;
}

//WindowProcedure�̃��[�U�����֐�
void WindowCreator::configure( Configuration* config ){
	Framework::create();
	//���[�U�ݒ�
	Framework f;
	Framework::Configuration fwConfig;
	f.configure( &fwConfig );
	config->setWidth( f.width() );
	config->setHeight( f.height() );
	config->setTitle( f.title() );
	config->enableFullScreen( f.isFullScreen() );
	config->forbidFullScreen( f.isFullScreenForbidden() );
	config->enableDragAndDrop( f.isDragAndDropEnabled() );
}

void WindowCreator::update(){
	WindowCreator wc;
	Graphics::Manager gm;
	Framework f;
	//WindowCreator�̏�Ԏ���ł��낢�낷��
	bool sleepFlag = false;
	//�ŏ������Ă���ΐQ��
	if ( wc.isMinimized() ){
		sleepFlag = true;
	}
	//�f�o�C�X���X�g�����Ă���悤�Ȃ畜�A�����݂邪�A��{�I�ɐQ��B
	if ( !gm.canRender() ){
		gm.restore();
		sleepFlag = true;
	}
	//Window���烂�[�h�ؑփV�O�i��
	bool wcFullScreen = wc.isFullScreen();
	if ( f.isFullScreen() != wcFullScreen ){
		f.enableFullScreen( wcFullScreen );
	}
	if ( !sleepFlag ){
		f.preUpdate();
	}
	//�㗬(�~�{�^���Ƃ�)����I�����߂����Ă��邩�`�F�b�N
	if ( isEndRequested() ){ //���Ă����Framework�ɏI�����߂�`�B
		f.requestEnd();
	}
	if ( sleepFlag ){
		Threading::sleep( 50 ); //50ms�Q��
	}else{
		//���[�U�R�[�h����̗�O�L���b�`���͑����t���O�𑗂�
		try{
			f.update();
		}
		catch ( ... ){
			f.postUpdate();
			Framework::destroy();
			end(); //�㗬�ɂ��m�点��
			throw EXCEPTION_EXIT;
		}
		f.postUpdate();
	}
	//�I������
	if ( f.isEndRequested() ){
		Framework::destroy();
		end(); //�㗬�ɂ��m�点��
	}
}

void WindowCreator::start( void* windowHandle ){
	Framework().start( windowHandle );
}

} //namespace GameLib
