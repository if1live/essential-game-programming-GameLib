#include "Matrix22.h"
#include "Vector2.h"

Matrix22::Matrix22( double e00, double e01, double e10, double e11 ) :
m00( e00 ),
m01( e01 ),
m10( e10 ),
m11( e11 ){
}

void Matrix22::multiply( Vector2* out, const Vector2& in ) const {
	double tx = in.x; //out��in��������������Ȃ��̂ŁA�o�b�N�A�b�v
	out->x = m00 * tx + m01 * in.y; //����*x + �E��*y
	out->y = m10 * tx + m11 * in.y; //����*x + �E��*y
}
