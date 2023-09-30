#ifndef INCLUDED_GRAPHICS_DATABASE_H
#define INCLUDED_GRAPHICS_DATABASE_H

namespace PseudoXml{
	class Element;
}

class VertexBuffer;
class IndexBuffer;
class Texture;
class Batch;
class Model;

class GraphicsDatabase{
public:
	//�j�Zxml��Element���琶��
	GraphicsDatabase( const PseudoXml::Element* );
	//�t�@�C�����琶��
	GraphicsDatabase( const char* filename );
	~GraphicsDatabase();
	//�擾�n
	const VertexBuffer* vertexBuffer( const char* name ) const;
	const IndexBuffer* indexBuffer( const char* name ) const;
	const Texture* texture( const char* name ) const;
	//���f������
	Model* createModel( const char* batchName ) const;
private:
	void createFromElement( const PseudoXml::Element* );

	VertexBuffer** mVertexBuffers;
	IndexBuffer** mIndexBuffers;
	Texture** mTextures;
	Batch** mBatches;

	int mVertexBufferNumber;
	int mIndexBufferNumber;
	int mTextureNumber;
	int mBatchNumber;
};

#endif

