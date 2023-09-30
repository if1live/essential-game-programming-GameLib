#include "GameLib/GameLib.h"
#include "GameLib/Scene/StringRenderer.h"
#include "GameLib/Scene/Font.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Matrix44.h"
#include "GameLib/Scene/PrimitiveRenderer.h"
#include "GameLib/Base/Stack.h"
#include "GameLib/Base/Array.h"
#include "GameLib/Base/Impl/ReferenceType.h"
#include <algorithm>
#include <string>
using namespace std;

namespace GameLib{
namespace Scene{

using namespace GameLib::Math;
using namespace GameLib::Graphics;

//�\�[�g�p�֗��N���X
template< class T > struct PointerLess{
	bool operator()( const T* a, const T* b ){
		return ( *a < *b );
	}
};

class StringRenderer::Impl : public ReferenceType{
public:
	Impl( int charCapacity, int requestCapacity ) :
	mCharCapacity( charCapacity ),
	mRequests( requestCapacity ){ //���N�G�X�g�m��
		//TODO:�X�e�[�g�ύX���߂̍ő吔�͂Ƃ肠�������������B�ߏ肷����B
		mPrimitiveRenderer = PrimitiveRenderer::create( charCapacity * 2, charCapacity + 4 );
		//draw���ĂԂ̂ɑΏ����邽�߂̕ϐ�
		mPreviousFrameId = Manager().frameId() - 1; //�߂��������l��
	}
	~Impl(){
	}
	void add( const Vector2& p, const char* s, unsigned c, bool wrap, float depth ){
		STRONG_ASSERT( mCurrentFont && "Font hasn't been set." );
		mRequests.push(); //��v�b�V��
		Request& r = *mRequests.get();
		r.mPosition = p;
		r.mDepth = depth;
		r.mString = s;
		r.mColor = c;
		r.mFont = mCurrentFont;
		r.mWrap = wrap;
	}
	void draw(){
		//�t���[��ID�`�F�b�N
		unsigned fid = Manager().frameId();
		STRONG_ASSERT( mPreviousFrameId != fid && "StringRenderer::draw() : you can't draw() twice in a frame!" );
		mPreviousFrameId = fid;

		if ( mRequests.size() == 0 ){ //��邱�ƂȂ�
			return;
		}
		//TriangleRenderer�ɕW���X�e�[�g�𔭍s
		mPrimitiveRenderer.setBlendMode( Graphics::BLEND_LINEAR );
		mPrimitiveRenderer.setCullMode( Graphics::CULL_NONE );
		mPrimitiveRenderer.enableDepthTest( false );
		mPrimitiveRenderer.enableDepthWrite( false );
		//���݂̃r���[�|�[�g���擾����float��
		Vector2 screenSize;
		int tw, th;
		Graphics::Manager().getViewport( 0, 0, &tw, &th );
		screenSize.set( 
			static_cast< float >( tw ),
			static_cast< float >( th ) );

		//�����o�b�t�@��p��
		Array< Char > chars( mCharCapacity );
		int charPosition = 0;
		//���N�G�X�g�����o���ď����B
		while ( mRequests.size() > 0 ){
			Request& r = *mRequests.get();
			Vector2 pos = r.mPosition;
			float charHeight = static_cast< float >( r.mFont.charHeight() );
			//�ꕶ���Â��o���Bunsigned char�ɂ���K�v����
			int n = static_cast< int >( r.mString.size() );
			const unsigned char* s = reinterpret_cast< const unsigned char* >( r.mString.c_str() );
			for ( int i = 0; i < n; ++i ){
				//Char����
				int code = s[ i ];
				//ShiftJis�͈͂��H
				if ( ( code >= 0x81 && code <= 0x9F ) || ( code >= 0xE0 && code <= 0xFC ) ){
					if ( ( i + 1 < n ) ){
						code <<= 8;
						code |= s[ i + 1 ]; //2�o�C�g�R�[�h
						++i;
					}
				}
				if ( code == '\n' ){ //���s�����ł��ȁB
					pos.x = r.mPosition.x;
					pos.y += charHeight;
					continue;
				}
				//�����Q�b�g
				Char& c = chars[ charPosition ];
				bool found = r.mFont.getChar( 
					&c.mTexture,
					&c.mUv0,
					&c.mUv1,
					code );
				if ( found ){ //����������
					//���݈ʒu�Ƀs�N�Z���������Ă����B
					c.mSize.setSub( c.mUv1, c.mUv0 );
					//�e�N�X�`�����畝�ƍ�����������āA�s�N�Z���ɒ���������߂�B
					c.mSize.x *= static_cast< float >( c.mTexture.width() );
					c.mSize.y *= static_cast< float >( c.mTexture.height() );
				}else{ //�����Ȃ������B�������o��
					c.mSize.set( charHeight, charHeight ); //�����`
				}
				//�J�����O�Ɛ܂�Ԃ��v�Z
				//�܂�Ԃ��A���̎��͂�����Ƃł��͂ݏo���Ȃ�܂�Ԃ��B
				if ( r.mWrap ){
					if ( pos.x + c.mSize.x > screenSize.x ){
						pos.x = r.mPosition.x;
						pos.y += charHeight;
					}
				}else{ //�܂�Ԃ��Ȃ��Ȃ犮�S�ɂ��ӂꂽ���ɐ؂�̂Ă�
					if ( pos.x > screenSize.x ){
						break;
					}
				}
				//�c�����S�ɂ��ӂꂽ�ꍇ�̂ݏI���B
				if ( pos.y > screenSize.y ){
					break;
				}
				//�G���g���ǉ�
				c.mColor = r.mColor;
				c.mPosition.set( pos.x, pos.y, r.mDepth );
				++charPosition;
				//��������
				pos.x += c.mSize.x;
			}
			mRequests.pop();
		}
		if ( charPosition == 0 ){ //�������Ȃ�
			return;
		}
		//�����㕶���o�b�t�@��Z�ƃe�N�X�`���Ń\�[�g
		Array< Char* > charPointers( charPosition );
		for ( int i = 0; i < charPosition; ++i ){
			charPointers[ i ] = &chars[ i ];
		}
		sort( &charPointers[ 0 ], &charPointers[ 0 ] + charPosition, PointerLess< Char >() );
		//�\�[�g���ꂽ���ɕ����o�b�t�@��TriangleBuffer��add
		Texture lastTexture;
		for ( int i = 0; i < charPosition; ++i ){
			Char& c = *charPointers[ i ];
			//�e�N�X�`�����O�ƈႦ�΃Z�b�g
			if ( lastTexture != c.mTexture ){
				mPrimitiveRenderer.setTexture( c.mTexture );
				lastTexture = c.mTexture;
			}
			//���_������Ďl�p�`��`��
			Vector2 p[ 2 ];
			p[ 0 ].set( c.mPosition.x, c.mPosition.y ); //����_
			p[ 1 ].set( c.mPosition.x + c.mSize.x, c.mPosition.y + c.mSize.y ); //�E��
			//�`��
			if ( c.mTexture ){ //�e�N�X�`�������
				mPrimitiveRenderer.addRectangle( 
					p[ 0 ], p[ 1 ],
					c.mUv0, c.mUv1,
					c.mColor );
			}else{ //�˂���B�����o������
				mPrimitiveRenderer.addRectangle( 
					p[ 0 ], p[ 1 ],
					c.mColor );
			}
		}
		//�S���I������̂�TriangleRenderer��`��
		mPrimitiveRenderer.draw();
	}
	void setFont( Font& font ){
		mCurrentFont = font;
	}
private:
	struct Request{
		Vector2 mPosition;
		float mDepth;
		string mString;
		unsigned mColor;
		Font mFont;
		bool mWrap;
	};
	//�����\����
	struct Char{
		//Z�ƃe�N�X�`���Ŕ�r�BZ�͉�����
		bool operator<( const Char& a ) const {
			if ( mPosition.z > a.mPosition.z ){
				return true;
			}else if ( mPosition.z < a.mPosition.z ){
				return false;
			}else if ( mTexture < a.mTexture ){
				return true;
			}else{
				return false;
			}
		}
		Vector3 mPosition; //����_
		Vector2 mUv0; //����UV
		Vector2 mUv1; //�E��UV
		Vector2 mSize;
		Texture mTexture; //�e�N�X�`��
		unsigned mColor;
	};
	int mCharCapacity;
	Stack< Request > mRequests;
	PrimitiveRenderer mPrimitiveRenderer;
	Font mCurrentFont;
	unsigned mPreviousFrameId;
};

StringRenderer StringRenderer::create( int charCapacity, int requestCapacity ){
	StringRenderer r;
	r.mImpl = NEW Impl( charCapacity, requestCapacity );
	return r;
}

void StringRenderer::add( int x, int y, const char* s, unsigned c, bool wrap, float z ){
	Vector2 p( 
		static_cast< float >( x ),
		static_cast< float >( y ) );
	mImpl->add( p, s, c, wrap, z );
}

void StringRenderer::add( const Vector2& p, const char* s, unsigned c, bool wrap, float z ){
	mImpl->add( p, s, c, wrap, z );
}

void StringRenderer::draw(){
	mImpl->draw();
}

void StringRenderer::setFont( Font& font ){
	mImpl->setFont( font );
}

#define TYPE StringRenderer
#include "GameLib/Base/Impl/ReferenceTypeTemplate.h"

} //namespace Scene
} //namespace GameLib
