#ifndef INCLUDED_GAMELIB_GRAPHICS_VERTEX_H
#define INCLUDED_GAMELIB_GRAPHICS_VERTEX_H

#include "GameLib/Math/Vector4.h"
#include "GameLib/Math/Vector3.h"
#include "GameLib/Math/Vector2.h"

namespace GameLib{
using namespace Math;
namespace Graphics{

class Vertex{
public:
	Vector4 mPosition; //�ʒu
	Vector3 mNormal; //�@��
	unsigned mColor; //���_�F
	Vector2 mUv; //�e�N�X�`�����W
};

} //namespace Graphics
} //namespace GameLib

#endif
