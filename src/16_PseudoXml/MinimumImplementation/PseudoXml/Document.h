#ifndef INCLUDED_PSEUDO_XML_DOCUMENT_H
#define INCLUDED_PSEUDO_XML_DOCUMENT_H

namespace PseudoXml{

class Element;

class Document{
public:
	Document(); //��̂��̂����
	Document( const char* filename );
	~Document();
	//�����o��
	void write( const char* filename ) const;
	//���[�g�Q�b�g
	const Element* root() const;
	Element* root(); //���������p
private:
	Element* mRoot;
};

} //namespace PseudoXml

#endif
