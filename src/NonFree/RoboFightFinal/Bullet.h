#ifndef INCLUDED_BULLET_H
#define INCLUDED_BULLET_H

#include "GameLib/Scene/Container.h"
#include "GameLib/Scene/Model.h"

namespace GameLib{
	namespace Math{
		class Matrix44;
		class Vector3;
	}
}
using namespace GameLib::Math;

class Bullet{
public:
	Bullet();
	~Bullet();
	void create( 
		GameLib::Scene::Container,
		const char* batchName,
		const Vector3& position,
		float angleX, 
		float angleY,
		bool homing );
	const Vector3* position() const;
	void update( const Vector3& enemyPos ); 
	void draw() const;
	bool isEmpty() const; //�g���ĂȂ����true�BmCount<0�Ŕ���
	void die(); //�e�����܂�
private:
	GameLib::Scene::Model mModel;
	int mCount; //-1�Ȃ炻�̒e�͋󂾂Ƃ������Ƃɂ��悤�B
	bool mHoming; //�ǔ�����H

	static const float mHomingX; //�p�x�U�����\X
	static const float mHomingY; //�p�x�U�����\Y
	static const int mLife; //�e����
	static const float mSpeed; //�e�̑��x
};

#endif