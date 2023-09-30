#ifndef INCLUDED_BULLET_H
#define INCLUDED_BULLET_H

class Model;
class GraphicsDatabase;
class Matrix44;
class Vector3;

class Bullet{
public:
	Bullet();
	~Bullet();
	void create( 
		GraphicsDatabase*,
		const char* batchName,
		const Vector3& position,
		double angleX, 
		double angleY,
		bool homing,
		const Vector3& color );
	const Vector3* position() const;
	//�������͒ǔ����邩�ۂ�
	void update( const Vector3& enemyPos ); 
	void draw( 
		const Matrix44& projectionVewMatrix,
		const Vector3& lightVector,
		const Vector3& lightColor,
		const Vector3& ambient ) const;
	bool isEmpty() const; //�g���ĂȂ����true�BmCount<0�Ŕ���
	void die(); //�e�����܂�
private:
	Model* mModel;
	int mCount; //-1�Ȃ炻�̒e�͋󂾂Ƃ������Ƃɂ��悤�B
	bool mHoming; //�ǔ�����H

	static const float mHomingX; //�p�x�U�����\X
	static const float mHomingY; //�p�x�U�����\Y
	static const int mLife; //�e����
	static const float mSpeed; //�e�̑��x
};

#endif