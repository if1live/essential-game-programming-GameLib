#ifndef INCLUDED_ROBO_H
#define INCLUDED_ROBO_H

#include "Vector3.h"

namespace GameLib{
	class Texture;
}
class Matrix44;

class Robo{
public:
	Robo( int id ); //�ԍ����炤�B���������Ԃ��m�肽���̂ŁB
	~Robo();
	void draw( const Matrix44& perspectiveViewMatrix ) const;
	void update( 
		const Vector3& enemyPosition, 
		const Vector3& viewVector );
	const Vector3* position() const;
	double angleY() const;
	void setPosition( const Vector3& );
	void setAngleY( double );
	void getDirection( Vector3* ) const;
private:
	Vector3 mPosition;
	double mAngleY;
	int mId;
	GameLib::Texture* mTexture;
};

#endif
