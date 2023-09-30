#ifndef INCLUDED_GAMELIB_MATH_FUNCTIONS_H
#define INCLUDED_GAMELIB_MATH_FUNCTIONS_H

//���w���C�u�����B�p�x�P�ʂ͓x�B
namespace GameLib{
namespace Math{

//�O�p�֐�
float sin( float );
float cos( float );
float tan( float );
//�t�O�p�֐�
float asin( float );
float acos( float );
float atan( float );
float atan2( float y, float x );

float pow( float a, float b ); //�ݏ�(a��b��)
float log2( float ); //2���Ƃ���ΐ�
float log10( float ); //10���Ƃ���ΐ�
float abs( float );//��Βl
float min( float, float ); //�ŏ�
float max( float, float ); //�ő�
float sqrt( float ); //������

const float EPSILON = 0.00000011920928955078125f; //float�̌��E���x(1/2^23)
const float FLOAT_MAX = 1.7014117331926442990585209174226e+38; //float�̍ő�l((2^24-1)/2^23 * 2^126 )

bool isSpecial( float ); //NaN,Inf�Ȃ�true

} //namespace Math
} //namespace GameLib

#endif