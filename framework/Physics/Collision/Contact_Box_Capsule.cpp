#include"Box.h"
#include"Capsule.h"
#include"Contact.h"
#include"../../Utility/GameMath.h"
#include"../../Utility/Macro.h"

enum BoxCapsSepAxisType
{
	BOX_AXIS,
	CROSS_AXIS
};



static const float voronoiTol = -1.0e-5f;

static const float lenSqrTol = 1.0e-30f;


#define AaxisTest(dim,letter,first) 					\
{														\
	if (first)											\
	{													\
		maxGap = gapsA.##letter;						\
		if(maxGap-capsuleB.radius>distanceThreshold)	\
			return maxGap-capsuleB.radius;				\
		axisType = BOX_AXIS;							\
		faceDimA = dim;									\
		axisA = iden[dim];								\
	}													\
	else												\
	{													\
		float gap = gapsA.##letter;						\
		if(gap-capsuleB.radius>distanceThreshold)		\
			return gap-capsuleB.radius;					\
		else if (gap > maxGap)							\
		{												\
			maxGap = gap;								\
			axisType = BOX_AXIS;						\
			faceDimA = dim;								\
			axisA = iden[dim];							\
		}												\
														\
	}													\
}

#define CrossAxisTest(dima,lettera)										  \
{																		  \
	const float lsqrTolerance = 1.0e-30f;								  \
	float lsqr;															  \
																		  \
	lsqr = lsqrs.##lettera;												  \
																		  \
	if (lsqr > lsqrTolerance)											  \
	{																	  \
		float recip = 1.0f / sqrtf(lsqr);								  \
		float gap = gapsAxB.##lettera * recip;							  \
		if (gap - capsuleB.radius > distanceThreshold)					  \
		{																  \
			return gap - capsuleB.radius;								  \
		}																  \
																		  \
		if (gap > maxGap)												  \
		{																  \
			maxGap = gap;												  \
			axisType = CROSS_AXIS;										  \
			edgeDimA = dima;											  \
			axisA = GameMath::GetCol3x3(dima, crossProdMat)*recip;		  \
		}																  \
	}																	  \
}

inline float VertexBFaceATest(
	bool& inVoronoi,
	float& t0,
	float& t1,
	D3DXVECTOR3& ptsVec,
	const D3DXVECTOR3& hA,
	D3DXVECTOR3 offsetAB,
	D3DXVECTOR3 capsDirection,
	float signB,
	float scaleB)
{
	D3DXVECTOR3 endPoint = offsetAB + capsDirection*scaleB;

	t0 = endPoint.x;
	t1 = endPoint.y;

	if (t0 > hA.x)
		t0 = hA.x;
	else if (t0 < -hA.x)
		t0 = -hA.x;

	if (t1>hA.y)
		t1 = hA.y;
	else if (t1 < -hA.y)
		t1 = -hA.y;

	endPoint.x -= t0;
	endPoint.y -= t1;

	ptsVec = endPoint;

	inVoronoi = (-signB*D3DXVec3Dot(&ptsVec, &capsDirection) >= voronoiTol);

	return (D3DXVec3LengthSq(&ptsVec));

}

#define VertexBFaceASetNewMin()	   \
{								   \
	minDistSqr = distSqr;		   \
	closestPtsVec = ptsVec;		   \
	localPointA.x = t0;			   \
	localPointA.y = t1;			   \
	segmentParamB = scaleB;		   \
}


void VertexBFaceATests(
	bool& done,
	float& minDistSqr,
	D3DXVECTOR3& closestPtsVec,
	D3DXVECTOR3& localPointA,
	float& segmentParamB,
	const D3DXVECTOR3& hA,
	D3DXVECTOR3 offsetAB,
	D3DXVECTOR3 capsDirection,
	float signB,
	float scaleB,
	bool first)
{
	D3DXVECTOR3 ptsVec;
	float t0, t1;
	float distSqr;


	distSqr = VertexBFaceATest(done, t0, t1, ptsVec, hA, offsetAB, capsDirection, signB, scaleB);

	if (first)
	{
		VertexBFaceASetNewMin();
	}
	else if (distSqr < minDistSqr)
	{
		VertexBFaceASetNewMin();
	}

	if (done)
		return;

	signB = -signB;
	scaleB = -scaleB;

	distSqr = VertexBFaceATest(done, t0, t1, ptsVec, hA, offsetAB, capsDirection, signB, scaleB);

	if (distSqr < minDistSqr)
	{
		VertexBFaceASetNewMin();
	}
}




#define EdgeEdgeTest(ac,acLetter,ad,adLetter)								\
{																			\
	ptsVec = offsetAB;														\
	ptsVec.##acLetter = ptsVec.##acLetter - scalesA.##acLetter;				\
																			\
	float capsDirection_ad = capsDirection.##adLetter;						\
																			\
	float ptsVec_ad = ptsVec.##adLetter;									\
																			\
	float capsDirDotPtsVec = D3DXVec3Dot(&capsDirection, &ptsVec);			\
																			\
	float denom = 1.0f - capsDirection_ad* capsDirection_ad;				\
																			\
																			\
	if (denom == 0.0f)														\
	{																		\
		tA = 0.0f;															\
	}																		\
	else																	\
	{																		\
		tA = (ptsVec_ad - capsDirDotPtsVec*capsDirection_ad) / denom;		\
		if (tA < -hA.##adLetter)											\
		{																	\
			tA = -hA.##adLetter;											\
		}																	\
		else if (tA > hA.##adLetter)										\
		{																	\
			tA = hA.##adLetter;												\
		}																	\
	}																		\
																			\
	tB = tA*capsDirection_ad - capsDirDotPtsVec;							\
																			\
	if (tB < -hB)															\
	{																		\
		tB = -hB;															\
		tA = tB*capsDirection_ad + ptsVec_ad;								\
																			\
		if (tA < -hA.##adLetter)											\
			tA = -hA.##adLetter;											\
		else if (tA>hA.##adLetter)											\
			tA = hA.##adLetter;												\
	}																		\
	else if (tB > tB)														\
	{																		\
		tB = hB;															\
		tA = tB*capsDirection_ad + ptsVec_ad;								\
																			\
		if (tA < -hA.##adLetter)											\
			tA = -hA.##adLetter;											\
		else if (tA>hA.##adLetter)											\
			tA = hA.##adLetter;												\
	}																		\
																			\
	ptsVec += capsDirection * tB;											\
	D3DXVECTOR3 cptsVec = GameMath::MulPerElem(ptsVec, signsA);				\
																			\
	inVoronoi = ((cptsVec.##acLetter >= voronoiTol * cptsVec.z) &&			\
				 (cptsVec.z >= voronoiTol * cptsVec.##acLetter));			\
																			\
	ptsVec.##adLetter = ptsVec.##adLetter - tA;								\
	return D3DXVec3LengthSq(&ptsVec);										\
}



float EdgeEdgeTest_01(
	bool& inVoronoi,
	float& tA,
	float& tB,
	D3DXVECTOR3& ptsVec,
	const D3DXVECTOR3& hA,
	float hB,
	D3DXVECTOR3 offsetAB,
	D3DXVECTOR3 capsDirection,
	D3DXVECTOR3 signsA,
	D3DXVECTOR3 scalesA)
{
	EdgeEdgeTest(0, x, 1, y);
}

float EdgeEdgeTest_10(
	bool& inVoronoi,
	float& tA,
	float& tB,
	D3DXVECTOR3& ptsVec,
	const D3DXVECTOR3& hA,
	float hB,
	D3DXVECTOR3 offsetAB,
	D3DXVECTOR3 capsDirection,
	D3DXVECTOR3 signsA,
	D3DXVECTOR3 scalesA)
{
	EdgeEdgeTest(1, y, 0, x);
}


#define EdgeEdgeSetNewMin(acLetter,adLetter)		   \
{													   \
	minDistSqr = distSqr;							   \
	closestPtsVec = ptsVec;							   \
	localPointA.##acLetter = scalesA.##acLetter;	   \
	localPointA.##adLetter = tA;					   \
	segmentParamB = tB;								   \
	otherFaceDimA = testOtherFaceDimA;				   \
													   \
}

void EdgeEdgeTests(
	bool& done,
	float& minDistSqr,
	D3DXVECTOR3& closestPtsVec,
	D3DXVECTOR3& localPointA,
	float& segmentParamB,
	int& otherFaceDimA,
	const D3DXVECTOR3& hA,
	float hB,
	D3DXVECTOR3 offsetAB,
	D3DXVECTOR3 capsDirection,
	D3DXVECTOR3 signsA,
	D3DXVECTOR3 scalesA,
	bool first)
{
	D3DXVECTOR3 ptsVec;
	float tA, tB;
	int testOtherFaceDimA = 0;

	float distSqr = EdgeEdgeTest_01(done, tA, tB, ptsVec, hA, hB,
		offsetAB, capsDirection, signsA, scalesA);

	if (first)
	{
		EdgeEdgeSetNewMin(x, y);

	}

	else if (distSqr < minDistSqr)
	{
		EdgeEdgeSetNewMin(x, y);
	}

	if (done)
		return;

	signsA.x = -signsA.x;
	scalesA.x = -scalesA.x;

	distSqr = EdgeEdgeTest_01(done, tA, tB, ptsVec, hA, hB,
		offsetAB, capsDirection, signsA, scalesA);

	if (distSqr < minDistSqr)
	{
		EdgeEdgeSetNewMin(x, y);
	}

	if (done)
		return;

	testOtherFaceDimA = 1;

	distSqr = EdgeEdgeTest_10(done, tA, tB, ptsVec, hA, hB,
		offsetAB, capsDirection, signsA, scalesA);

	if (distSqr < minDistSqr)
	{
		EdgeEdgeSetNewMin(y, x);
	}

	if (done)
		return;

	signsA.y = -signsA.y;
	scalesA.y = -scalesA.y;

	distSqr = EdgeEdgeTest_10(done, tA, tB, ptsVec, hA, hB,
		offsetAB, capsDirection, signsA, scalesA);

	if (distSqr < minDistSqr)
	{
		EdgeEdgeSetNewMin(y, x);
	}
}


float ContactBoxCapsule(
	D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
	void* shapeA, const D3DXMATRIX& transformA,
	void* shapeB, const D3DXMATRIX& transformB,
	float distanceThreshold)
{
	Box boxA = *((Box*)shapeA);
	Capsule capsuleB = *((Capsule*)shapeB);

	D3DXVECTOR3 iden[3] =
	{
		D3DXVECTOR3(1, 0, 0),
		D3DXVECTOR3(0, 1, 0),
		D3DXVECTOR3(0, 0, 1)
	};

	D3DXMATRIX matrixA = GameMath::Get3x3(transformA);
	D3DXMATRIX matrixAinv = GameMath::Transpose3x3(matrixA);

	D3DXVECTOR3 directionB = GameMath::GetCol3x3(0, transformB);
	D3DXVECTOR3 translationB = GameMath::GetTranslation(transformB);

	D3DXVECTOR3 capsDirection = GameMath::Rotate(directionB, matrixAinv);
	D3DXVECTOR3 absCapsDirection = GameMath::AbsPerElem(capsDirection);
	D3DXVECTOR3 offsetAB = GameMath::Rotate(translationB - GameMath::GetTranslation(transformA), matrixAinv);

	BoxCapsSepAxisType axisType;
	D3DXVECTOR3 axisA;
	float maxGap;
	int faceDimA = 0;
	int edgeDimA = 0;

	D3DXVECTOR3 gapsA = GameMath::AbsPerElem(offsetAB) - boxA.half - absCapsDirection*capsuleB.halfLength;

	AaxisTest(0, x, true);
	AaxisTest(1, y, false);
	AaxisTest(2, z, false);

	D3DXVECTOR3 lsqrs, projOffset, projAhalf;

	D3DXMATRIX crossProdMat = GameMath::IdentityMatrix() * GameMath::CrossMatrix(capsDirection);
	D3DXMATRIX crossProdMatT = GameMath::IdentityMatrix() * GameMath::CrossMatrix(-capsDirection);

	lsqrs = GameMath::MulPerElem(GameMath::GetCol3x3(0, crossProdMatT), GameMath::GetCol3x3(0, crossProdMatT)) +
			GameMath::MulPerElem(GameMath::GetCol3x3(1, crossProdMatT), GameMath::GetCol3x3(1, crossProdMatT)) +
			GameMath::MulPerElem(GameMath::GetCol3x3(2, crossProdMatT), GameMath::GetCol3x3(2, crossProdMatT));

	projOffset = GameMath::Rotate(offsetAB, crossProdMatT);

	projAhalf = GameMath::Rotate(boxA.half, GameMath::AbsPerElem(crossProdMatT));
	D3DXVECTOR3 gapsAxB = GameMath::AbsPerElem(projOffset) - projAhalf;

	CrossAxisTest(0, x);
	CrossAxisTest(1, y);
	CrossAxisTest(2, z);

	if (D3DXVec3Dot(&axisA, &offsetAB) < 0.0f)
	{
		axisA = -axisA;
	}
	
	int dimA[3];
	if (axisType == CROSS_AXIS)
	{
		D3DXVECTOR3 absAxisA = GameMath::AbsPerElem(axisA);
		dimA[1] = edgeDimA;

		switch (edgeDimA)
		{
		case 0:
			if (absAxisA.y > absAxisA.z)
			{
				dimA[0] = 2;
				dimA[2] = 1;
			}
			else
			{
				dimA[0] = 1;
				dimA[2] = 2;
			}
			break;
		case 1:
			if (absAxisA.z > absAxisA.x)
			{
				dimA[0] = 0;
				dimA[2] = 2;
			}
			else
			{
				dimA[0] = 2;
				dimA[2] = 0;
			}

			break;

		case 2:
			if (absAxisA.x > absAxisA.z)
			{
				dimA[0] = 1;
				dimA[2] = 0;
			}
			else
			{
				dimA[0] = 0;
				dimA[2] = 1;
			}

			break;

		}

	}
	else
	{
		dimA[2] = faceDimA;
		dimA[0] = (faceDimA + 1) % 3;
		dimA[1] = (faceDimA + 2) % 3;
	}

	D3DXMATRIX apermCol = GameMath::IdentityMatrix();
	GameMath::SetCol3x3(0, apermCol, iden[dimA[0]]);
	GameMath::SetCol3x3(1, apermCol, iden[dimA[1]]);
	GameMath::SetCol3x3(2, apermCol, iden[dimA[2]]);


	D3DXMATRIX apermRow = GameMath::Transpose3x3(apermCol);

	D3DXVECTOR3 offsetABperm = GameMath::Rotate(offsetAB, apermRow);
	D3DXVECTOR3 halfAperm = GameMath::Rotate(boxA.half, apermRow);

	D3DXVECTOR3 signsAperm = GameMath::CopySignPerElem(GameMath::GetVec3(1.0f), GameMath::Rotate(axisA, apermRow));
	D3DXVECTOR3 scalesAperm = GameMath::MulPerElem(signsAperm, halfAperm);
	D3DXVECTOR3 capsDirectionPerm = GameMath::Rotate(capsDirection, apermRow);
	float signB = (-D3DXVec3Dot(&capsDirection, &axisA) > 0.0f) ? 1.0f : -1.0f;
	float scaleB = signB*capsuleB.halfLength;

	offsetABperm.z = offsetABperm.z - scalesAperm.z;

	if (maxGap < 0.0f)
	{
		offsetABperm -= GameMath::Rotate(axisA, apermRow)*maxGap*1.01f;
	}

	D3DXVECTOR3 closestPtsVecPerm;
	D3DXVECTOR3 localPointAPerm;
	float minDistSqr;
	float segmentParamB;
	bool done;

	localPointAPerm.z = scalesAperm.z;
	scalesAperm.z = 0.0f;

	D3DXVECTOR3 hAperm = halfAperm;
	int otherFaceDimA;
	if (axisType == CROSS_AXIS)
	{
		EdgeEdgeTests(done, minDistSqr, closestPtsVecPerm, localPointAPerm, segmentParamB,
			otherFaceDimA, hAperm, capsuleB.halfLength, offsetABperm, capsDirectionPerm, signsAperm, scalesAperm, true);

		if (!done)
		{
			VertexBFaceATests(done, minDistSqr, closestPtsVecPerm, localPointAPerm, segmentParamB,
				hAperm, offsetABperm, capsDirectionPerm, signB, scaleB, false);
		}

	}
	else
	{
		VertexBFaceATests(done, minDistSqr, closestPtsVecPerm, localPointAPerm, segmentParamB,
			hAperm, offsetABperm, capsDirectionPerm, signB, scaleB, true);

		if (!done)
		{
			EdgeEdgeTests(done, minDistSqr, closestPtsVecPerm, localPointAPerm, segmentParamB,
				otherFaceDimA, hAperm, capsuleB.halfLength, offsetABperm, capsDirectionPerm, signsAperm, scalesAperm, false);

		}

	}


	bool centerInside = (signsAperm.z * closestPtsVecPerm.z < 0.0f);
	if (centerInside || (minDistSqr < lenSqrTol))
	{

		normal = GameMath::Rotate(axisA, matrixA);
	}
	else
	{
		D3DXVECTOR3 closestPtsVec = GameMath::Rotate(closestPtsVecPerm, apermCol);
		normal = GameMath::Rotate(closestPtsVec*(1.0f / sqrtf(minDistSqr)), matrixA);
	}

	pointA = GameMath::Rotate(localPointAPerm, apermCol);

	pointB = GameMath::Rotate((directionB*segmentParamB - normal*capsuleB.radius), GameMath::Transpose3x3(transformB));

	if (centerInside)
	{
		return (-sqrtf(minDistSqr) - capsuleB.radius);
	}
	else
	{
		return (sqrtf(minDistSqr) - capsuleB.radius);
	}



}


