#ifndef INCLUDED_PSEUDO_XML_ATTRIBUTE_H
#define INCLUDED_PSEUDO_XML_ATTRIBUTE_H

#include <string>
using namespace std;

namespace PseudoXml{

class Attribute{
public:
	Attribute();
	Attribute( const char* name, const char* value );
	~Attribute();

	void set( const char* name, const char* value );
	void set( const char* name, int value ); //int�l
	void set( const char* name, double value ); //double�l
	void set( const char* name, const int* values, int number ); //int�z��
	void set( const char* name, const double* values, int number ); //double�z��
	const string* value() const;
	const string* name() const;
	//���s�����0��Ԃ����A���Ƃ���0�������Ă����̂Ƃ͋�ʂ����Ȃ��B
	int getIntValue() const;
	//���s�����0.0��Ԃ����A���Ƃ���0.0�������Ă����̂Ƃ͋�ʂ����Ȃ��B
	double getDoubleValue() const;
	//�߂�l�͎��ۂɓǂ߂��v�f��
	int getIntValues( int* out, int number ) const;
	//�߂�l�͎��ۂɓǂ߂��v�f��
	int getDoubleValues( double* out, int number ) const;
private:
	string mName;
	string mValue;
};

} //namespace PseudoXml

#endif
