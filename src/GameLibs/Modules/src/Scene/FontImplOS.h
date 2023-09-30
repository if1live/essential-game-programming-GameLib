#ifndef INCLUDED_GAMELIB_SCENE_FONTIMPLOS_H
#define INCLUDED_GAMELIB_SCENE_FONTIMPLOS_H

#include "Scene/FontImpl.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/WindowCreator/WindowCreator.h"
#include <map> //�����ꎩ��Ɋ�������...

#include <windows.h>
#undef min
#undef max

namespace GameLib{
namespace Scene{
using namespace std;

using namespace GameLib::Graphics;
using namespace GameLib::Math;

namespace { //����

//Windows����t�H���g�f�[�^���擾
class FontImplOS : public Font::Impl{
	struct Char;
public:
	FontImplOS(
	const char* fontName,
	int charHeightHint, //����ɂȂ�Ƃ͌���Ȃ�
	bool bold,
	bool italic,
	bool proportional,
	bool antialias,
	int textureWidth,
	int textureHeight ) :
	mCharHeight( 0 ),
	mCharAscent( 0 ),
	mAntiAliased( antialias ),
	mFontHandle( 0 ),
	mDeviceContextHandle( 0 ),
	mCurrentX( 0 ),
	mCurrentY( 0 ),
	mCurrentLineHeight( 0 ),
	mCurrentPage( 0 ),
	mTextureWidth( textureWidth ),
	mTextureHeight( textureHeight ){
		//�e�N�X�`�������B�Ƃ肠�����ꖇ
		Texture t = Texture::create( mTextureWidth, mTextureHeight, false );
		//�}�b�v�Ɋi�[
		mTextures.insert( make_pair( 0, t ) );

		//�t�H���g����UNICODE�ϊ�
		DWORD pitch = ( proportional ) ? VARIABLE_PITCH : FIXED_PITCH;

		//�t�H���g����
		mFontHandle = CreateFontA(
			charHeightHint,
			0,
			0,
			0,
			( bold ) ? FW_BOLD : FW_DONTCARE,
			( italic ) ? TRUE : FALSE,
			FALSE,
			FALSE,
			SHIFTJIS_CHARSET,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			ANTIALIASED_QUALITY,
			pitch | FF_DONTCARE,
			fontName );
		ASSERT( mFontHandle );
		//HDC����
		mDeviceContextHandle = CreateCompatibleDC( NULL );
		//�e�L�X�g���g���N�X�擾
		//font�Z�b�g
		SelectObject( mDeviceContextHandle, mFontHandle );
		//�����̃��g���N�X����
		TEXTMETRIC tm;
		GetTextMetrics( mDeviceContextHandle, &tm );
		mCharHeight = tm.tmHeight; //���ۂ̒l�ŏ㏑���B
		mCharAscent = tm.tmAscent; //�x�[�X���C���̏�̐��@���K�v
	}
	~FontImplOS(){
		DeleteDC( mDeviceContextHandle );
		mDeviceContextHandle = 0;
	
		DeleteObject( mFontHandle );
		mFontHandle = 0;
	}
	bool getChar(
	Texture* texture,
	Vector2* uvTopLeft,
	Vector2* uvBottomRight,
	int code ){
		//�R�[�h�e�[�u���ɂ���Ύg���B
		CharIt cit = mChars.find( static_cast< unsigned short >( code ) );
		const Char* c = 0;
		if ( cit != mChars.end() ){
			c = &cit->second;
		}else{
			c = createGlyph( code );
		}
		TextureIt tit = mTextures.find( c->mTexturePage );
		ASSERT( tit != mTextures.end() );
		*texture = tit->second;
		float tw = static_cast< float >( texture->width() );
		float th = static_cast< float >( texture->height() );
		float w = static_cast< float >( c->mWidth );
		float h = static_cast< float >( c->mHeight );
		float x = static_cast< float >( c->mX );
		float y = static_cast< float >( c->mY );
		uvTopLeft->set( x / tw, y / th );
		uvBottomRight->set( 
			( x + w ) / tw, 
			( y + h ) / th );
		return true;
	}
	const Char* createGlyph( int code ){
		//font�Z�b�g
		SelectObject( mDeviceContextHandle, mFontHandle );

		GLYPHMETRICS gm; // ���
		MAT2 mat = {
			{ 0, 1 }, 
			{ 0, 0 }, 
			{ 0, 0 }, 
			{ 0, 1 }
		};
		DWORD size;
		const UINT format = ( mAntiAliased ) ? GGO_GRAY8_BITMAP : GGO_BITMAP;
		// �܂��͕K�v�ȃo�b�t�@�T�C�Y���擾
		size = GetGlyphOutlineA(
			mDeviceContextHandle,
			static_cast< UINT >( code ),
			format,
			&gm, 0, NULL, &mat );
		//���@�擾
		int cellW = static_cast< int >( gm.gmCellIncX );
		int cellH = static_cast< int >( mCharHeight ); //IncY�͖𗧂����B
		int glyphW = static_cast< int >( gm.gmBlackBoxX );
		int glyphH = static_cast< int >( gm.gmBlackBoxY );
		int glyphX = static_cast< int >( gm.gmptGlyphOrigin.x );
		int glyphY = static_cast< int >( mCharAscent - gm.gmptGlyphOrigin.y );
		if ( glyphW + glyphX > cellW ){ //���肦�񂪑Ώ�����ق�����܂�
			cellW = glyphW + glyphX;
		}
		if ( glyphH + glyphY > cellH ){ //���肦�񂪑Ώ�����ق�����܂�
			cellH = glyphH + glyphY;
		}
		//�������݃e�N�X�`���擾
		TextureIt tit = mTextures.find( mCurrentPage );
		ASSERT( tit != mTextures.end() );
		Texture* tex = &( tit->second );
		int tw = tex->width();
		int th = tex->height();
		//�����ӂ�܂����I
		if ( mCurrentX + cellW >= tw ){
			mCurrentX = 0;
			mCurrentY += mCurrentLineHeight;
			mCurrentLineHeight = 0;
			//�c���ӂ�܂����I
			if ( mCurrentY + cellH >= th ){
				mCurrentY = 0;
				++mCurrentPage;
				Texture t = Texture::create( mTextureWidth, mTextureHeight, false );
				//�}�b�v�Ɋi�[
				mTextures.insert( make_pair( mCurrentPage, t ) );
				//�e�N�X�`����蒼��
				TextureIt tit = mTextures.find( mCurrentPage );
				ASSERT( tit != mTextures.end() );
				tex = &( tit->second );
			}
		}
		//�e�N�X�`�����b�N(�������b�N)
		unsigned* dst;
		int dstPitch;
		tex->lock( 
			&dst, 
			&dstPitch,
			mCurrentX,
			mCurrentY,
			cellW,
			cellH );

		if ( size == 0 ){ //�T�C�Y������0����
			for ( int y = 0; y < cellH; ++y ){
				for ( int x = 0; x < cellW; ++x ){
					dst[ x ] = 0x00ffffff;
				}
				dst += dstPitch / 4;
			}
		}else{
			// �o�b�t�@���m��
			BYTE* data = NEW BYTE[ size ];
			int srcPitch = ( glyphW + 3 ) & ~( 3 ); //4�o�C�g�A���C��

			// ��x�ڂ̌Ăяo�����{���B�f�[�^�擾�B
			size = GetGlyphOutlineA(
				mDeviceContextHandle,
				static_cast< UINT >( code ),
				format,
				&gm, size, data, &mat );
			ASSERT( size != GDI_ERROR );

			//1bit�̎����s�ւ�����̂ŁA�o�C�g�z��ɃR�s�[
			if ( mAntiAliased ){ //�A���`���������Ă�Ȃ�64->255�K���ϊ�
				for ( DWORD i = 0; i < size; ++i ){
					data[ i ] = data[ i ] * 255 / 64;
				}
			}else{
				BYTE* newData = NEW BYTE[ size * 8 ]; //8�{����Α���邾��
				const BYTE* src = data;
				BYTE* dst = newData;
				int srcPitch = ( ( ( glyphW + 7 ) / 8 ) + 3 ) & ( ~3 );
				int dstPitch = ( glyphW + 3 ) & ( ~3 );
				for ( int y = 0; y < glyphH; ++y ){
					for ( int x = 0; x < glyphW; ++x ){
						int shift = 7 - ( x % 8 );
						int mask = 0x1 << shift;
						int bit = ( src[ x / 8 ] & mask ) >> shift;
						dst[ x ] = static_cast< BYTE >( bit * 255 );
					}
					dst += dstPitch;
					src += srcPitch;
				}
				//���z���delete l|
				SAFE_DELETE_ARRAY( data );
				//�V�z��ɍ����ւ�
				data = newData;
			}
			BYTE* src = data;
			//�㕔�󔒂̓h��Ԃ�
			int y = 0;
			for ( ; y < glyphY; ++y ){
				for ( int x = 0; x < cellW; ++x ){
					dst[ x ] = 0x00ffffff;
				}
				dst += dstPitch / 4;
			}
			//�O���t�{��Z
			for ( ; y < glyphY + glyphH; ++y ){
				LONG x = 0;
				//������
				for ( ; x < glyphX; ++x ){
					dst[ x ] = 0x00ffffff;
				}
				//�O���t�{��
				for ( ; x < glyphX + glyphW; ++x ){
					int a = src[ x - glyphX ];
					dst[ x ] = ( a << 24 ) | 0xffffff;
				}
				//�E����
				for ( ; x < cellW; ++x ){
					dst[ x ] = 0x00ffffff;
				}
				dst += dstPitch / 4;
				src += srcPitch;
			}
			//�����󔒂̓h��Ԃ�
			for ( ; y < cellH; ++y ){
				for ( int x = 0; x < cellW; ++x ){
					dst[ x ] = 0x00ffffff;
				}
				dst += dstPitch / 4;
			}
			SAFE_DELETE_ARRAY( data );
		}
		tex->unlock( &dst, 0 );
		//���X�g�ɒǉ�
		Char c;
		c.mTexturePage = static_cast< unsigned char >( mCurrentPage );
		c.mWidth = static_cast< unsigned char >( cellW );
		c.mHeight = static_cast< unsigned char >( cellH );
		c.mX = static_cast< unsigned short >( mCurrentX );
		c.mY = static_cast< unsigned short >( mCurrentY );
		mCurrentX += cellW; //�������݈ʒu�i�߂�
		if ( mCurrentLineHeight < cellH ){
			mCurrentLineHeight = cellH;
		}

		CharIt it = mChars.insert( make_pair( code, c ) ).first;
		return &( it->second );
	}
	int charHeight() const {
		return mCharHeight;
	}
private:
	void operator=( const FontImplOS& ); //�֎~

	struct Char{
		unsigned char mTexturePage; //�e�N�X�`���y�[�W
		unsigned char mHeight; //�������(�s�N�Z��)
		unsigned char mWidth; //�����(�s�N�Z��)
		unsigned short mX; //���[
		unsigned short mY; //��[
	};
	typedef map< unsigned short, Char > CharMap;
	typedef CharMap::iterator CharIt;
	typedef map< int, Texture > TextureMap;
	typedef TextureMap::iterator TextureIt;

	int mCharHeight;
	int mCharAscent; //TEXTMETRIX����擾
	bool mAntiAliased;
	HFONT mFontHandle;
	HDC mDeviceContextHandle;
	CharMap mChars;
	TextureMap mTextures;
	int mCurrentX; //���̕����̍���X
	int mCurrentY; //���̕����̍���Y
	int mCurrentLineHeight; //���݂̍s�̍ő卂��
	int mCurrentPage; //���ݏ������ݒ��̃e�N�X�`���ԍ�
	const int mTextureWidth;
	const int mTextureHeight;
};

} //namespace {}

} //Scene:
} //GameLib
#endif
