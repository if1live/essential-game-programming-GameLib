#include "GameLib/GameLib.h"
#include "Library/IndexBuffer.h"
#include "GameLib/Graphics/Manager.h"
#include "GameLib/Graphics/IndexBuffer.h"
#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
using namespace GameLib::PseudoXml;

IndexBuffer::IndexBuffer( Element& e ){
	//�G���[�`�F�b�N�S�R���Ă܂���
	int n = e.childNumber();
	mSize = n * 3; //�O�p�`���~3
	if ( mSize > 0 ){
		mIndexBuffer = GameLib::Graphics::IndexBuffer::create( mSize );
		unsigned short* indices = mIndexBuffer.lock();
		for ( int i = 0; i < n; ++i ){
			Element triangle = e.child( i );
			Attribute a = triangle.attribute( 0 ); //indices������Ȃ��̂Ō��߂���
			//unsigned short�����֐����Ȃ��̂�int�ɓ���Ă���R�s�[
			int tmp[ 3 ];
			a.getIntValues( tmp, 3 ); //�G���[�`�F�b�N���Ȃ��Ƃ����Ȃ���ł���H�{���́B
			indices[ i * 3 + 0 ] = static_cast< unsigned short >( tmp[ 0 ] );
			indices[ i * 3 + 1 ] = static_cast< unsigned short >( tmp[ 1 ] );
			indices[ i * 3 + 2 ] = static_cast< unsigned short >( tmp[ 2 ] );
		}
		mIndexBuffer.unlock( &indices );
	}
	//���O�𔲂�
	int an = e.attributeNumber();
	for ( int i = 0; i < an; ++i ){
		Attribute a = e.attribute( i );
		string name = a.name();
		if ( name == "name" ){ 
			mName = a.value();
		}
	}
}

IndexBuffer::~IndexBuffer(){
	//�e��������ƃC���f�N�X�o�b�t�@������ɏ�����B
}

int IndexBuffer::size() const {
	return mSize;
}

const string* IndexBuffer::name() const {
	return &mName;
}

void IndexBuffer::set() const {
	GameLib::Graphics::Manager::instance().setIndexBuffer( mIndexBuffer );
}
