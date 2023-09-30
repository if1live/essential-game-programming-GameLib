#include "PseudoXml/ElementImpl.h"
#include "PseudoXml/AttributeImpl.h"

#include "GameLib/PseudoXml/Document.h"

namespace GameLib{
namespace PseudoXml{

Document::Impl::Impl() : 
mStringPool( 1024 ), //�Ƃ肠����1KB�Â�
mElementPool( 16 ), //16�Â�
mAttributePool( 16 ), //16�Â�
mRoot( 0 ){
	mRoot = allocateElement( 1 );
	mRoot->setDocument( this );
}

Document::Impl::~Impl(){
}

bool Document::Impl::isReady(){
	bool r = false;
	if ( mRoot->name() ){ //���O��NULL�łȂ��Ȃ�build�ς݁B
		r = true;
	}else if ( mFile ){ //���[�h��
		if ( mFile.isFinished() ){ //���[�h�I�����
			build( mFile.data(), mFile.size() );
			mFile.release();
			r = true;
		}
	}else{
		HALT( "IT MUST BE BUG!" ); //���[�g�v�f���Ȃ�
	}
	return r;
}

void Document::Impl::build( const char* data, int size ){
	if ( RefString( data, 4 ) == "PXML" ){ //�o�C�i�����I
		IBinaryStream is( data, size );
		is.setPosition( 4 );
		//�v�[���ɕ�������Ԃ�����
		int stringBufferOffset = is.readInt();
		int stringBufferSize = size - stringBufferOffset;
		is.setPosition( stringBufferOffset );
		char* stringBuffer = mStringPool.allocate( stringBufferSize );
		is.read( stringBuffer, stringBufferSize );
		is.setPosition( 8 );
		mRoot->setName( "" );
		mRoot->setChildNumber( is.readInt() );
		for ( int i = 0; i < mRoot->childNumber(); ++i ){
			mRoot->child( i )->build( &is, stringBuffer );
		}
	}else{ //�e�L�X�g��
		const char* begin = data;
		const char* end = begin + size;
		Tag tag( "<ROOT>" );
		mRoot->build( &tag, &begin, end );
	}
}

//�P���Ɉ�ÂĂ�ōs���B
void Document::Impl::convertToString( string* out ) const {
	OStringStream oss;
	int childN = mRoot->childNumber();
	for ( int i = 0; i < childN; ++i ){
		const Element::Impl* e = mRoot->child( i );
		if ( e->name() ){ //���O��������̂����o���Ȃ�
			e->convertToString( &oss, 0 );
		}
	}
	Array< char > t;
	oss.get( &t );
	*out = &t[ 0 ];
}

//�P���Ɉ�ÂĂ�ōs���B
void Document::Impl::convertToBinary( Array< char >* out ) const {
	int childN = mRoot->childNumber();
	OStringStream binaryOut;
	OStringStream stringBuffer;
	map< RefString, int > stringMap;
	binaryOut.write( "PXML", 4  ); //�}�W�b�N
	binaryOut.write( 0 ); //�X�g�����O�o�b�t�@�I�t�Z�b�g�̃_�~�[
	binaryOut.write( childN ); //�G�������g��
	for ( int i = 0; i < childN; ++i ){
		const Element::Impl* e = mRoot->child( i );
		if ( e->name() ){ //���O��������̂����o���Ȃ�
			e->convertToBinary( &binaryOut, &stringBuffer, &stringMap );
		}
	}
	int stringBufferOffset = binaryOut.size();
	//�X�g�����O�o�b�t�@���}�[�W
	stringBuffer.get( out );
	binaryOut.write( &( *out )[ 0 ], out->size() );
	out->clear();

	binaryOut.get( out );
	//�X�g�����O�o�b�t�@�I�t�Z�b�g�̖{��������
	const char* p = reinterpret_cast< const char* >( &stringBufferOffset );
	for ( int i = 0; i < 4; ++i ){
		( *out )[ 4 + i ] = p[ i ];
	}
}

Element::Impl* Document::Impl::root(){
	return mRoot;
}

Element::Impl* Document::Impl::allocateElement( int n ){
	return mElementPool.allocate( n );
}

Attribute::Impl* Document::Impl::allocateAttribute( int n ){
	return mAttributePool.allocate( n );
}

} //namespace PseudoXml
} //namespace GameLib

