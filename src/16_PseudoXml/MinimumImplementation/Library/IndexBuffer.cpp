#include "GameLib/GameLib.h"
#include "Library/IndexBuffer.h"
#include "PseudoXml/Element.h"
#include "PseudoXml/Attribute.h"
using namespace PseudoXml;

IndexBuffer::IndexBuffer( const Element* e ){
	//�G���[�`�F�b�N�S�R���Ă܂���
	int n = e->childNumber();
	mSize = n * 3; //�O�p�`���~3
	if ( mSize > 0 ){
		mIndices = new unsigned[ mSize ];
		for ( int i = 0; i < n; ++i ){
			const Element* triangle = e->child( i );
			const Attribute* a = triangle->attribute( 0 ); //indices������Ȃ��̂Ō��߂���
			//unsigned�����֐����Ȃ��̂�int�ɓ���Ă���R�s�[
			int tmp[ 3 ];
			a->getIntValues( tmp, 3 ); //�G���[�`�F�b�N���Ȃ��Ƃ����Ȃ���ł���H�{���́B
			mIndices[ i * 3 + 0 ] = tmp[ 0 ];
			mIndices[ i * 3 + 1 ] = tmp[ 1 ];
			mIndices[ i * 3 + 2 ] = tmp[ 2 ];
		}
	}
	//���O�𔲂�
	int an = e->attributeNumber();
	for ( int i = 0; i < an; ++i ){
		const Attribute* a = e->attribute( i );
		if ( *( a->name() ) == "name" ){ 
			mName = *( a->value() );
		}
	}
}

IndexBuffer::IndexBuffer( int size ) :
mIndices( 0 ),
mSize( size ){
	mIndices = new unsigned[ mSize ];
}

IndexBuffer::~IndexBuffer(){
	SAFE_DELETE_ARRAY( mIndices );
}

unsigned IndexBuffer::index( int i ) const {
	ASSERT( i >= 0 && i < mSize );
	return mIndices[ i ];
}

void IndexBuffer::setIndex( int i, unsigned v ){
	ASSERT( i >= 0 && i < mSize );
	mIndices[ i ] = v;
}

int IndexBuffer::size() const {
	return mSize;
}

const string* IndexBuffer::name() const {
	return &mName;
}