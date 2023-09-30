#ifndef INCLUDED_GAMELIB_SOUND_WAVEIMPL_H
#define INCLUDED_GAMELIB_SOUND_WAVEIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/Sound/Wave.h"
#include "GameLib/FileIO/Manager.h"
#include "GameLib/FileIO/InFile.h"
#include "GameLib/Base/Impl/ReferenceType.h"
#include "GameLib/Base/RefString.h"
#include <dsound.h>

namespace GameLib{
namespace Sound{

class Wave::Impl : public ReferenceType{
public:
	Impl( const char* filename, IDirectSound8* directSound ) : 
	mDirectSound( directSound ),
	mBuffer( 0 ),
	mIsError( false ){
		mFile = FileIO::InFile::create( filename );
	}
	Impl( 
	const void* data, 
	int bitsPerSample,
	int size, 
	int frequency, 
	int channels,
	IDirectSound8* directSound ) : 
	mDirectSound( directSound ),
	mBuffer( 0 ),
	mIsError( false ){
		createBuffer( bitsPerSample, frequency, channels, size );
		fillBuffer( data, size );
	}
	~Impl(){
		if ( mBuffer ){
			mBuffer->Release();
		}
		mDirectSound = 0;
	}
	void createBuffer( int bitDepth, int frequency, int channels, int dataSize ){
		STRONG_ASSERT( mDirectSound );
		WAVEFORMATEX format;
		ZeroMemory( &format, sizeof( WAVEFORMATEX ) );
		format.wFormatTag = WAVE_FORMAT_PCM;
		format.nChannels = static_cast< WORD >( channels );
		format.nSamplesPerSec = frequency;
		format.wBitsPerSample = static_cast< WORD >( bitDepth );
		format.nBlockAlign = format.nChannels * format.wBitsPerSample / 8;
		format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

		DSBUFFERDESC desc;
		ZeroMemory( &desc, sizeof( DSBUFFERDESC ) );
		desc.dwSize = sizeof( DSBUFFERDESC );
		desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS;
		desc.dwBufferBytes = dataSize;
		desc.lpwfxFormat = &format;

		IDirectSoundBuffer* buffer = 0;
		HRESULT hr = mDirectSound->CreateSoundBuffer( &desc, &buffer, NULL );
		STRONG_ASSERT( hr != DSERR_ALLOCATED && "Sound::Wave::Impl::Impl : DSERR_ALLOCATED" );
		STRONG_ASSERT( hr != DSERR_BADFORMAT && "Sound::Wave::Impl::Impl : DSERR_BADFORMAT" );
		STRONG_ASSERT( hr != DSERR_BUFFERTOOSMALL && "Sound::Wave::Impl::Impl : DSERR_BUFFERTOOSMALL" );
		STRONG_ASSERT( hr != DSERR_CONTROLUNAVAIL && "Sound::Wave::Impl::Impl : DSERR_CONTROLUNAVAIL" );
		STRONG_ASSERT( hr != DSERR_DS8_REQUIRED && "Sound::Wave::Impl::Impl : DSERR_DS8_REQUIRED" );
		STRONG_ASSERT( hr != DSERR_INVALIDCALL && "Sound::Wave::Impl::Impl : DSERR_INVALIDCALL" );
		STRONG_ASSERT( hr != DSERR_INVALIDPARAM && "Sound::Wave::Impl::Impl : DSERR_INVALIDPARAM" );
		STRONG_ASSERT( hr != DSERR_NOAGGREGATION && "Sound::Wave::Impl::Impl : DSERR_NOAGGREGATION" );
		STRONG_ASSERT( hr != DSERR_OUTOFMEMORY && "Sound::Wave::Impl::Impl : DSERR_OUTOFMEMORY" );
		STRONG_ASSERT( hr != DSERR_UNINITIALIZED && "Sound::Wave::Impl::Impl : DSERR_UNINITIALIZED" );
		STRONG_ASSERT( hr != DSERR_UNSUPPORTED && "Sound::Wave::Impl::Impl : DSERR_UNSUPPORTED" );

		hr = buffer->QueryInterface( IID_IDirectSoundBuffer8, ( void** )&mBuffer );
		STRONG_ASSERT( SUCCEEDED( hr ) );
		buffer->Release();
		buffer = 0;
		mDirectSound = 0; //��x�͍��Ȃ��̂ł����ł������߂�
	}
	void fillBuffer( const void* data, int size ){
		//�f�[�^���߂܂�
		void* p0 = 0;
		void* p1 = 0;
		DWORD s0;
		DWORD s1;
		mBuffer->Lock( 0, 0, &p0, &s0, &p1, &s1, DSBLOCK_ENTIREBUFFER );
		ASSERT( !p1 && ( s1 == 0 ) && p0 && ( s0 == static_cast< unsigned >( size ) ) );
		char* dst = static_cast< char* >( p0 );
		const char* src = static_cast< const char* >( data );
		for ( int i = 0; i < size; ++i ){
			dst[ i ] = src[ i ];
		}
		mBuffer->Unlock( p0, s0, p1, s1 );
	}
	bool isReady(){
		if ( mFile ){
			if ( mFile.isFinished() ){
				if ( mFile.isError() ){ //���[�h�I��
					mIsError = true;
				}else{
					int channels = mFile.getShort( 22 );
					int frequency = mFile.getInt( 24 );
					int bitsPerSample = mFile.getShort( 34 );
					const char* data = mFile.data();
					int waveSize = 0;
					int waveStart = 0;
					int pos = 36;
					RefString dataStr( "data" );
					while ( dataStr != RefString( data + pos, 4 ) ){ //����Ȃ����̂������Ă���Ԕ�΂�
						if ( pos >= mFile.size() ){
							mIsError = true;
							break;
						}
						pos += mFile.getInt( pos+4 ); //����Ȃ����̂̃T�C�Y�������ɓ����Ă���
					}
					if ( !mIsError ){
						waveSize = mFile.getInt( pos + 4 );
						waveStart = pos + 8;
						//�f�[�^�T�C�Y���t�@�C���T�C�Y��菬���������ǂ�����B�G���[�ɂ͂��Ȃ��B
						if ( waveSize + waveStart > mFile.size() ){
							waveSize = mFile.size() - waveStart;
						}
						createBuffer( bitsPerSample, frequency, channels, waveSize );
						fillBuffer( mFile.data() + waveStart, waveSize );
					}
					mFile.release();
				}
				return true; //�G���[�ł��I��
			}else{
				return false; //���[�h�I����Ă܂���B
			}
		}else{
			return true; //���������t�@�C�����Ȃ��Ȃ�A���S�ɏI��������A�ŏ�����t�@�C���o�R�łȂ����B
		}
	}
	bool isError() const {
		return mIsError;
	}
	IDirectSoundBuffer8* buffer(){
		return mBuffer;
	}
private:
	IDirectSound8* mDirectSound;
	IDirectSoundBuffer8* mBuffer;
	FileIO::InFile mFile;
	bool mIsError;
};

} //namespace Sound
} //namespace GameLib

#endif
