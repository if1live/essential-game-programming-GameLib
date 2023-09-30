#ifndef INCLUDED_GAMELIB_SCENE_CURVE_H
#define INCLUDED_GAMELIB_SCENE_CURVE_H

#include "GameLib/PseudoXml/Element.h"
#include "GameLib/PseudoXml/Attribute.h"
#include "GameLib/Base/RefString.h"
#include "GameLib/Math/Functions.h"

namespace GameLib{
using namespace PseudoXml;
using namespace Math;
namespace Scene{

class Curve{
public:
	enum Type{
		TYPE_TX, //transX
		TYPE_TY, //transY
		TYPE_TZ, //transZ
		TYPE_RX, //rotateX
		TYPE_RY, //rotateY
		TYPE_RZ, //rotateZ
		TYPE_SX, //scaleX
		TYPE_SY, //scaleY
		TYPE_SZ, //scaleZ
		TYPE_SA, //scale ALL(XYZ)
		TYPE_UNKNOWN,
	};
	struct Key{
		Key() : mTime( 0.f ), mValue( 0.f ), mLeftSlope( 0.f ), mRightSlope( 0.f ){}
		float mTime;
		float mValue;
		float mLeftSlope;
		float mRightSlope;
	};
	Curve( PseudoXml::ConstElement& e ) :
	mKeys( 0 ),
	mKeyNumber( 0 ),
	mType( TYPE_UNKNOWN ), //�K��
	mInterporation( INTERPORATION_NONE ){
		ASSERT( RefString( "Curve" ) == e.name() ); //Curve����ˁH
		ASSERT( e.childNumber() > 0 ); //�f�[�^��͂����ˁH
		//���O�擾
		bool isRot = false; //�t�@�C���͓x�Ȃ̂ŁA���W�A���ɒ����B
		int an = e.attributeNumber();
		for ( int i = 0; i < an; ++i ){
			ConstAttribute a = e.attribute( i );
			RefString name( a.name() );
			RefString value( a.value() );
			if ( name == "type" ){
				if ( value == "translationX" ){
					mType = TYPE_TX;
				}else if ( value == "translationY" ){
					mType = TYPE_TY;
				}else if ( value == "translationZ" ){
					mType = TYPE_TZ;
				}else if ( value == "rotationX" ){
					mType = TYPE_RX;
					isRot = true;
				}else if ( value == "rotationY" ){
					mType = TYPE_RY;
					isRot = true;
				}else if ( value == "rotationZ" ){
					mType = TYPE_RZ;
					isRot = true;
				}else if ( value == "scaleX" ){
					mType = TYPE_SX;
				}else if ( value == "scaleY" ){
					mType = TYPE_SY;
				}else if ( value == "scaleZ" ){
					mType = TYPE_SZ;
				}else if ( value == "scaleXYZ" ){
					mType = TYPE_SA;
				}
			}else if ( name == "interporation" ){
				if ( value == "none" ){
					mInterporation = INTERPORATION_NONE;
				}else if ( value == "linear" ){
					mInterporation = INTERPORATION_LINEAR;
				}else if ( value == "cubic" ){
					mInterporation = INTERPORATION_CUBIC;
				}
			}
		}
		mKeyNumber = e.childNumber();
		mKeys = static_cast< Key* >( OPERATOR_NEW( sizeof( Key ) * mKeyNumber ) );
		for ( int i = 0; i < mKeyNumber; ++i ){
			new ( &mKeys[ i ] ) Key;
			ConstElement c = e.child( i );
			an = c.attributeNumber();
			for ( int j = 0; j < an; ++j ){
				ConstAttribute a = c.attribute( j );
				RefString name( a.name() );
				float v = a.getFloatValue();
				if ( name == "time" ){
					mKeys[ i ].mTime = v;
				}else if ( name == "value" ){
					mKeys[ i ].mValue = v;
				}else if ( name == "leftSlope" ){
					mKeys[ i ].mLeftSlope = v;
				}else if ( name == "rightSlope" ){
					mKeys[ i ].mRightSlope = v;
				}else if ( name == "slope" ){
					mKeys[ i ].mRightSlope = mKeys[ i ].mLeftSlope = v;
				}else{
					ASSERT( false ); //���肦��
				}
			}
		}
	}
	~Curve(){
		mName = 0; //�R���e�i�������Ă���̂ŎQ�Ƃ��O������
		for ( int i = 0; i < mKeyNumber; ++i ){
			mKeys[ i ].~Key();
		}
		OPERATOR_DELETE( mKeys );
	}
	float operator()( float t ) const {
		//���[�v����������B�Ō�̃f�[�^�̎����������ƍl����B����ŏz����ˁB
		//a���牽��b�������邩���v�Z����ɂ́Aa -= toInt(a/b) * b�ŋ��܂�BtoInt�͐����ɐ؂�̂Ă�֐��Ƃ��悤�B�L���X�g�����܂������Ώo����B
		float quot = t / mKeys[ mKeyNumber - 1 ].mTime; //���������̂𐮐��ɒ����Ə��̐������o��
		int quotInt = static_cast< int >( quot ); 
		t -= static_cast< float >( quotInt ) * mKeys[ mKeyNumber - 1 ].mTime;

		if ( mInterporation == INTERPORATION_NONE ){
			int last = 0;
			for ( int i = 0; i < mKeyNumber; ++i ){
				if ( mKeys[ i ].mTime > t ){
					break;
				}
				last = i;
			}
			//t���z���Ȃ��ŏ���time�����f�[�^���g��(��ԂȂ�)
			return mKeys[ last ].mValue;
		}else if ( mInterporation == INTERPORATION_LINEAR ){
			int begin = 0;
			int end = 0;
			for ( end = 0; end < mKeyNumber; ++end ){
				if ( mKeys[ end ].mTime > t ){
					break;
				}
				begin = end;
			}
			if ( end >= mKeyNumber ){ //���ӂ�h�~�B�Ō�̒l�Ԃ��Ă��
				return mKeys[ mKeyNumber - 1 ].mValue;
			}
			float t0 = mKeys[ begin ].mTime;
			float t1 = mKeys[ end ].mTime;
			float p0 = mKeys[ begin ].mValue;
			float p1 = mKeys[ end ].mValue;
			//�ϐ��ϊ�
			t = ( t - t0 ) / ( t1 - t0 );
			//���`���
			return p0 + ( p1 - p0 ) * t;
		}else if ( mInterporation == INTERPORATION_CUBIC ){
			int begin = 0;
			int end = 0;
			for ( end = 0; end < mKeyNumber; ++end ){
				if ( mKeys[ end ].mTime > t ){
					break;
				}
				begin = end;
			}
			if ( end >= mKeyNumber ){ //���ӂ�h�~�B�Ō�̒l�Ԃ��Ă��
				return mKeys[ mKeyNumber - 1 ].mValue;
			}
			float t0 = mKeys[ begin ].mTime;
			float t1 = mKeys[ end ].mTime;
			float p0 = mKeys[ begin ].mValue;
			float p1 = mKeys[ end ].mValue;
			float v0 = mKeys[ begin ].mRightSlope; //�n�_�̉E�X��
			float v1 = mKeys[ end ].mLeftSlope; //�I�_�̍��X��
			//�ϐ��ϊ�
			t = ( t - t0 ) / ( t1 - t0 );
			//at^3 + bt^2 + c + d�Ōv�Z�Bc=v0�Ad=p0���B
			float a = 2.f * ( p0 - p1 ) + ( v0 + v1 );
			float b = 3.f * ( p1 - p0 ) - ( 2.f * v0 ) - v1;
			//���`���
			float r = a; //a
			r *= t; //at
			r += b; //at+b
			r *= t; //at^2+bt
			r += v0; //at^2+bt+c
			r *= t; //at^3+bt^2+ct
			r += p0; //at^3+bt^2+ct+d
			return r;
		}else{
			ASSERT( false ); //���肦��
			return 0.0;
		}
	}
	Type type() const {
		return mType;
	}
private:
	enum Interporation{
		INTERPORATION_NONE, //��ԂȂ�
		INTERPORATION_LINEAR, //���`
		INTERPORATION_CUBIC, //3��
	};
	Key* mKeys;
	int mKeyNumber;
	const char* mName;
	Type mType;
	Interporation mInterporation;
};

} //namespace Scene
} //namespace GameLib

#endif

