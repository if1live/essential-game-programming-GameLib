#ifndef INCLUDED_GAMELIB_SCENE_TREE_H
#define INCLUDED_GAMELIB_SCENE_TREE_H

namespace GameLib{

namespace Math{
	class Matrix44;
	class Vector3;
	class Vector4;
};

namespace Scene{
class Animation;
using namespace Math;

///�K�w�\�����f��
class Tree{
public:
	///�`��
	void draw() const;
	///�A�j���[�V�����ݒ�
	void setAnimation( Animation );
	///�A�j���[�V�����X�V(�����͎����B�}�C�i�X�Ȃ璆�ŏ���ɐi�߂�)
	void updateAnimation( float time = -1.f );
	///�A�j�����x�ݒ�
	void setAnimationSpeed( float );

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

	Tree();
	~Tree();
	Tree( const Tree& );
	operator void*() const;
	Tree& operator=( const Tree& );
	void release();
	bool operator==( const Tree& ) const;
	bool operator!=( const Tree& ) const;

	class Impl;
	explicit Tree( Impl* );
private:
	Impl* mImpl;
};

//const��
class ConstTree{
public:
	void draw() const;
	const Vector3* position() const;
	const Vector3* angle() const;
	const Vector3* scale() const;
	const Vector3* color() const;
	float transparency() const;

	//�ȉ��m��Ȃ��Ă���
	ConstTree();
	ConstTree( const Tree& );
	~ConstTree();
	operator void*() const;
	ConstTree& operator=( const Tree& );
	void release();
	bool operator==( const Tree& ) const;
	bool operator!=( const Tree& ) const;
private:
	Tree mBase;
};


} //namespace Scene
} //namespace GameLib

#endif
