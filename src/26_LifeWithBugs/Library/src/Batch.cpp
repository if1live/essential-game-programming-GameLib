#include "GameLib/GameLib.h"
#include "GameLib/Framework.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Math/Vector2.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Matrix44.h"
using namespace GameLib;
#include "Library/Batch.h"
#include "Library/VertexBuffer.h"
#include "Library/IndexBuffer.h"
#include "Library/Texture.h"
#include "Library/GraphicsDatabase.h"

#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
using namespace GameLib::PseudoXml;

Batch::Batch( Element& e, const GraphicsDatabase& db ) : 
mBlendMode( Graphics::BLEND_OPAQUE ),
mDiffuseColor( 1.f, 1.f, 1.f ),
mSpecularColor( 1.f, 1.f, 1.f ),
mTransparency( 1.f ),
mSpecularSharpness( 1.f ){
	//���O���Ȃɂ��𔲂�
	int an = e.attributeNumber();
	for ( int i = 0; i < an; ++i ){
		Attribute a = e.attribute( i );
		string name = a.name();
		string value = a.value();
		if ( name == "name" ){ 
			mName = value;
		}else if ( name == "vertexBuffer" ){
			mVertexBuffer = db.getVertexBuffer( value.c_str() );
		}else if ( name == "indexBuffer" ){
			mIndexBuffer = db.getIndexBuffer( value.c_str() );
		}else if ( name == "texture" ){
			mTexture = db.getTexture( value.c_str() );
		}else if ( name == "diffuseColor" ){
			a.getFloatValues( &mDiffuseColor.x, 3 );
		}else if ( name == "specularColor" ){
			a.getFloatValues( &mSpecularColor.x, 3 );
		}else if ( name == "specularSharpness" ){
			mSpecularSharpness = a.getFloatValue();
		}else if ( name == "transparency" ){
			mTransparency = a.getFloatValue();
		}else if ( name == "blend" ){
			if ( value == "opaque" ){
				mBlendMode = Graphics::BLEND_OPAQUE;
			}else if ( value == "additive" ){
				mBlendMode = Graphics::BLEND_ADDITIVE;
			}else if ( value == "linear" ){
				mBlendMode = Graphics::BLEND_LINEAR;
			}
		}
	}
}

//�������Ȃ��Ă���
Batch::~Batch(){
}

void Batch::draw() const {
	Graphics::Manager m = Graphics::Manager::instance();
	//�e�N�X�`���Z�b�g
	if ( mTexture ){
		mTexture->set();
	}else{
		m.setTexture( 0 ); //��̃e�N�X�`��
	}
	//�u�����h���[�h�Z�b�g
	m.setBlendMode( mBlendMode );
	//�u�����h���[�h�ɂ����Z�o�b�t�@�������݂̃t���O��On,Off
	if ( mBlendMode == Graphics::BLEND_OPAQUE ){
		m.enableDepthWrite( true );
	}else{
		m.enableDepthWrite( false );
	}
	//Z�e�X�g�͂���On
	m.enableDepthTest( true );
	//�J�����O�͂Ƃ肠����Off
	m.setCullMode( Graphics::CULL_NONE );
	//�F�p�����[�^
	m.setDiffuseColor( mDiffuseColor );
	m.setSpecularColor( mSpecularColor );
	m.setSpecularSharpness( mSpecularSharpness );
	m.setTransparency( mTransparency );

	//�f�[�^�Z�b�g
	mVertexBuffer->set();
	mIndexBuffer->set();
	//�`��J�n
	m.drawIndexed( 0, mIndexBuffer->size() / 3, Graphics::PRIMITIVE_TRIANGLE );
}

const string* Batch::name() const {
	return &mName;
}
