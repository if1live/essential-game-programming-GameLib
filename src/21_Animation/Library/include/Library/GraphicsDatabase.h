#ifndef INCLUDED_GRAPHICS_DATABASE_H
#define INCLUDED_GRAPHICS_DATABASE_H

namespace GameLib{
	namespace PseudoXml{
		class Element;
	}
}

class VertexBuffer;
class IndexBuffer;
class Texture;
class Batch;
class Model;
class Tree;
class TreeTemplate;
class Animation;

class GraphicsDatabase{
public:
	//�j�Zxml��Element���琶��
	GraphicsDatabase( GameLib::PseudoXml::Element& );
	//�t�@�C�����琶��
	GraphicsDatabase( const char* filename );
	~GraphicsDatabase();
	//�擾�n
	const VertexBuffer* vertexBuffer( const char* name ) const;
	const IndexBuffer* indexBuffer( const char* name ) const;
	const Texture* texture( const char* name ) const;
	const Batch* batch( const char* name ) const;
	const TreeTemplate* tree( const char* name ) const;
	const Animation* animation( const char* name ) const;
	//���f������
	Model* createModel( const char* batchName ) const;
	//�c���[����
	Tree* createTree( const char* treeName ) const;
private:
	void createFromElement( GameLib::PseudoXml::Element& );

	VertexBuffer** mVertexBuffers;
	IndexBuffer** mIndexBuffers;
	Texture** mTextures;
	Batch** mBatches;
	TreeTemplate** mTrees;
	Animation** mAnimations;

	int mVertexBufferNumber;
	int mIndexBufferNumber;
	int mTextureNumber;
	int mBatchNumber;
	int mTreeNumber;
	int mAnimationNumber;
};

#endif

