#ifndef INCLUDED_ANIMATION_NODE_H
#define INCLUDED_ANIMATION_NODE_H

class Vector3;
class Curve;

#include <string>
using namespace std;

namespace GameLib{
	namespace PseudoXml{
		class Element;
	}
}

class AnimationNode{
public:
	AnimationNode( GameLib::PseudoXml::Element& );
	~AnimationNode();
	//�f�[�^�����炤�B�A�j���[�V�������Ȃ����͂���������̂�������Ȃ��B
	void data( 
		Vector3* translationInOut,
		Vector3* rotationInOut,
		Vector3* scaleInOut,
		double time ) const;
	const string* name() const;
private:
	Curve** mCurves;
	int mCurveNumber;
	string mName;
};

#endif
