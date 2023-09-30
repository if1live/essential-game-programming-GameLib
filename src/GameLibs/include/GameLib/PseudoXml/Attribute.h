#ifndef INCLUDED_GAMELIB_PSEUDOXML_ATTRIBUTE_H
#define INCLUDED_GAMELIB_PSEUDOXML_ATTRIBUTE_H

namespace GameLib{
namespace PseudoXml{

class Attribute{
public:
	///NULL�I�[�łȂ��Ȃǂ��ăT�C�Y�𖾎��I�ɗ^���������B-1������Ώ���ɑ���̂ŁA�Е������^���邱�Ƃ��ł���
	void set( const char* name, int nameSize, const char* value, int valueSize );
	void set( const char* name, const char* value );
	void set( const char* name, int value ); //int�l
	void set( const char* name, float value ); //float�l
	void set( const char* name, double value ); //double�l
	void set( const char* name, const int* values, int number ); //int�z��
	void set( const char* name, const float* values, int number ); //float�z��
	void set( const char* name, const double* values, int number ); //double�z��
	const char* value() const;
	const char* name() const;
	///���s�����0��Ԃ����A���Ƃ���0�������Ă����̂Ƃ͋�ʂ����Ȃ��B
	int getIntValue() const;
	///���s�����0.f��Ԃ����A���Ƃ���0.f�������Ă����̂Ƃ͋�ʂ����Ȃ��B
	float getFloatValue() const;
	///���s�����0.0��Ԃ����A���Ƃ���0.0�������Ă����̂Ƃ͋�ʂ����Ȃ��B
	double getDoubleValue() const;
	///�߂�l�͎��ۂɓǂ߂��v�f��
	int getIntValues( int* out, int number ) const;
	///�߂�l�͎��ۂɓǂ߂��v�f��
	int getFloatValues( float* out, int number ) const;
	///�߂�l�͎��ۂɓǂ߂��v�f��
	int getDoubleValues( double* out, int number ) const;

	//�ȉ����[�U�͈ӎ����Ȃ��Ă���
	Attribute();
	~Attribute();
	Attribute( const Attribute& );
	operator void*() const;
	Attribute& operator=( const Attribute& );
	void release();
	bool operator==( const Attribute& ) const;
	bool operator!=( const Attribute& ) const;

	class Impl;
private:
	friend class Element;
	friend class Tag;

	explicit Attribute( Impl* );
	Impl* mImpl;
};

//const��
class ConstAttribute{
public:
	const char* value() const;
	const char* name() const;
	///���s�����0��Ԃ����A���Ƃ���0�������Ă����̂Ƃ͋�ʂ����Ȃ��B
	int getIntValue() const;
	///���s�����0.f��Ԃ����A���Ƃ���0.f�������Ă����̂Ƃ͋�ʂ����Ȃ��B
	float getFloatValue() const;
	///���s�����0.0��Ԃ����A���Ƃ���0.0�������Ă����̂Ƃ͋�ʂ����Ȃ��B
	double getDoubleValue() const;
	///�߂�l�͎��ۂɓǂ߂��v�f��
	int getIntValues( int* out, int number ) const;
	///�߂�l�͎��ۂɓǂ߂��v�f��
	int getFloatValues( float* out, int number ) const;
	///�߂�l�͎��ۂɓǂ߂��v�f��
	int getDoubleValues( double* out, int number ) const;

	//�ȉ����[�U�͈ӎ����Ȃ��Ă���
	ConstAttribute();
	ConstAttribute( const Attribute& );
	~ConstAttribute();
	operator void*() const;
	ConstAttribute& operator=( const Attribute& );
	void release();
	bool operator==( const Attribute& ) const;
	bool operator!=( const Attribute& ) const;
private:
	Attribute mBase;
};


} //namespace PseudoXml
} //namespace GameLib

#endif
