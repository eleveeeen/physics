#include"Capsule.h"
#include"Contact.h"
#include"../../Utility/GameMath.h"
#include"../../Utility/Macro.h"

inline void SegmentsClosePoints(
	D3DXVECTOR3& ptsVector,
	D3DXVECTOR3& offsetA,
	D3DXVECTOR3& offsetB,
	float& tA, float& tB,
	const D3DXVECTOR3& translation,
	const D3DXVECTOR3& dirA, float hLenA,
	const D3DXVECTOR3& dirB, float hLenB)
{
	float dirAdotDirB = D3DXVec3Dot(&dirA, &dirB);
	float dirAdotTrans = D3DXVec3Dot(&dirA, &translation);
	float dirBdotTrans = D3DXVec3Dot(&dirB, &translation);

	float denom = 1.0f - dirAdotDirB*dirAdotDirB;

	if (denom == 0.0f)
		tA = 0.0f;
	else
	{
		tA = (dirAdotTrans - dirBdotTrans*dirAdotDirB) / denom;
		if (tA < -hLenA)
			tA = -hLenA;
		else if (tA > hLenA)
			tA = hLenA;
	}

	tB = tA*dirAdotDirB - dirBdotTrans;
	if (tB < -hLenB)
	{
		tB = -hLenB;
		tA = tB*dirAdotDirB + dirAdotTrans;

		if (tA < -hLenA)
			tA = -hLenA;
		else if (tA > hLenA)
			tA = hLenA;
	}
	else if (tB > hLenB)
	{
		tB = hLenB;
		tA = tB*dirAdotDirB + dirAdotTrans;

		if (tA < -hLenA)
			tA = -hLenA;
		else if (tA > hLenA)
			tA = hLenA;
	}

	offsetA = dirA*tA;
	offsetB = dirB*tB;

	ptsVector = translation - offsetA + offsetB;

}

inline void SegmentNormal(D3DXVECTOR3& normal, const D3DXVECTOR3& ptsVector)
{
	normal = GameMath::Safe_Normalize(ptsVector);

}


float ContactCapsuleCapsule(
	D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
	void* shapeA, const D3DXMATRIX& transformA,
	void* shapeB, const D3DXMATRIX& transformB,
	float distanceThreshold)
{
	Capsule& capsuleA = *((Capsule*)shapeA);
	Capsule& capsuleB = *((Capsule*)shapeB);

	D3DXVECTOR3 directionA = GameMath::GetCol3x3(0, transformA);
	D3DXVECTOR3 translationA = GameMath::GetTranslation(transformA);

	D3DXVECTOR3 directionB = GameMath::GetCol3x3(0, transformB);
	D3DXVECTOR3 translationB = GameMath::GetTranslation(transformB);

	D3DXVECTOR3 translation = translationB - translationA;

	D3DXVECTOR3 ptsVector;
	D3DXVECTOR3 offsetA, offsetB;
	float tA, tB;

	SegmentsClosePoints(ptsVector, offsetA, offsetB, tA, tB, translation,
		directionA, capsuleA.halfLength, directionB, capsuleB.halfLength);

	float distance = D3DXVec3Length(&ptsVector) - capsuleA.radius - capsuleB.radius;

	if (distance > distanceThreshold)
		return distance;

	SegmentNormal(normal, ptsVector);

	pointA = GameMath::Rotate(
		(offsetA + normal*capsuleA.radius),
		GameMath::Transpose3x3(transformA));

	pointB = GameMath::Rotate(
		(offsetB - normal*capsuleB.radius),
		GameMath::Transpose3x3(transformB));

	return distance;
}