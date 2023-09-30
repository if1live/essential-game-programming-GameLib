#include "GameLib/GameLib.h"
#include "PseudoXml/Document.h"
#include "PseudoXml/Tag.h"
#include "PseudoXml/Element.h"
#include "Library/File.h"

namespace PseudoXml{

Document::Document() : mRoot( 0 ){
	mRoot = new Element( "<ROOT>" );
}

Document::Document( const char* filename ) : mRoot( 0 ){
	//�t�@�C����S���ǂݍ���
	File file( filename );
	//�_�~�[�̍����G�������g�p�Ƀ^�O��p�ӁB���O��<ROOT>
	//<�͖��O�ɂ͎g���Ȃ������Ȃ̂ŁA���ꂪ���ʂł��邱�Ƃ������킩��B
	Tag tag( "<ROOT>" );
	const char* begin = file.data();
	const char* end = begin + file.size();
	mRoot = new Element( &tag, &begin, end );
}

Document::~Document(){
	SAFE_DELETE( mRoot );
}

void Document::write( const char* filename ) const {
	string str;
	//���[�g�v�f�͖������Ă�������
	for ( int i = 0; i < mRoot->childNumber(); ++i ){
		mRoot->child( i )->convertToString( &str, 0 );
	}
	File::write( filename, str.c_str(), static_cast< int >( str.size() ) );
}

const Element* Document::root() const {
	return mRoot;
}

Element* Document::root(){
	return mRoot;
}

} //namespace PseudoXml
