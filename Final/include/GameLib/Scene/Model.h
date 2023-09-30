#ifndef INCLUDED_GAMELIB_SCENE_MODEL_H
#define INCLUDED_GAMELIB_SCENE_MODEL_H

namespace GameLib{

namespace Math{
	class Vector3;
	class Vector4;
};

namespace Scene{

using namespace Math;
class Batch;

///�`��C���X�^���X
/*!
�s���n����draw����ΊG���o�邪�A����������F��ς�������ł���B
*/
class Model{
public:
	///�`��
	void draw() const;

	const Vector3* position() const;
	const Vector3* angle() const; //�P�ʂ͓x
	const Vector3* scale() const;
	const Vector3* color() const;
	float transparency() const;
	void setPosition( const Vector3& );
	void setAngle( const Vector3& ); //�P�ʂ͓x
	void setScale( const Vector3& );
	void setColor( const Vector3& );
	void setTransparency( float );

	Model();
	~Model();
	Model( const Model& );
	operator void*() const;
	Model& operator=( const Model& );
	void release();
	bool operator==( const Model& ) const;
	bool operator!=( const Model& ) const;

	class Impl;
	explicit Model( Impl* );
private:
	Impl* mImpl;
};

//const��
class ConstModel{
public:
	void draw() const;
	const Vector3* position() const;
	const Vector3* angle() const;
	const Vector3* scale() const;
	const Vector3* color() const;
	float transparency() const;

	//�ȉ��m��Ȃ��Ă���
	ConstModel();
	ConstModel( const Model& );
	~ConstModel();
	operator void*() const;
	ConstModel& operator=( const Model& );
	void release();
	bool operator==( const Model& ) const;
	bool operator!=( const Model& ) const;
private:
	Model mBase;
};

} //namespace Scene
} //namespace GameLib

#endif
