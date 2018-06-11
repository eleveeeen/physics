#include"Box.h"
#include"Sphere.h"
#include"Contact.h"
#include"../../Utility/GameMath.h"
#include"../../Utility/Macro.h"

static const float lenSqrTol = 1.0e-30f;

inline float VertexBFaceATest(
	D3DXVECTOR3& ptsVec,
	float& t0,
	float& t1,
	const D3DXVECTOR3& hA,
	const D3DXVECTOR3& offsetAB)
{
	D3DXVECTOR3 cptsVec = offsetAB;

	t0 = cptsVec.x;
	t1 = cptsVec.y;

	if (t0 > hA.x)
	{
		t0 = hA.x;
	}
	else if (t0 < -hA.x)
	{
		t0 = -hA.x;
	}

	if (t1>hA.y)
	{
		t1 = hA.y;
	}
	else if (t1<-hA.y)
	{
		t1 = -hA.y;
	}


	cptsVec.x -= t0;
	cptsVec.y -= t1;

	ptsVec = cptsVec;
	return D3DXVec3Dot(&ptsVec, &ptsVec);

}


float ContactBoxSphere(
	D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
	void* shapeA, const D3DXMATRIX& transformA,
	void* shapeB, const D3DXMATRIX& transformB,
	float distanceThreshold)
{
	Box& boxA = *((Box*)shapeA);
	Sphere& sphereB = *((Sphere*)shapeB);

	D3DXVECTOR3 iden[3] =
	{
		D3DXVECTOR3(1, 0, 0),
		D3DXVECTOR3(0, 1, 0),
		D3DXVECTOR3(0, 0, 1)
	};

	D3DXVECTOR3 translationB = GameMath::GetTranslation(transformB);
	D3DXVECTOR3 offsetAB = GameMath::Rotate(
		(translationB - GameMath::GetTranslation(transformA)),
		GameMath::Transpose3x3(transformA));

	D3DXVECTOR3 axisA;
	int faceDimA;
	float maxGap;
	D3DXVECTOR3 gapsA = GameMath::AbsPerElem(offsetAB) - boxA.half - GameMath::GetVec3(sphereB.radius);
	D3DXVECTOR3 signsA = GameMath::CopySignPerElem(GameMath::GetVec3(1.0f), offsetAB);

	{
		float gap = gapsA.x;
		if (gap > distanceThreshold)
			return gap;

		maxGap = gap;
		faceDimA = 0;
		axisA = GameMath::MulPerElem(iden[0], signsA);

		if (gap > maxGap)
		{
			maxGap = gap;
			faceDimA = 0;
			axisA = GameMath::MulPerElem(iden[0], signsA);
		}

		gap = gapsA.y;

		if (gap > distanceThreshold)
			return gap;

		if (gap > maxGap)
		{
			maxGap = gap;
			faceDimA = 1;
			axisA = GameMath::MulPerElem(iden[1], signsA);
		}

		gap = gapsA.z;
		if (gap > distanceThreshold)
			return gap;

		if (gap > maxGap)
		{
			maxGap = gap;
			faceDimA = 2;
			axisA = GameMath::MulPerElem(iden[2], signsA);
		}

	}

	int dimA[3];
	dimA[2] = faceDimA;
	dimA[0] = (faceDimA + 1) % 3;
	dimA[1] = (faceDimA + 2) % 3;

	D3DXMATRIX apermCol;
	GameMath::SetCol3x3(0, apermCol, iden[dimA[0]]);
	GameMath::SetCol3x3(1, apermCol, iden[dimA[1]]);
	GameMath::SetCol3x3(2, apermCol, iden[dimA[2]]);

	D3DXMATRIX apermRow = GameMath::Transpose3x3(apermCol);

	D3DXVECTOR3 halfAperm = GameMath::Rotate(boxA.half, apermRow);
	D3DXVECTOR3 offsetABperm = GameMath::Rotate(offsetAB, apermRow);
	D3DXVECTOR3 signsAperm = GameMath::Rotate(signsA, apermRow);

	float signA2 = signsAperm.z;
	float scaleA2 = halfAperm.z*signA2;

	offsetABperm.z = offsetABperm.z - scaleA2;

	float t0, t1;
	float minDistSqr;
	D3DXVECTOR3 closestPtsVecPerm;
	D3DXVECTOR3 localPointAPerm;

	minDistSqr = VertexBFaceATest(closestPtsVecPerm, t0, t1, halfAperm, offsetABperm);

	localPointAPerm = D3DXVECTOR3(t0, t1, scaleA2);
	bool centerInside = (signA2*closestPtsVecPerm.z < 0.0f);

	if (centerInside || (minDistSqr < lenSqrTol))
	{
		normal = GameMath::Rotate(axisA, transformA);
	}
	else
	{
		D3DXVECTOR3 closestPtsVec = GameMath::Rotate(closestPtsVecPerm, apermCol);
		normal = GameMath::Rotate(
			(closestPtsVec*(1.0f / sqrtf(minDistSqr))),
			transformA);
	}

	pointA = GameMath::Rotate(localPointAPerm, apermCol);
	pointB = GameMath::Rotate(
		(-normal*sphereB.radius),
		GameMath::Transpose3x3(transformB));


	if (centerInside)
		return -sqrtf(minDistSqr) - sphereB.radius;
	else
		return sqrtf(minDistSqr) - sphereB.radius;

}