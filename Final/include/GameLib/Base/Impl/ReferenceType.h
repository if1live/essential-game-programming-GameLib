#ifndef INCLUDED_GAMELIB_BASE_REFERENCETYPE_H
#define INCLUDED_GAMELIB_BASE_REFERENCETYPE_H

//�Q�ƃJ�E���g�����^����鎞�Ɋy�ɂ��邽�߂̊��N���X
namespace GameLib{

class ReferenceType{
public:
	ReferenceType() : mReferenceCount( 1 ){
	}
	void refer(){
		++mReferenceCount;
	}
	void release(){
		--mReferenceCount;
	}
	int referenceCount() const {
		return mReferenceCount;
	}
protected:
	~ReferenceType(){
	}
private:
	int mReferenceCount;
};

} //namespace GameLib

#endif
