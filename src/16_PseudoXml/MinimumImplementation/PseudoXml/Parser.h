#ifndef INCLUDED_PSEUDO_XML_PARSER_H
#define INCLUDED_PSEUDO_XML_PARSER_H

#include <string>
#include <list>
using namespace std;

namespace PseudoXml{

class ElementData;

class Parser{
public:
	Parser( const char* data, int size );
	Element* build();
	~Parser();
private:
	int mMode; //���[�h�ϐ�
	bool mInEndTag; //�I���^�O�ɂ���̂��H
	list< ElementData* > mElementStack; //�G�������g�̍ޗ����l�܂��Ă���
};

} //namespace PseudoXml

#endif