#ifndef INCLUDED_CURVE_H
#define INCLUDED_CURVE_H

namespace GameLib{
	namespace PseudoXml{
		class Element;
	}
}

class Curve{
public:
	enum Type{
		TYPE_TRANSLATION_X,
		TYPE_TRANSLATION_Y,
		TYPE_TRANSLATION_Z,
		TYPE_ROTATION_X,
		TYPE_ROTATION_Y,
		TYPE_ROTATION_Z,
		TYPE_SCALE_X,
		TYPE_SCALE_Y,
		TYPE_SCALE_Z,
	};
	enum Interporation{
		INTERPORATION_NONE, //��ԂȂ��B
		INTERPORATION_LINEAR, //1�����
		INTERPORATION_CUBIC, //3�����
	};
	Curve( GameLib::PseudoXml::Element& );
	~Curve();
	//���鎞���̃f�[�^�����炤
	double get( double time ) const;
	//�^�C�v�擾
	Type type() const;
private:
	struct Data{
		Data();
		double mTime;
		double mValue;
		double mLeftSlope;
		double mRightSlope;
	};
	Data* mData;
	int mDataNumber;
	Type mType;
	Interporation mInterporation;
};

#endif
