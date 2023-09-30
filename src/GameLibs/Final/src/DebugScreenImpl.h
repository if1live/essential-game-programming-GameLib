#ifndef INCLUDED_GAMELIB_DEBUGSCREENIMPL_H
#define INCLUDED_GAMELIB_DEBUGSCREENIMPL_H

#include "GameLib/Scene/Font.h"
#include "GameLib/Scene/StringRenderer.h"
#include "GameLib/Base/OStringStream.h"
#include "FontTextureGenerated.h"

namespace GameLib{

using namespace GameLib::Scene;

class DebugScreenImpl{
public:
	DebugScreenImpl() : 
	mColor( 0xffffffff ),
	mX( 0 ),
	mY( 0 ),
	mOriginX( 0 ){
		//�����`��
		//�A�X�L�[�����̕\(�A�X�L�[(126-32+1=95����+63������158����
		Array< Scene::Font::BitmapChar > chars( 158 );
		//�A�X�L�[�����[�U
		int pos = 0;
		for ( int i = 0x20; i <= 0x7E; ++i ){
			Scene::Font::BitmapChar& c = chars[ pos ];
			c.mCode = static_cast< unsigned short >( i );
			c.mX = static_cast< unsigned short >( ( i % 16 ) * 8 );
			c.mY = static_cast< unsigned short >( ( i / 16 ) * 16 );
			c.mTexturePage = 0;
			c.mWidth = 8;
			c.mHeight = 16;
			++pos;
		}
		//���p�J�i��
		for ( int i = 0xA1; i <= 0xDF; ++i ){
			Scene::Font::BitmapChar& c = chars[ pos ];
			c.mCode = static_cast< unsigned short >( i );
			c.mX = static_cast< unsigned short >( ( i % 16 ) * 8 );
			c.mY = static_cast< unsigned short >( ( i / 16 ) * 16 );
			c.mTexturePage = 0;
			c.mWidth = 8;
			c.mHeight = 16;
			++pos;
		}

		ASSERT( pos == 158 );
		int textureSize = sizeof( gFontTga );
		const char* fontDataArray[ 1 ];
		fontDataArray[ 0 ] = gFontTga;
		mFont = Scene::Font::create( fontDataArray, &textureSize, 1, &( chars[ 0 ] ), 158 );
		mRenderer = Scene::StringRenderer::create( 2048, 128 ); //����Ȃ���ő���邾�낤�H

		mRenderer.setFont( mFont );
	}
	void add(){
		Array< char > s;
		mBuffer.get( &s );
		mBuffer.clear();
		//�������𐔂��Ȃ߂āA���s���������炻���ň�U�`��
		int begin = 0;
		int count = 0;
		int n = s.size();
		for ( int i = 0; i < n; ++i ){
			if ( s[ i ] == '\n' ){
				s[ i ] = '\0'; //NULL�����ɕϊ����ĕ`��
				if ( i > begin ){
					mRenderer.add( mX * 8, mY * 16, &s[ begin ], mColor );
				}
				begin = i + 1;
				mX = mOriginX;
				mY += 1; //���s
				count = 0;
			}else{
				++count;
			}
		}
		//�Ō�̕`��
		if ( count > 0 ){
			mRenderer.add( mX * 8, mY * 16, &s[ begin ], mColor );
			mX += count;
		}
	}
	void draw(){
		if ( mBuffer.size() > 0 ){ //�c���Ă���Ȃ�f���o����
			add();
		}
		mRenderer.draw();
		//�ϐ����Z�b�g
		mX = mY = mOriginX = 0;
		mColor = 0xffffffff;
	}
	void newLine(){
		mX = mOriginX;
		++mY;
	}
	void setPosition( int x, int y ){
		mX = mOriginX = x;
		mY = y;
	}
	OStringStream mBuffer;
	Font mFont;
	StringRenderer mRenderer;
	unsigned mColor;
	int mX;
	int mY;
	int mOriginX;
};
extern DebugScreenImpl* gDebugScreenImpl;

} //namespace GameLib

#endif