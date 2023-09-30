#ifndef INCLUDED_ROBO_H
#define INCLUDED_ROBO_H

#include "Library/Vector3.h"
#include "Sphere.h"

class Matrix44;
class GraphicsDatabase;
class Model;

class Robo{
public:
	Robo( int id ); //�ԍ����炤�B���������Ԃ��m�肽���̂ŁB
	~Robo();
	void draw( const Matrix44& perspectiveViewMatrix ) const;
	//�����\����Q�b�g
	void getMove( 
		Vector3* moveOut,
		const Vector3& viewVector ) const;
	//����������^���č��t���[���̏�����������
	void update( 
		const Vector3& move,
		const Vector3& enemyPosition );
	const Vector3* position() const;
	double angleY() const;
	void setPosition( const Vector3& );
	void setAngleY( double );
	void getDirection( Vector3* ) const;
	const Sphere* sphere() const;
private:
	Vector3 mPosition;
	double mAngleY;
	int mId;
	GraphicsDatabase* mDatabase;
	Model* mModel;
	Sphere mSphere;
};

#endif
