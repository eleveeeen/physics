#ifndef _RayFunc_h_
#define _RayFunc_h_

//���C�L���X�g�p�̊֐����
#include"Ray.h"
#include"shape.h"
#include"../../Utility/GameMath.h"
typedef bool(*IntersectRayFunc)(
	const RayInput& ray, RayOutput& out,
	const Shape& shape, const D3DXMATRIX& transform);

//�I�u�W�F�N�g�̌`��ɂ��������C�L���X�g�̊֐����擾
IntersectRayFunc GetIntersectRayFunc(UInt8 shapeType);

//���C�L���X�g�̊֐��Z�b�g
int SetIntersectRayFunc(UInt8 shapeType, IntersectRayFunc func);

//���C�L���X�g�p��AABB
//�ȈՔ�
static inline bool IntersectRayAABB(
	const D3DXVECTOR3& rayStartPos,
	const D3DXVECTOR3& rayDirection,
	const D3DXVECTOR3& aabbCenter,
	const D3DXVECTOR3& aabbHalf,
	float& variable)
{
	const float intersectEpsilon = 0.00001f;

	D3DXVECTOR3 aabbMin = aabbCenter - aabbHalf;
	D3DXVECTOR3 aabbMax = aabbCenter + aabbHalf;

	D3DXVECTOR3 dir = rayDirection;
	D3DXVECTOR3 absDir = GameMath::AbsPerElem(dir);
	D3DXVECTOR3 sign = GameMath::CopySignPerElem(GameMath::GetVec3(1.0f), dir);

	if (absDir.x < intersectEpsilon)
	{
		if (rayStartPos.x < aabbMin.x || rayStartPos.x>aabbMax.x)
		{
			return false;
		}

		dir.x = sign.x * intersectEpsilon;
	}

	if (absDir.y < intersectEpsilon)
	{
		if (rayStartPos.y<aabbMin.y || rayStartPos.y>aabbMax.y)
		{
			return false;
		}
		dir.y = sign.y * intersectEpsilon;

	}

	if (absDir.z < intersectEpsilon)
	{
		if (rayStartPos.z<aabbMin.z || rayStartPos.z>aabbMax.z)
		{
			return false;
		}
		dir.z = sign.z * intersectEpsilon;
	}

	D3DXVECTOR3 t1 = GameMath::DivPerElem(aabbMin - rayStartPos, dir);
	D3DXVECTOR3 t2 = GameMath::DivPerElem(aabbMax - rayStartPos, dir);

	
	D3DXVECTOR3 tMin = GameMath::MinPerElem(t1, t2);
	D3DXVECTOR3 tMax = GameMath::MaxPerElem(t1, t2);

	
	if (GameMath::MaxElem(tMin) > GameMath::MinElem(tMax))
		return false;

	if (tMin.x > tMin.y)
	{
		if (tMin.x > tMin.z)
		{
			variable = tMin.x;
		}
		else
		{
			variable = tMin.z;
		}

	}
	else
	{
		if (tMin.y > tMin.z)
		{
			variable = tMin.y;
		}
		else
		{
			variable = tMin.z;
		}
	}


	return true;
}

//���C�L���X�g�p��AABB
static inline bool IntersectRayAABB(
	const D3DXVECTOR3& rayStartPos,
	const D3DXVECTOR3& rayDirection,
	const D3DXVECTOR3& aabbCenter,
	const D3DXVECTOR3& aabbHalf,
	float& variable,
	D3DXVECTOR3& normal)
{

	const float intersectEpsilon = 0.00001f;

	//��������Ƃ���aabb�̃{�b�N�X�쐬
	D3DXVECTOR3 aabbMin = aabbCenter - aabbHalf;
	D3DXVECTOR3 aabbMax = aabbCenter + aabbHalf;


	D3DXVECTOR3 dir = rayDirection;
	//�����x�N�g���̊e�����Βl�ϊ�
	D3DXVECTOR3 absDir = GameMath::AbsPerElem(dir);
	
	D3DXVECTOR3 sign = GameMath::CopySignPerElem(GameMath::GetVec3(1.0f), dir);

	//�n�_�ƃ{�b�N�X�̓��O����
	if (aabbMin.x < rayStartPos.x && rayStartPos.x < aabbMax.x&&
		aabbMin.y < rayStartPos.y && rayStartPos.y < aabbMax.y&&
		aabbMin.z < rayStartPos.z && rayStartPos.z < aabbMax.z)
	{
		return false;
	}

	if (absDir.x < intersectEpsilon)
	{
		if (rayStartPos.x<aabbMin.x || rayStartPos.x > aabbMax.x)
		{
			return false;
		}

		dir.x = sign.x * intersectEpsilon;
	}

	if (absDir.y < intersectEpsilon)
	{
		if (rayStartPos.y<aabbMin.y || rayStartPos.y > aabbMax.y)
		{
			return false;
		}

		dir.y = sign.y * intersectEpsilon;
	}

	if (absDir.z < intersectEpsilon)
	{
		if (rayStartPos.z<aabbMin.z || rayStartPos.z > aabbMax.z)
		{
			return false;
		}

		dir.z = sign.z * intersectEpsilon;
	}

	D3DXVECTOR3 t1 = GameMath::DivPerElem(aabbMin - rayStartPos, dir);
	D3DXVECTOR3 t2 = GameMath::DivPerElem(aabbMax - rayStartPos, dir);

	D3DXVECTOR3 tMin = GameMath::MinPerElem(t1, t2);
	D3DXVECTOR3 tMax = GameMath::MaxPerElem(t1, t2);

	normal = GameMath::GetVec3(0.0f);

	if (GameMath::MaxElem(tMin) > GameMath::MinElem(tMax))
		return false;

	if (tMin.x > tMin.y)
	{
		if (tMin.x > tMin.z)
		{

			variable = tMin.x;
			normal.x = -sign.x;
		}
		else
		{
			variable = tMin.z;
			normal.z = -sign.z;
		}
	}
	else
	{
		if (tMin.y > tMin.z)
		{
			variable = tMin.y;
			normal.y = -sign.y;
		}
		else
		{
			variable = tMin.z;
			normal.z = -sign.z;
		}

	}

	return true;


}


#endif