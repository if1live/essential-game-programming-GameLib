#ifndef INCLUDED_GRAPHICS_DATABASE_H
#define INCLUDED_GRAPHICS_DATABASE_H

namespace GameLib{
	namespace PseudoXml{
		class Element;
	}
}
using namespace GameLib; //�ʓ|�Ȃ̂�using�B���܂�悭�Ȃ��B

class VertexBuffer;
class IndexBuffer;
class Texture;
class Batch;
class Model;

class GraphicsDatabase{
public:
	//�j�Zxml��Element���琶��
	GraphicsDatabase( GameLib::PseudoXml::Element& );
	//�t�@�C�����琶��
	GraphicsDatabase( const char* filename );
	~GraphicsDatabase();
	//�擾�n(const�łŕԂ�)
	ConstSharedPtr< VertexBuffer > getVertexBuffer( const char* name ) const;
	ConstSharedPtr< IndexBuffer > getIndexBuffer( const char* name ) const;
	ConstSharedPtr< Texture > getTexture( const char* name ) const;
	ConstSharedPtr< Batch > getBatch( const char* name ) const;
	//���f������
	Model* createModel( const char* batchName ) const;
private:
	void createFromElement( GameLib::PseudoXml::Element& );

	Array< SharedPtr< VertexBuffer > > mVertexBuffers;
	Array< SharedPtr< IndexBuffer > > mIndexBuffers;
	Array< SharedPtr< Texture > > mTextures;
	Array< SharedPtr< Batch > > mBatches;
};

#endif

