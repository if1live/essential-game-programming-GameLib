#ifndef INCLUDED_GAMELIB_SCENE_BATCHIMPL_H
#define INCLUDED_GAMELIB_SCENE_BATCHIMPL_H

#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
#include "GameLib/Scene/Container.h"
#include "GameLib/Scene/Batch.h"
#include "GameLib/Graphics/VertexBuffer.h"
#include "GameLib/Graphics/IndexBuffer.h"
#include "GameLib/Graphics/Texture.h"
#include "GameLib/Graphics/Enum.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Math/Vector3.h"

namespace GameLib{
namespace Scene{
using namespace GameLib::Graphics;
using namespace GameLib::Math;

class Batch::Impl{
public:
	//���̊֐��̓R���e�i���g���̂ŁA�C���N���[�h���[�v������邽�߂�Batch.cpp�ɂ����Ă���B
	Impl( PseudoXml::ConstElement& e, Container::Impl& );
	~Impl(){
		mName = 0; //�R���e�i�������Ă��镶����Ȃ̂ł����ł͎Q�Ƃ��͂�������
		mContainer = 0;
	}
	void draw() const {
		Graphics::Manager m = Graphics::Manager();
		m.setTexture( mTexture );
		m.setBlendMode( mBlendMode );
		if ( mCullMode != CULL_MAX){
			m.setCullMode( mCullMode );
		}
		m.setDiffuseColor( mDiffuseColor );
		if ( mLighting ){
			m.setLightingMode( LIGHTING_PER_PIXEL ); //�Ƃ肠�������ߑł�
			m.setSpecularColor( mSpecularColor );
			m.setEmissionColor( mEmissionColor );
			m.setSpecularSharpness( mSpecularSharpness );
		}else{
			m.setLightingMode( LIGHTING_NONE );
		}
		m.setTransparency( mTransparency );
		m.setVertexBuffer( mVertexBuffer );
		if ( mIndexBuffer ){
			m.setIndexBuffer( mIndexBuffer );
			m.drawIndexed( mBufferOffset, mPrimitiveNumber, mPrimitiveType );
		}else{
			m.draw( mBufferOffset, mPrimitiveNumber, mPrimitiveType );
		}
	}
	void draw( const Vector3& color, float transparency ) const {
		Graphics::Manager m = Graphics::Manager();
		m.setTexture( mTexture );
		m.setBlendMode( mBlendMode );
		if ( mCullMode != CULL_MAX){
			m.setCullMode( mCullMode );
		}
		Vector3 dc;
		dc.setMul( mDiffuseColor, color );
		m.setDiffuseColor( dc );
		if ( mLighting ){
			m.setLightingMode( LIGHTING_PER_PIXEL ); //�Ƃ肠�������ߑł�
			m.setSpecularColor( mSpecularColor );
			m.setEmissionColor( mEmissionColor );
			m.setSpecularSharpness( mSpecularSharpness );
		}else{
			m.setLightingMode( LIGHTING_NONE );
		}
		m.setTransparency( mTransparency * transparency );
		m.setVertexBuffer( mVertexBuffer );
		if ( mIndexBuffer ){
			m.setIndexBuffer( mIndexBuffer );
			m.drawIndexed( mBufferOffset, mPrimitiveNumber, mPrimitiveType );
		}else{
			m.draw( mBufferOffset, mPrimitiveNumber, mPrimitiveType );
		}
	}
	const char* name() const {
		return mName;
	}
	Container::Impl* container(){
		return mContainer;
	}
private:
	Vector3 mDiffuseColor;
	Vector3 mSpecularColor;
	Vector3 mEmissionColor;
	float mSpecularSharpness;
	float mTransparency;
	int mBufferOffset;
	int mPrimitiveNumber;
	BlendMode mBlendMode;
	CullMode mCullMode;
	PrimitiveType mPrimitiveType;
	bool mLighting;
	VertexBuffer mVertexBuffer;
	IndexBuffer mIndexBuffer;
	Texture mTexture;
	const char* mName;
	Container::Impl* mContainer;
};

} //namespace Scene
} //namespace GameLib

#endif

