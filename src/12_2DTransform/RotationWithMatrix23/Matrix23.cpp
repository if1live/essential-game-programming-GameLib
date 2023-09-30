#include "Matrix23.h"
#include "Vector2.h"

Matrix23::Matrix23( 
double e00, double e01, double e02,
double e10, double e11, double e12 ) :
m00( e00 ),
m01( e01 ),
m02( e02 ),
m10( e10 ),
m11( e11 ),
m12( e12 ){
}

void Matrix23::multiply( Vector2* out, const Vector2& in ) const {
	double tx = in.x; //out��in��������������Ȃ��̂ŁA�o�b�N�A�b�v
	out->x = m00 * tx + m01 * in.y + m02; //����*x + ����*y + �E��*1
	out->y = m10 * tx + m11 * in.y + m12; //����*x + ����*y + ����*1
}
