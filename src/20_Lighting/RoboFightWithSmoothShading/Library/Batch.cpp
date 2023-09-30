#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
#include "Library/Batch.h"
#include "Library/VertexBuffer.h"
#include "Library/IndexBuffer.h"
#include "Library/Texture.h"
#include "Library/GraphicsDatabase.h"
#include "Library/Matrix44.h"
#include "Library/Matrix34.h"
#include "Library/Vector3.h"
#include "Library/Vector2.h"

#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
using namespace GameLib;
using namespace GameLib::PseudoXml;

//�֗��֐��Q
namespace {

double clamp( double a, double min, double max ){
	if ( a < min ){
		return min;
	}else if ( a > max ){
		return max;
	}else{
		return a;
	}
}

//���_�P�ʃ��C�e�B���O
unsigned light(
const Vector3& lightVector,
const Vector3& lightColor,
const Vector3& ambient,
const Vector3& diffuseColor,
const Vector3& normal ){
	//�@���ƃ��C�g�x�N�^�̓��ς����B
	double cosine = lightVector.dot( normal );
	if ( cosine < 0.0 ){
		cosine = 0.0; //���͂Ȃ�
	}
	Vector3 c;
	c.x = lightColor.x * diffuseColor.x * cosine + ambient.x;
	c.y = lightColor.y * diffuseColor.y * cosine + ambient.y;
	c.z = lightColor.z * diffuseColor.z * cosine + ambient.z;
	c.x = clamp( c.x, 0.0, 1.0 );
	c.y = clamp( c.y, 0.0, 1.0 );
	c.z = clamp( c.z, 0.0, 1.0 );
	int r = static_cast< int >( c.x * 255.0 + 0.5 );
	int g = static_cast< int >( c.y * 255.0 + 0.5 );
	int b = static_cast< int >( c.z * 255.0 + 0.5 );
	return 0xff000000 | ( r << 16 ) | ( g << 8 ) | b;
}

void computeNormal( Vector3* normals, const VertexBuffer& vb, const IndexBuffer& ib ){
	//�@����0������
	for ( int i = 0; i < vb.size(); ++i ){
		normals[ i ].set( 0.0, 0.0, 0.0 );
	}
	//�v�Z�J�n
	int triangleNumber = ib.size() / 3;	
	for ( int i = 0; i < triangleNumber; ++i ){
		unsigned i0 = ib.index( i * 3 + 0 );
		unsigned i1 = ib.index( i * 3 + 1 );
		unsigned i2 = ib.index( i * 3 + 2 );

		//�@�����v�Z���悤�B
		Vector3 n;
		Vector3 p01, p02;
		p01.setSub( *vb.position( i1 ), *vb.position( i0 ) );
		p02.setSub( *vb.position( i2 ), *vb.position( i0 ) );
		n.setCross( p01, p02 );
		n *= 1.f / n.length(); //������1�ɂ���
		//����𒸓_���Ƃ̖@���ɉ��Z
		normals[ i0 ] += n;
		normals[ i1 ] += n;
		normals[ i2 ] += n;
	}
	//�Ō�ɑS��������1��
	for ( int i = 0; i < vb.size(); ++i ){
		normals[ i ] *= 1.f / normals[ i ].length();
	}
}

} //namespace{}


Batch::Batch( Element& e, const GraphicsDatabase& db ) : 
mVertexBuffer( 0 ),
mIndexBuffer( 0 ),
mTexture( 0 ),
mBlendMode( Framework::BLEND_OPAQUE ){
	//���O���Ȃɂ��𔲂�
	int an = e.attributeNumber();
	for ( int i = 0; i < an; ++i ){
		Attribute a = e.attribute( i );
		string name = a.name();
		string value = a.value();
		if ( name == "name" ){ 
			mName = value;
		}else if ( name == "vertexBuffer" ){
			mVertexBuffer = db.vertexBuffer( value.c_str() );
		}else if ( name == "indexBuffer" ){
			mIndexBuffer = db.indexBuffer( value.c_str() );
		}else if ( name == "texture" ){
			mTexture = db.texture( value.c_str() );
		}else if ( name == "blend" ){
			if ( value == "opaque" ){
				mBlendMode = Framework::BLEND_OPAQUE;
			}else if ( value == "additive" ){
				mBlendMode = Framework::BLEND_ADDITIVE;
			}else if ( value == "linear" ){
				mBlendMode = Framework::BLEND_LINEAR;
			}
		}
	}
	//�@���v�Z
	mNormals = new Vector3[ mVertexBuffer->size() ];
	computeNormal( mNormals, *mVertexBuffer, *mIndexBuffer );
}

Batch::Batch(
const VertexBuffer* vb,
const IndexBuffer* ib,
const ::Texture* tex,
Framework::BlendMode blend ) :
mVertexBuffer( vb ),
mIndexBuffer( ib ),
mTexture( tex ),
mBlendMode( blend ){
}

Batch::~Batch(){
	mVertexBuffer = 0;
	mIndexBuffer = 0;
	mTexture = 0;
	SAFE_DELETE_ARRAY( mNormals );
}

void Batch::draw( 
const Matrix44& pvm, 
const Matrix34& wm, 
const Vector3& lightVector,
const Vector3& lightColor,
const Vector3& ambient,
const Vector3& diffuseColor ) const {
	Framework f = Framework::instance();
	//�e�N�X�`���Z�b�g
	if ( mTexture ){
		mTexture->set();
	}else{
		f.setTexture( 0 ); //��̃e�N�X�`��
	}
	//�u�����h���[�h�Z�b�g
	f.setBlendMode( mBlendMode );
	//�u�����h���[�h�ɂ����Z�o�b�t�@�������݂̃t���O��On,Off
	if ( mBlendMode == Framework::BLEND_OPAQUE ){
		f.enableDepthWrite( true );
	}else{
		f.enableDepthWrite( false );
	}
	//Z�e�X�g�͂���On
	f.enableDepthTest( true );
	//���_�����[���h�ɕϊ�
	int vertexNumber = mVertexBuffer->size();
	Vector3* wv = new Vector3[ vertexNumber ];
	for ( int i = 0;i < vertexNumber; ++i ){
		wm.multiply( &wv[ i ], *mVertexBuffer->position( i ) );
	}
	//���_���ŏI�ϊ�
	double* fv = new double[ vertexNumber * 4 ]; //final vertices
	for ( int i = 0;i < vertexNumber; ++i ){
		pvm.multiply( &fv[ i * 4 ], wv[ i ] );
	}
	//�@�������[���h���W�ɕϊ�����B�{���̓��[���h�s��ł͂����Ȃ��B
	Matrix34 wmNormal = wm; //���[���h�s�񂩂�ړ��𔲂�������
	wmNormal.m03 = wmNormal.m13 = wmNormal.m23 = 0.0;
	//�O�����Ē��_�P�ʂŐF�����߂Ă���
	unsigned* colors = new unsigned[ vertexNumber ];
	for ( int i = 0; i < vertexNumber; ++i ){
		Vector3 wm; //world normal
		wmNormal.multiply( &wm, mNormals[ i ] ); //���[���h���W�ɕϊ�
		wm *= 1.f / wm.length(); //������1�ɂ���
		colors[ i ] = light( lightVector, lightColor, ambient, diffuseColor, wm );
	}
	int triangleNumber = mIndexBuffer->size() / 3;
	for ( int i = 0; i < triangleNumber; ++i ){
		unsigned i0 = mIndexBuffer->index( i * 3 + 0 );
		unsigned i1 = mIndexBuffer->index( i * 3 + 1 );
		unsigned i2 = mIndexBuffer->index( i * 3 + 2 );
		f.drawTriangle3DH(
			&fv[ i0 * 4 ],
			&fv[ i1 * 4 ],
			&fv[ i2 * 4 ],
			&mVertexBuffer->uv( i0 )->x,
			&mVertexBuffer->uv( i1 )->x,
			&mVertexBuffer->uv( i2 )->x,
			colors[ i0 ],
			colors[ i1 ],
			colors[ i2 ] );
	}
	SAFE_DELETE_ARRAY( wv );
	SAFE_DELETE_ARRAY( fv );
	SAFE_DELETE_ARRAY( colors );
}

const string* Batch::name() const {
	return &mName;
}

const IndexBuffer* Batch::indexBuffer() const {
	return mIndexBuffer;
}

const VertexBuffer* Batch::vertexBuffer() const {
	return mVertexBuffer;
}

