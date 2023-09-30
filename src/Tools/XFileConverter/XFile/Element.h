#ifndef INCLUDED_XFILE_ELEMENT_H
#define INCLUDED_XFILE_ELEMENT_H

#include "GameLib/Base/RefString.h"
#include "GameLib/Base/Array.h"
#include "GameLib/Base/AutoPtr.h"
#include "GameLib/Base/OStringStream.h"
#include <string>

namespace XFile{

using namespace std;
using namespace GameLib;
class Template;

//�f�[�^�^
class Element{
public:
	Element();
	virtual ~Element();
	virtual void toString( OStringStream*, int indent, const RefString& name ) const = 0;
private:
	void operator=( const Element& ); //�֎~
	Element( const Element& ); //�֎~
};

class CompositeElement : public Element{
public:
	struct Child{
		Child() : mElement( 0 ){}
		Element* mElement;
		RefString mPrimitive;
	};
	CompositeElement( const Template* t, int childNumber, const RefString& name );
	~CompositeElement();
	void setChild( int i, const Child& c );
	const Child* child( int i );
	int childNumber() const;
	void toString( OStringStream*, int indent, const RefString& name ) const;
private:
	const Template* mTemplate;
	Array< Child > mChildren;

	const RefString mName; //�����^�ɂ͖��O�����邱�Ƃ�����B
};

class CompositeArrayElement : public Element{
public:
	CompositeArrayElement( Array< int >& sizes );
	~CompositeArrayElement();
	void setElement( int i, Element** e );

	void toString( OStringStream*, int indent, const RefString& name ) const;
private:
	Array< int > mSizes;
	Array< AutoPtr< Element > > mElements;
};

class PrimitiveArrayElement : public Element{
public:
	PrimitiveArrayElement( Array< int >& sizes, const RefString& typeName );
	~PrimitiveArrayElement();
	void setElement( int i, const RefString& );

	void toString( OStringStream*, int indent, const RefString& name ) const;
private:
	Array< int > mSizes;
	Array< RefString > mElements; //�z�񂪃v���~�e�B�u�^�Ȃ炱�������g���B
	const RefString mType;
};

class ReferenceElement : public Element{
public:
	ReferenceElement( const RefString& name );
	~ReferenceElement();

	void toString( OStringStream*, int indent, const RefString& name ) const;
private:
	const RefString mName; //�����^�ɂ͖��O�����邱�Ƃ�����B
};



} //namespace XFile

#endif
