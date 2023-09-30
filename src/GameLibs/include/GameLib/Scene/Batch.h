#ifndef INCLUDED_GAMELIB_SCENE_BATCH_H
#define INCLUDED_GAMELIB_SCENE_BATCH_H

namespace GameLib{

namespace Math{
	class Vector3;
};

namespace Scene{

using namespace Math;
class Batch;

///�`��C���^�[�t�F�C�X
/*!
draw����ΊG���o��B�������Ȃ��Ă������͂���ő����B
*/
class Batch{
public:
	///�`��
	void draw() const;
	///�`��(�C���F�t��)
	void draw( const Vector3& color, float transparency ) const;

	Batch();
	~Batch();
	Batch( const Batch& );
	operator void*() const;
	Batch& operator=( const Batch& );
	void release();
	bool operator==( const Batch& ) const;
	bool operator!=( const Batch& ) const;

	class Impl;
	explicit Batch( Impl* );
private:
	Impl* mImpl;
};

} //namespace Scene
} //namespace GameLib

#endif
