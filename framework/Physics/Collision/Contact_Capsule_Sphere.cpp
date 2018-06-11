#include"Capsule.h"
#include"Sphere.h"
#include"Contact.h"
#include"../../Utility/GameMath.h"
#include"../../Utility/Macro.h"

inline void SegmentPointClosestPoints(
	D3DXVECTOR3& ptsVector,
	D3DXVECTOR3& offsetA,
	float& tA,
	const D3DXVECTOR3& translation,
	const D3DXVECTOR3& dirA,
	float hLenA)
{
	tA = D3DXVec3Dot(&dirA, &translation);

	if (tA < -hLenA)
		tA = -hLenA;
	else if (tA > hLenA)
		tA = hLenA;

	offsetA = dirA*tA;

	ptsVector = translation - offsetA;

}


inline void SegmentPointNormal(D3DXVECTOR3& normal, const D3DXVECTOR3& ptsVector)
{
	normal = GameMath::Safe_Normalize(ptsVector);
}


float ContactCapsuleSphere(
	D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
	void* shapeA, const D3DXMATRIX& transformA,
	void* shapeB, const D3DXMATRIX& transformB,
	float distanceThreshold)
{

	Capsule& capsuleA = *((Capsule*)shapeA);
	Sphere& sphereB = *((Sphere*)shapeB);

	D3DXVECTOR3 directionA = GameMath::GetCol3x3(0, transformA);
	D3DXVECTOR3 translationA = GameMath::GetTranslation(transformA);
	D3DXVECTOR3 translationB = GameMath::GetTranslation(transformB);

	D3DXVECTOR3 translation = translationB - translationA;

	D3DXVECTOR3 ptsVector;
	D3DXVECTOR3 offsetA;
	float tA;

	SegmentPointClosestPoints(ptsVector, offsetA, tA, translation, directionA, capsuleA.halfLength);

	float distance = D3DXVec3Length(&ptsVector) - capsuleA.radius - sphereB.radius;

	if (distance > distanceThreshold)
		return distance;

	SegmentPointNormal(normal, ptsVector);

	pointA = GameMath::Rotate(
		(offsetA + normal*capsuleA.radius),
		GameMath::Transpose3x3(transformA));

	pointB = GameMath::Rotate(
		(-normal*sphereB.radius),
		GameMath::Transpose3x3(transformB));

	return distance;

}