#ifndef INCLUDED_GAMELIB_SCENE_CONTAINERIMPL_H
#define INCLUDED_GAMELIB_SCENE_CONTAINERIMPL_H

#include "GameLib/GameLib.h"
#include "GameLib/PseudoXml/Document.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/Base/Impl/ReferenceType.h"
#include "GameLib/Base/HashMultiMap.h"
#include "GameLib/Base/Pool.h"
#include "GameLib/Graphics/VertexBuffer.h"
#include "GameLib/Graphics/IndexBuffer.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Scene/Batch.h"
#include "GameLib/Scene/Container.h"

#include "Scene/AnimationImpl.h"
#include "Scene/BatchImpl.h"
#include "Scene/TreeTemplate.h"

namespace GameLib{
namespace Scene{
using namespace PseudoXml;
using namespace Graphics;

class Container::Impl : public ReferenceType{
public:
	Impl( ConstElement e, const char* extraDataPath ){
		ASSERT( RefString( "Container" ) == e.name() );
		if ( extraDataPath ){
			RefString tmp( extraDataPath );
			mExtraDataPath = allocateString( tmp );
		}else{
			mExtraDataPath = allocateString( RefString( ".\\" ) ); //�����B
		}
		createFromElement( e );
	}
	Impl( const char* filename, const char* extraDataPath ){
		if ( extraDataPath ){
			RefString tmp( extraDataPath );
			mExtraDataPath = allocateString( tmp );
		}else{
			string t = filename; //��U�t�@�C����������
			// /��\�ɕϊ��B�Ō�Ɍ������ꏊ���L�^
			int slashPos = -1;
			for ( size_t i = 0; i < t.size(); ++i ){
				if ( t[ i ] == '/' ){
					t[ i ] = '\\';
				}
				if ( t[ i ] == '\\' ){
					slashPos = static_cast< int >( i );
				}
			}
			if ( slashPos == -1 ){	//����-1�Ȃ�Ō��\�𑫂�
				t = ""; //����
			}else{
				t.erase( slashPos + 1, string::npos );
			}
			mExtraDataPath = allocateString( RefString( t.c_str() ) );
		}
		mDocument = Document::create( filename );
	}
	~Impl(){
		//VertexBuffer, IndexBuffer, Texture�͎Q�ƃJ�E���g�ŏ���ɊǗ�����Ă���B�ق����Ă����ėǂ��B
		//�܂����낢��g���Ă���Tree
		for ( int i = 0; i < mTrees.size(); ++i ){
			SAFE_DELETE( mTrees[ i ] );
		}
		//����Batch�������B
		for ( int i = 0; i < mBatches.size(); ++i ){
			SAFE_DELETE( mBatches[ i ] );
		}
		//�Ō�ɃA�j��
		for ( int i = 0; i < mAnimations.size(); ++i ){
			SAFE_DELETE( mAnimations[ i ] );
		}
		mExtraDataPath = 0; //stringPool�̒�������0�ŎQ�Ƃ����������ł���
	}
	Batch::Impl* batch( const char* name ) const {
		int p = mIndices.find( name );
		RefString nameRef( name );
		while ( !mIndices.isEnd( p ) && ( nameRef == *mIndices.key( p ) ) ){
			int i = *mIndices.value( p );
			if ( ( i & 0xffff0000 ) == PREFIX_BATCH ){
				return mBatches[ i & 0xffff ];
			}
			p = mIndices.next( p );
		}
		return 0;
	}
	Batch::Impl* batch( int id ) const {
		return mBatches[ id ];
	}
	Animation::Impl* animation( const char* name ) const {
		int p = mIndices.find( name );
		RefString nameRef( name );
		while ( !mIndices.isEnd( p ) && ( nameRef == *mIndices.key( p ) ) ){
			int i = *mIndices.value( p );
			if ( ( i & 0xffff0000 ) == PREFIX_ANIMATION ){
				return mAnimations[ i & 0xffff ];
			}
			p = mIndices.next( p );
		}
		return 0;
	}
	Animation::Impl* animation( int id ) const {
		return mAnimations[ id ];
	}
	int animationNumber() const {
		return mAnimations.size();
	}
	int batchNumber() const {
		return mBatches.size();
	}
	TreeTemplate* tree( const char* name ) const {
		int p = mIndices.find( name );
		RefString nameRef( name );
		while ( !mIndices.isEnd( p ) && ( nameRef == *mIndices.key( p ) ) ){
			int i = *mIndices.value( p );
			if ( ( i & 0xffff0000 ) == PREFIX_TREE ){
				return mTrees[ i & 0xffff ];
			}
			p = mIndices.next( p );
		}
		return 0;
	}
	TreeTemplate* tree( int id ) const {
		return mTrees[ id ];
	}
	int treeNumber() const {
		return mTrees.size();
	}
	VertexBuffer vertexBuffer( const char* name ) const {
		int p = mIndices.find( name );
		RefString nameRef( name );
		while ( !mIndices.isEnd( p ) && ( nameRef == *mIndices.key( p ) ) ){
			int i = *mIndices.value( p );
			if ( ( i & 0xffff0000 ) == PREFIX_VERTEX_BUFFER ){
				return mVertexBuffers[ i & 0xffff ];
			}
			p = mIndices.next( p );
		}
		return VertexBuffer();
	}
	IndexBuffer indexBuffer( const char* name ) const {
		int p = mIndices.find( name );
		RefString nameRef( name );
		while ( !mIndices.isEnd( p ) && ( nameRef == *mIndices.key( p ) ) ){
			int i = *mIndices.value( p );
			if ( ( i & 0xffff0000 ) == PREFIX_INDEX_BUFFER ){
				return mIndexBuffers[ i & 0xffff ];
			}
			p = mIndices.next( p );
		}
		return IndexBuffer();
	}
	Texture texture( const char* name ) const {
		int p = mIndices.find( name );
		RefString nameRef( name );
		while ( !mIndices.isEnd( p ) && ( nameRef == *mIndices.key( p ) ) ){
			int i = *mIndices.value( p );
			if ( ( i & 0xffff0000 ) == PREFIX_TEXTURE ){
				return mTextures[ i & 0xffff ];
			}
			p = mIndices.next( p );
		}
		return Texture();
	}
	const char* allocateString( const RefString& s ){
		char* p = mStringPool.allocate( s.get(), s.size() + 1 );
		p[ s.size() ] = '\0';
		return p;
	}
	bool isReady(){
		if ( mDocument ){
			if ( mDocument.isReady() ){
				ConstElement e = mDocument.root();
				//�q������l�Ŗ��O��Container�̎��Ɍ����āA�q��n���B
				if ( ( e.childNumber() == 1 ) && ( RefString( "Container" ) != e.name() ) ){
					e = e.child( 0 );
				}
				createFromElement( e );
				e.release(); //��ɔj���B
				mDocument.release();
			}
		}
		if ( !mDocument ){ //���[�h���I����ĂāA�e�N�X�`�����S�����[�h����Ă����true
			for ( int i = 0; i < mTextures.size(); ++i ){
				if ( !mTextures[ i ].isReady() ){
					return false;
				}
			}
			return true;
		}
		return false;
	}
private:
	//Hash�������Ă����Y���������̓Y�����Ȃ̂���\����B����16�r�b�g���Y����
	enum IndexPrefix{
		PREFIX_VERTEX_BUFFER = 0x10000,
		PREFIX_INDEX_BUFFER = 0x20000,
		PREFIX_TEXTURE = 0x30000,
		PREFIX_ANIMATION = 0x40000,
		PREFIX_BATCH = 0x50000,
		PREFIX_TREE = 0x60000,
	};
	void createFromElement( ConstElement e ){
		//�܂����𐔂���
		int vertexBufferN = 0;
		int indexBufferN = 0;
		int textureN = 0;
		int animationN = 0;
		int batchN = 0;
		int treeN = 0;

		int n = e.childNumber();
		for ( int i = 0; i < n; ++i ){
			ConstElement child = e.child( i );
			RefString name( child.name() );
			if ( name == "VertexBuffer" ){
				++vertexBufferN;
			}else if ( name == "IndexBuffer" ){
				++indexBufferN;
			}else if ( name == "Texture" ){
				++textureN;
			}else if ( name == "Animation" ){
				++animationN;
			}else if ( name == "Batch" ){
				++batchN;
			}else if ( name == "Tree" ){
				++treeN;
			}
		}
		//�m��
		mVertexBuffers.setSize( vertexBufferN );
		mIndexBuffers.setSize( indexBufferN );
		mTextures.setSize( textureN );
		mAnimations.setSize( animationN );
		mBatches.setSize( batchN );
		mTrees.setSize( treeN );

		mIndices.setCapacity( n );

		//�쐬�J�n�B��U�������������Ĉʒu�Ɏg��
		vertexBufferN = 0;
		indexBufferN = 0;
		textureN = 0;
		animationN = 0;
		batchN = 0;
		treeN = 0;

		//�ˑ��̂Ȃ����̂����
		for ( int i = 0; i < n; ++i ){
			ConstElement child = e.child( i );
			RefString name( child.name() );
			if ( name == "VertexBuffer" ){
				mVertexBuffers[ vertexBufferN ] = VertexBuffer::create( child );
				mIndices.add( 
					mVertexBuffers[ vertexBufferN ].name(), 
					PREFIX_VERTEX_BUFFER | vertexBufferN );
				++vertexBufferN;
			}else if ( name == "IndexBuffer" ){
				mIndexBuffers[ indexBufferN ] = IndexBuffer::create( child );
				mIndices.add( 
					mIndexBuffers[ indexBufferN ].name(), 
					PREFIX_INDEX_BUFFER | indexBufferN );
				++indexBufferN;
			}else if ( name == "Texture" ){
				mTextures[ textureN ] = Texture::create( child, mExtraDataPath );
				mIndices.add( 
					mTextures[ textureN ].name(), 
					PREFIX_TEXTURE | textureN );
				++textureN;
			}else if ( name == "Animation" ){
				mAnimations[ animationN ] = new Animation::Impl( child, *this );
				mIndices.add( 
					mAnimations[ animationN ]->name(), 
					PREFIX_ANIMATION | animationN );
				++animationN;
			}
		}
		//�ȏ�3��Ɉˑ�����o�b�`�����
		for ( int i = 0; i < n; ++i ){
			ConstElement child = e.child( i );
			RefString name( child.name() );
			if ( name == "Batch" ){
				mBatches[ batchN ] = NEW Batch::Impl( child, *this );
				mIndices.add( 
					mBatches[ batchN ]->name(), 
					PREFIX_BATCH | batchN );
				++batchN;
			}
		}
		//�o�b�`�Ɉˑ�����c���[�����
		for ( int i = 0; i < n; ++i ){
			ConstElement child = e.child( i );
			RefString name( child.name() );
			if ( name == "Tree" ){
				mTrees[ treeN ] = NEW TreeTemplate( child, *this );
				mIndices.add( 
					mTrees[ treeN ]->name(), 
					PREFIX_TREE | treeN );
				++treeN;
			}
		}
	}

	//�t�@�C�����烍�[�h���鎞�p��Document.InFile�ł�����������Ă������̂����A�ʓ|�Ȃ̂ł���B
	Document mDocument;

	//Graphics�͎̂Q�ƌ^�Ŋi�[
	Array< VertexBuffer > mVertexBuffers;
	Array< IndexBuffer > mIndexBuffers;
	Array< Texture > mTextures;
	Array< Animation::Impl* > mAnimations;
	//Scene�̂�Impl��������^���̂��̂�������B
	Array< Batch::Impl* > mBatches;
	Array< TreeTemplate* > mTrees;

	//���O�����p�̃n�b�V���B
	//���̌^�͎Q�ƂŁA���̂����݂������邱�ƂɈˑ����Ă���B���ӁB
	//���^�͔z��̓Y�����B�����Ƃɕʂ̃n�b�V������肽���Ȃ��̂ŁA
	//HashMultiMap�ɂ���int�̏�̃r�b�g�ɂ��ꂪ���Ȃ̂��̈�����Ă���B
	//��������Ȃ��Ɠ������O�̃e�N�X�`���ƃc���[�����������ɍ���킯���B
	HashMultiMap< const char*, int > mIndices;

	//������i�[�p�̃v�[���B�X�̌^��new���Ă�ƒx������ȁB
	Pool< char > mStringPool;

	//�ǉ��f�[�^�����p�X
	const char* mExtraDataPath;
};

} //namespace Scene
} //namespace GameLib

#endif

