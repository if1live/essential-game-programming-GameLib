#include "XFile/Element.h"
#include "XFile/Template.h"
#include "GameLib/Base/RefString.h"
#include "GameLib/Base/Array.h"
#include "GameLib/Base/AutoPtr.h"
#include "GameLib/Base/OStringStream.h"

namespace XFile{

using namespace GameLib;

//Element
Element::Element(){
}

Element::~Element(){
}

//CompositeArrayElement
CompositeArrayElement::CompositeArrayElement( Array< int >& sizes ) :
mSizes( sizes.size() ){
	int n = 1;
	for ( int i = 0; i < mSizes.size(); ++i ){
		mSizes[ i ] = sizes[ i ];
		n *= sizes[ i ];
	}
	mElements.setSize( n );
}

CompositeArrayElement::~CompositeArrayElement(){}

void CompositeArrayElement::setElement( int i, Element** e ){ //���̓Y�����͈ꎟ���������Y����
	mElements[ i ] = *e;
	*e = 0;
}


void CompositeArrayElement::toString( OStringStream* o, int indent, const RefString& name ) const {
	//�J�n�^�O
	for ( int i = 0; i < indent; ++i ){
		*o << '\t';
	}
	*o << "<Array";
	if ( name.size() > 0 ){
		*o << " name=\"";
		o->write( name.get(), name.size() );
		*o << "\"";
	}
	//�z��T�C�Y�o��
	*o << " size=\"";
	for ( int i = 0; i < mSizes.size(); ++i ){
		*o << mSizes[ i ];
		if ( i < mSizes.size() - 1 ){
			*o << ',';
		}
	}
	*o << "\">" << endl;
	//���g
	RefString nullString; //�z��̊e�v�f�͖��O�������Ȃ�
	for ( int i = 0; i < mElements.size(); ++i ){
		mElements[ i ]->toString( o, indent+1, nullString );
	}
	//�I���^�O
	for ( int i = 0; i < indent; ++i ){
		*o << '\t';
	}
	*o << "</Array>" << endl;
}

PrimitiveArrayElement::PrimitiveArrayElement( Array< int >& sizes, const RefString& type ) :
mSizes( sizes.size() ),
mType( type ){
	int n = 1;
	for ( int i = 0; i < mSizes.size(); ++i ){
		mSizes[ i ] = sizes[ i ];
		n *= sizes[ i ];
	}
	mElements.setSize( n );
}

PrimitiveArrayElement::~PrimitiveArrayElement(){}

void PrimitiveArrayElement::setElement( int i, const RefString& e ){ //���̓Y�����͈ꎟ���������Y����
	mElements[ i ] = e;
}

void PrimitiveArrayElement::toString( OStringStream* o, int indent, const RefString& name ) const {
	//�J�n�^�O
	for ( int i = 0; i < indent; ++i ){
		*o << '\t';
	}
	*o << "<Array";
	if ( name.size() > 0 ){
		*o << " name=\"";
		o->write( name.get(), name.size() );
		*o << "\"";
	}
	//�z��T�C�Y�o��
	*o << " size=\"";
	for ( int i = 0; i < mSizes.size(); ++i ){
		*o << mSizes[ i ];
		if ( i < mSizes.size() - 1 ){
			*o << ',';
		}
	}
	*o << "\">" << endl;
	for ( int i = 0; i < mElements.size(); ++i ){
		for ( int j = 0; j < indent + 1; ++j ){
			*o << '\t';
		}
		*o << "<";
		o->write( mType.get(), mType.size() );
		*o << " value=\"";
		o->write( mElements[ i ].get(), mElements[ i ].size() );
		*o << "\"/>" << endl;
	}
	//�I���^�O
	for ( int i = 0; i < indent; ++i ){
		*o << '\t';
	}
	*o << "</Array>" << endl;
}

//CompositeElement
CompositeElement::CompositeElement( const Template* t, int childNumber, const RefString& name ) : 
mTemplate( t ),
mChildren( childNumber ),
mName( name ){
}

CompositeElement::~CompositeElement(){
	for ( int i = 0; i < mChildren.size(); ++i ){
		if ( mChildren[ i ].mElement ){
			SAFE_DELETE( mChildren[ i ].mElement );
		}
	}
}

void CompositeElement::setChild( int i, const Child& c ){ //���L���͂������Ɉڂ�
	mChildren[ i ] = c;
}

const CompositeElement::Child* CompositeElement::child( int i ){ 
	return &mChildren[ i ]; 
}

int CompositeElement::childNumber() const {
	return mChildren.size();
}

void CompositeElement::toString( OStringStream* o, int indent, const RefString& name ) const {
	//�J�n�^�O
	for ( int i = 0; i < indent; ++i ){
		*o << '\t';
	}
	*o << "<";
	const RefString& type = *mTemplate->getType();
	o->write( type.get(), type.size() );

	if ( mName.size() > 0 ){
		*o << " name=\"";
		o->write( mName.get(), mName.size() );
		*o << "\"";
	}else if ( name.size() > 0 ){
		*o << " name=\"";
		o->write( name.get(), name.size() );
		*o << "\"";
	}
	*o << ">" << endl;
	//���g
	for ( int i = 0; i < mChildren.size(); ++i ){
		const Child& c = mChildren[ i ];
		if ( c.mElement ){ //�����^
			RefString nullString;
			const RefString* name = mTemplate->getChildName( i );
			if ( !name ){ //�g�������͖����Ȃ̂ł��̏��u���K�v
				name = &nullString;
			}
			c.mElement->toString( o, indent+1, *name );
		}else{ //�v���~�e�B�u
			for ( int j = 0; j < indent + 1; ++j ){
				*o << '\t';
			}
			*o << "<";
			const RefString& childType = *mTemplate->getChildType( i );
			o->write( childType.get(), childType.size() );
			*o << " name=\"";
			const RefString& childName = *mTemplate->getChildName( i );
			o->write( childName.get(), childName.size() );
			*o << "\"";
			*o << " value=\"";
			o->write( c.mPrimitive.get(), c.mPrimitive.size() );
			*o << "\"/>" << endl;
		}
	}
	//�I���^�O
	for ( int i = 0; i < indent; ++i ){
		*o << '\t';
	}
	*o << "</";
	o->write( type.get(), type.size() );
	*o << ">" << endl;
}

ReferenceElement::ReferenceElement( const RefString& name ) : mName( name ){
}

ReferenceElement::~ReferenceElement(){
}

void ReferenceElement::toString( OStringStream* o, int indent, const RefString& ) const {
	//�J�n�^�O
	for ( int i = 0; i < indent; ++i ){
		*o << '\t';
	}
	*o << "<Reference name=\"";
	o->write( mName.get(), mName.size() );
	*o << "\"/>";
}

} //namespace XFile

