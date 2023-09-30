#include "GameLib/GameLib.h"
#include "Library/GraphicsDatabase.h"
#include "Library/VertexBuffer.h"
#include "Library/IndexBuffer.h"
#include "Library/Texture.h"
#include "Library/Batch.h"
#include "Library/Model.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
#include "GameLib/PseudoXml/Document.h"
using namespace GameLib::PseudoXml; //���Ԃ��ĂȂ���using�����Ⴆ

GraphicsDatabase::GraphicsDatabase( Element& e ) : 
mVertexBuffers( 0 ),
mIndexBuffers( 0 ),
mTextures( 0 ),
mBatches( 0 ),
mVertexBufferNumber( 0 ),
mIndexBufferNumber( 0 ),
mTextureNumber( 0 ),
mBatchNumber( 0 ){
	createFromElement( e );
}

GraphicsDatabase::GraphicsDatabase( const char* filename ) : 
mVertexBuffers( 0 ),
mIndexBuffers( 0 ),
mTextures( 0 ),
mBatches( 0 ),
mVertexBufferNumber( 0 ),
mIndexBufferNumber( 0 ),
mTextureNumber( 0 ),
mBatchNumber( 0 ){
	Document document = Document::create( filename );
	while ( !document.isReady() ){
		; //���[�h�҂�
	}
	Element root = document.root();
	createFromElement( root );
}

GraphicsDatabase::~GraphicsDatabase(){
	//�܂��o�b�`����B�ˑ����Ă�����̂�������B�ˑ�����Ă�����̂͌�B
	if ( mBatches ){
		for ( int i = 0; i < mBatchNumber; ++i ){
			SAFE_DELETE( mBatches[ i ] );
		}
		SAFE_DELETE_ARRAY( mBatches );
	}
	//�e�N�X�`��
	if ( mTextures ){
		for ( int i = 0; i < mTextureNumber; ++i ){
			SAFE_DELETE( mTextures[ i ] );
		}
		SAFE_DELETE_ARRAY( mTextures );
	}
	//�C���f�N�X�o�b�t�@
	if ( mIndexBuffers ){
		for ( int i = 0; i < mIndexBufferNumber; ++i ){
			SAFE_DELETE( mIndexBuffers[ i ] );
		}
		SAFE_DELETE_ARRAY( mIndexBuffers );
	}
	//���_�o�b�t�@
	if ( mVertexBuffers ){
		for ( int i = 0; i < mVertexBufferNumber; ++i ){
			SAFE_DELETE( mVertexBuffers[ i ] );
		}
		SAFE_DELETE_ARRAY( mVertexBuffers );
	}
}

Model* GraphicsDatabase::createModel( const char* name ) const {
	for ( int i = 0; i < mBatchNumber; ++i ){
		if ( *( mBatches[ i ]->name() ) == name ){
			return new Model( mBatches[ i ] );
		}
	}
	return 0;
}

const VertexBuffer* GraphicsDatabase::vertexBuffer( const char* name ) const {
	for ( int i = 0; i < mVertexBufferNumber; ++i ){
		if ( * ( mVertexBuffers[ i ]->name() ) == name ){
			return mVertexBuffers[ i ];
		}
	}
	return 0;
}

const IndexBuffer* GraphicsDatabase::indexBuffer( const char* name ) const {
	for ( int i = 0; i < mIndexBufferNumber; ++i ){
		if ( *( mIndexBuffers[ i ]->name() ) == name ){
			return mIndexBuffers[ i ];
		}
	}
	return 0;
}

const Texture* GraphicsDatabase::texture( const char* name ) const {
	for ( int i = 0; i < mTextureNumber; ++i ){
		if ( *( mTextures[ i ]->name() ) == name ){
			return mTextures[ i ];
		}
	}
	return 0;
}

const Batch* GraphicsDatabase::batch( const char* name ) const {
	for ( int i = 0; i < mBatchNumber; ++i ){
		if ( *( mBatches[ i ]->name() ) == name ){
			return mBatches[ i ];
		}
	}
	return 0;
}

void GraphicsDatabase::createFromElement( Element& e ){
	int n = e.childNumber();
	//�܂����𐔂���
	for ( int i = 0; i < n; ++i ){
		Element child = e.child( i );
		string name = child.name();
		if ( name == "VertexBuffer" ){
			++mVertexBufferNumber;
		}else if ( name == "IndexBuffer" ){
			++mIndexBufferNumber;
		}else if ( name == "Texture" ){
			++mTextureNumber;
		}else if ( name == "Batch" ){
			++mBatchNumber;
		}
	}
	//�z��m��
	if ( mVertexBufferNumber > 0 ){
		mVertexBuffers = new VertexBuffer*[ mVertexBufferNumber ];
	}
	if ( mIndexBufferNumber > 0 ){
		mIndexBuffers = new IndexBuffer*[ mIndexBufferNumber ];
	}
	if ( mTextureNumber > 0 ){
		mTextures = new Texture*[ mTextureNumber ];
	}
	if ( mBatchNumber > 0 ){
		mBatches = new Batch*[ mBatchNumber ];
	}
	//�����ڂɏ�������ł��邩
	int vertexBufferPos = 0;
	int indexBufferPos = 0;
	int texturePos = 0;
	int batchPos = 0;

	//���ɂ��ˑ����Ă��Ȃ��O�����
	for ( int i = 0; i < n; ++i ){
		Element child = e.child( i );
		string name = child.name();
		if ( name == "VertexBuffer" ){
			mVertexBuffers[ vertexBufferPos ] = new VertexBuffer( child );
			++vertexBufferPos;
		}else if ( name == "IndexBuffer" ){
			mIndexBuffers[ indexBufferPos ] = new IndexBuffer( child );
			++indexBufferPos;
		}else if ( name == "Texture" ){
			mTextures[ texturePos ] = new Texture( child );
			++texturePos;
		}
	}
	//Batch�͑��Ɉˑ����Ă���̂Ō�ł��
	for ( int i = 0; i < n; ++i ){
		Element child = e.child( i );
		string name = child.name();
		if ( name == "Batch" ){
			mBatches[ batchPos ] = new Batch( child, *this );
			++batchPos;
		}
	}
}

