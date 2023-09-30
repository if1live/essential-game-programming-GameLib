#ifndef INCLUDED_GAMELIB_COLLISIONMESH_H
#define INCLUDED_GAMELIB_COLLISIONMESH_H

namespace GameLib{
namespace PseudoXml{
	class ConstElement;
}
namespace Math{
	class Vector3;
};
namespace Scene{
using namespace Math;

///�Փ˔���p�O�p�`�W���N���X
/*
Container������VertexBuffer��IndexBuffer��S���g���Ĉ�̎O�p�`�W�����`������B
Batch��Animation���X�͒P���Ɍ��Ȃ��B
*/
class CollisionMesh{
public:
	///�j�ZXML�t�@�C������R���X�g���N�g
	static CollisionMesh create( const char* filename );
	///�j�ZXML�̃G�������g(Container)����R���X�g���N�g
	static CollisionMesh create( PseudoXml::ConstElement );
	///���[�h�I�����m�B�t�@�C�������������ꍇ��true���Ԃ邱�Ƃ��m�F���Ă���g�����ƁB
	bool isReady();
	///�����ƌ�������B
	bool isIntersect( const Vector3& lineOrigin, const Vector3& lineVector ) const;
	///�����ƌ�������B�ŏ��ɓ�������������timeOut�ɓ����B�������true��Ԃ��B
	bool getIntersection( 
		float* timeOut, //0�|�C���^�Ȃ�Ԃ��Ȃ�
		Vector3* normalOut, //0�|�C���^�Ȃ�Ԃ��Ȃ�
		const Vector3& lineOrigin, 
		const Vector3& lineVector ) const;

	CollisionMesh();
	~CollisionMesh();
	CollisionMesh( const CollisionMesh& );
	operator void*() const;
	CollisionMesh& operator=( const CollisionMesh& );
	void release();
	bool operator==( const CollisionMesh& ) const;
	bool operator!=( const CollisionMesh& ) const;
private:
	class Impl;
	explicit CollisionMesh( Impl* );
	Impl* mImpl;
};

} //namespace Scene
} //namespace GameLib

#endif
