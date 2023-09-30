#ifndef INCLUDED_BATCH_H
#define INCLUDED_BATCH_H

#include "GameLib/Framework.h"

#include <string>
using namespace std;

namespace GameLib{
	namespace PseudoXml{
		class Element;
	}
}

class VertexBuffer;
class IndexBuffer;
class Texture;
class Matrix44;
class Matrix34;
class GraphicsDatabase;
class Vector3;

class Batch{
public:
	Batch( GameLib::PseudoXml::Element&, const GraphicsDatabase& );
	Batch( 
		const VertexBuffer*,
		const IndexBuffer*,
		const Texture*,
		GameLib::Framework::BlendMode );
	~Batch();
	void draw( 
		const Matrix44& projectionViewMatrix, 
		const Matrix34& worldMatrix,
		const Vector3& lightVector, //���C�g�Ɍ������x�N�^
		const Vector3& lightColor, //���C�g�̐F
		const Vector3& ambient, //����
		const Vector3& diffuseColor ) const; //���m�̐F
	const string* name() const;
	const IndexBuffer* indexBuffer() const;
	const VertexBuffer* vertexBuffer() const;
private:
	const VertexBuffer* mVertexBuffer;
	const IndexBuffer* mIndexBuffer;
	const Texture* mTexture;
	GameLib::Framework::BlendMode mBlendMode;
	string mName;
};

#endif
