#include"Sphere.h"
#include"Contact.h"
#include"../../Utility/GameMath.h"

const float lenSqrTol = 1.0e-30f;

float ContactSphereSphere(
	D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
	void* shapeA, const D3DXMATRIX& transformA,
	void* shapeB, const D3DXMATRIX& transformB,
	float distanceThreshold)
{

	Sphere& sphereA = *((Sphere*)shapeA);
	Sphere& sphereB = *((Sphere*)shapeB);

	D3DXVECTOR3 direction(0.0f, 0.0f, 0.0f);
	
	D3DXVECTOR3 translationA = GameMath::GetTranslation(transformA);
	D3DXVECTOR3 translationB = GameMath::GetTranslation(transformB);

	D3DXVECTOR3 offsetAB;
	offsetAB = translationB - translationA;

	float distSqr = D3DXVec3Dot(&offsetAB, &offsetAB);

	float dist = sqrtf(distSqr);
	float sphereDist = dist - sphereA.radius - sphereB.radius;

	if (sphereDist > distanceThreshold)
		return sphereDist;

	if (distSqr > lenSqrTol)
	{
		float distInv = 1.0f / dist;
		direction = offsetAB*distInv;
	}
	else
	{
		direction = D3DXVECTOR3(0, 0, 1);
	}

	normal = direction;

	pointA = GameMath::Rotate(
		(direction*sphereA.radius),
		GameMath::Transpose3x3(transformA));

	pointB = GameMath::Rotate(
		(direction*sphereB.radius),
		GameMath::Transpose3x3(transformB));

	return sphereDist;

}
