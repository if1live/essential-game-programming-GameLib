#include "Library/Texture.h"
#include "GameLib/Graphics/Manager.h"
using namespace GameLib;
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
#include "GameLib/Threading/Functions.h"
using namespace GameLib::PseudoXml;

Texture::Texture( Element& e ){
	//���O���t�@�C�������𔲂�
	int an = e.attributeNumber();
	for ( int i = 0; i < an; ++i ){
		Attribute a = e.attribute( i );
		string name = a.name();
		if ( name == "name" ){
			mName = a.value();
		}else if ( name == "filename" ){
			mTexture = Graphics::Texture::create( a.value() );
			//���[�h�I���܂ő҂�(���������R�[�h)
			while ( !mTexture.isReady() ){
				Threading::sleep( 1 );
			}
		}
	}
}

Texture::Texture( const char* filename ){
	mTexture = Graphics::Texture::create( filename );
	//���[�h�I���܂ő҂�(���������R�[�h)
	while ( !mTexture.isReady() ){
		Threading::sleep( 1 );
	}
}

Texture::~Texture(){
	//�e�N�X�`���͐e��������Ə���ɏ�����
}

void Texture::set() const {
	Graphics::Manager::instance().setTexture( mTexture );
}

const string* Texture::name() const {
	return &mName;
}
