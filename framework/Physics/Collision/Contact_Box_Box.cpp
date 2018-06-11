#include"Contact.h"
#include"Box.h"
#include"../../Utility/Macro.h"
#include"../../Utility/GameMath.h"
enum BoxSepAxisType
{
	A_AXIS,B_AXIS,CROSS_AXIS
};

static const float voronoiTol = -1.0e-5f;


inline bool AxisTestFirst(int srcDim, float srcAxis, BoxSepAxisType srcType, const D3DXVECTOR3& iden,
	int& dstDim, D3DXVECTOR3& dstAxis, BoxSepAxisType& dstType,
	float& maxGap, float& gap, float distanceThreshold)
{
	maxGap = gap = srcAxis;
	if (gap > distanceThreshold)
		return true;
	dstType = srcType;
	dstDim = srcDim;
	dstAxis = iden;

	return false;
}

inline bool AxisTest(int srcDim, float srcAxis, BoxSepAxisType srcType, const D3DXVECTOR3& iden,
	int& dstDim, D3DXVECTOR3& dstAxis, BoxSepAxisType& dstType,
	float& maxGap, float& gap,float distanceThreshold)
{

	gap = srcAxis;

	if (gap > distanceThreshold)
		return true;

	else if (gap > maxGap)
	{
		maxGap = gap;
		dstType = srcType;
		dstDim = srcDim;
		dstAxis = iden;
	}
	

	return false;
}

inline bool CrossAxisTest(int srcDimA, int srcDimB, int srcAxis,const D3DXVECTOR3& iden,
	const D3DXMATRIX& srcMat,const D3DXMATRIX& srcGapsMat,const D3DXMATRIX& srclsqrs,
	int& dstDimA,int& dstDimB,D3DXVECTOR3& dstAxis,BoxSepAxisType& dstType,
	float& maxGap, float& gap, float distanceThreshold)
{
	const float lsqr_tolerance = 1.0e-30f;
	float lsqr;

	lsqr = srclsqrs.m[srcDimA][srcDimB];

	if (lsqr > lsqr_tolerance)
	{
		float recip = 1.0f / sqrt(lsqr);
		gap = srcGapsMat.m[srcDimA][srcAxis] * recip;

		if (gap>distanceThreshold)
			return true;

		if (gap > maxGap)
		{
			maxGap = gap;
			dstType = CROSS_AXIS;
			dstDimA = srcDimA;
			dstDimB = srcDimB;
			dstAxis = GameMath::Cross(iden, D3DXVECTOR3(srcMat.m[srcDimB]))*recip;
		}

	}

	return false;
}

inline float EdgeTest(
	int edgeAC,
	int edgeAD,
	int edgeBC,
	int edgeBD,
	bool& inVoronoi,
	float& tA,
	float& tB,
	const D3DXVECTOR3& hA,
	const D3DXVECTOR3& hB,
	const D3DXVECTOR3& faceOffsetAB,
	const D3DXVECTOR3& faceOffsetBA,
	const D3DXMATRIX& matrixAB,
	const D3DXMATRIX& matrixBA,
	const D3DXVECTOR3& signsA,
	const D3DXVECTOR3& signsB,
	const D3DXVECTOR3& scalesA,
	const D3DXVECTOR3& scalesB)
{

	D3DXVECTOR3 edgeOffsetAB;
	D3DXVECTOR3 edgeOffsetBA;
	
	edgeOffsetAB = faceOffsetAB + GameMath::GetCol3x3(edgeBC, matrixAB)*scalesB[edgeBC];
	edgeOffsetAB[edgeAC] = edgeOffsetAB[edgeAC] - scalesA[edgeAC];

	edgeOffsetBA = faceOffsetBA + GameMath::GetCol3x3(edgeAC, matrixBA)*scalesA[edgeAC];
	edgeOffsetBA[edgeBC] = edgeOffsetBA[edgeBC] - scalesB[edgeBC];

	float dirDot = GameMath::GetCol3x3(edgeBD, matrixAB)[edgeAD];
	float denom = 1.0f - dirDot*dirDot;
	float edgeOffsetABad = edgeOffsetAB[edgeAD];
	float edgeOffsetBAbd = edgeOffsetBA[edgeBD];


	tA = (denom == 0.0f) ? 
		0.0f : 
		(edgeOffsetABad + edgeOffsetBAbd*dirDot) / denom;

	if (tA < -hA[edgeAD])
	{
		tA = -hA[edgeAD];
	}
	else if (tA>hA[edgeAD])
	{
		tA = hA[edgeAD];
	}


	tB = tA * dirDot + edgeOffsetBAbd;

	if (tB < -hB[edgeBD])
	{
		tB = -hB[edgeBD];
		tA = tB * dirDot + edgeOffsetABad;

		if (tA < -hA[edgeAD])
		{
			tA = -hA[edgeAD];
		}
		else if (tA>hA[edgeAD])
		{
			tA = hA[edgeAD];
		}
	}
	else if (tB > hB[edgeBD])
	{
		tB = hB[edgeBD];
		tA = tB * dirDot + edgeOffsetABad;

		if (tA < -hA[edgeAD])
		{
			tA = -hA[edgeAD];
		}
		else if (tA>hA[edgeAD])
		{
			tA = hA[edgeAD];
		}
	}

	D3DXVECTOR3 edgeOffAB = GameMath::MulPerElem(edgeOffsetAB + GameMath::GetCol3x3(edgeBD, matrixAB)*tB, signsA);
	D3DXVECTOR3 edgeOffBA = GameMath::MulPerElem(edgeOffsetBA + GameMath::GetCol3x3(edgeAD, matrixBA)*tA, signsB);


	inVoronoi = (edgeOffAB[edgeAC] >= voronoiTol * edgeOffAB[2]) &&
				(edgeOffAB[2] >= voronoiTol * edgeOffAB[edgeAC]) &&
				(edgeOffBA[edgeBC] >= voronoiTol * edgeOffBA[2]) &&
				(edgeOffBA[2] >= voronoiTol * edgeOffBA[edgeBC]);

	edgeOffAB[edgeAD] -= tA;

	return D3DXVec3Dot(&edgeOffAB, &edgeOffAB);	
}


float EdgeTest0101(
	bool& inVoronoi,
	float& tA,
	float& tB,
	const D3DXVECTOR3& hA,
	const D3DXVECTOR3& hB,
	const D3DXVECTOR3& faceOffsetAB,
	const D3DXVECTOR3& faceOffsetBA,
	const D3DXMATRIX& matrixAB,
	const D3DXMATRIX& matrixBA,
	D3DXVECTOR3 signsA,
	D3DXVECTOR3 signsB,
	D3DXVECTOR3 scalesA,
	D3DXVECTOR3 scalesB
	)
{
	return EdgeTest(0, 1, 0, 1,inVoronoi, tA, tB, hA, hB, faceOffsetAB, 
		faceOffsetBA, matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);
}


float EdgeTest0110(
	bool& inVoronoi,
	float& tA,
	float& tB,
	const D3DXVECTOR3& hA,
	const D3DXVECTOR3& hB,
	const D3DXVECTOR3& faceOffsetAB,
	const D3DXVECTOR3& faceOffsetBA,
	const D3DXMATRIX& matrixAB,
	const D3DXMATRIX& matrixBA,
	D3DXVECTOR3 signsA,
	D3DXVECTOR3 signsB,
	D3DXVECTOR3 scalesA,
	D3DXVECTOR3 scalesB
	)
{
	return EdgeTest(0, 1, 1, 0, inVoronoi, tA, tB, hA, hB, faceOffsetAB,
		faceOffsetBA, matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);
}


float EdgeTest1001(
	bool& inVoronoi,
	float& tA,
	float& tB,
	const D3DXVECTOR3& hA,
	const D3DXVECTOR3& hB,
	const D3DXVECTOR3& faceOffsetAB,
	const D3DXVECTOR3& faceOffsetBA,
	const D3DXMATRIX& matrixAB,
	const D3DXMATRIX& matrixBA,
	D3DXVECTOR3 signsA,
	D3DXVECTOR3 signsB,
	D3DXVECTOR3 scalesA,
	D3DXVECTOR3 scalesB
	)
{
	return EdgeTest(1, 0, 0, 1, inVoronoi, tA, tB, hA, hB, faceOffsetAB,
		faceOffsetBA, matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);
}


float EdgeTest1010(
	bool& inVoronoi,
	float& tA,
	float& tB,
	const D3DXVECTOR3& hA,
	const D3DXVECTOR3& hB,
	const D3DXVECTOR3& faceOffsetAB,
	const D3DXVECTOR3& faceOffsetBA,
	const D3DXMATRIX& matrixAB,
	const D3DXMATRIX& matrixBA,
	D3DXVECTOR3 signsA,
	D3DXVECTOR3 signsB,
	D3DXVECTOR3 scalesA,
	D3DXVECTOR3 scalesB
	)
{
	return EdgeTest(1, 0, 1, 0, inVoronoi, tA, tB, hA, hB, faceOffsetAB,
		faceOffsetBA, matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);
}



inline void SetNewMin(int edgeAC, int edgeAD, int edgeBC, int edgeBD,
	const D3DXVECTOR3& srcScalesA, const D3DXVECTOR3& srcScalesB,
	const float& tA, const float& tB,
	const int& srcDimA, const int& srcDimB,
	D3DXVECTOR3& dstPointA, D3DXVECTOR3& dstPointB,
	int& dstDimA, int& dstDimB,
	const float& srcSqr,float& dstSqr)
{
	dstSqr = srcSqr;

	dstPointA[edgeAC] = srcScalesA[edgeAC];
	dstPointA[edgeAD] = tA;

	dstPointB[edgeBC] = srcScalesB[edgeBC];
	dstPointB[edgeBD] = tB;

	dstDimA = srcDimA;
	dstDimB = srcDimB;

}

void EdgeTests(
	bool& done,
	float& minDistSqr,
	D3DXVECTOR3& localPointA,
	D3DXVECTOR3& localPointB,
	int& otherFaceDimA,
	int& otherFaceDimB,
	const D3DXVECTOR3& hA,
	const D3DXVECTOR3& hB,
	const D3DXVECTOR3& faceOffsetAB,
	const D3DXVECTOR3& faceOffsetBA,
	const D3DXMATRIX& matrixAB,
	const D3DXMATRIX& matrixBA,
	D3DXVECTOR3 signsA,
	D3DXVECTOR3 signsB,
	D3DXVECTOR3 scalesA,
	D3DXVECTOR3 scalesB,
	bool first
	)
{
	float distSqr;
	float tA, tB;

	int testOtherFaceDimA, testOtherFaceDimB;

	testOtherFaceDimA = 0;
	testOtherFaceDimB = 0;

	distSqr = EdgeTest0101(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA, 
								matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (first)
	{
		SetNewMin(0, 1, 0, 1,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);

	}
	else
	{
		if (distSqr < minDistSqr)
			SetNewMin(0, 1, 0, 1,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}

	if (done)
		return;

	signsA.x = -signsA.x;
	scalesA.x = -scalesA.x;

	distSqr = EdgeTest0101(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA,
								matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (distSqr < minDistSqr)
	{
		SetNewMin(0, 1, 0, 1,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}

	if (done)
		return;

	signsB.x = -signsB.x;
	scalesB.x = -scalesB.x;

	distSqr = EdgeTest0101(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA,
								matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (distSqr < minDistSqr)
	{
		SetNewMin(0, 1, 0, 1,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}


	if (done)
		return;

	signsA.x = -signsA.x;
	scalesA.x = -scalesA.x;

	distSqr = EdgeTest0101(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA,
								matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (distSqr < minDistSqr)
	{
		SetNewMin(0, 1, 0, 1,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}


	if (done)
		return;


	testOtherFaceDimA = 1;
	testOtherFaceDimB = 0;

	signsB.x = -signsB.x;
	scalesB.x = -scalesB.x;

	distSqr = EdgeTest1001(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA,
		matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (distSqr < minDistSqr)
	{
		SetNewMin(1,0,0,1,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}

	if (done)
		return;

	signsA.y = -signsA.y;
	scalesA.y = -scalesA.y;

	distSqr = EdgeTest1001(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA,
		matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (distSqr < minDistSqr)
	{

		SetNewMin(1, 0, 0, 1,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}

	if (done)
		return;


	signsB.x = -signsB.x;
	scalesB.x = -scalesB.x;


	distSqr = EdgeTest1001(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA,
		matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (distSqr < minDistSqr)
	{
		SetNewMin(1, 0, 0, 1,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}

	if (done)
		return;


	signsA.y = -signsA.y;
	scalesA.y = -scalesA.y;

	distSqr = EdgeTest1001(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA,
		matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (distSqr < minDistSqr)
	{

		SetNewMin(1, 0, 0, 1,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}

	if (done)
		return;

	testOtherFaceDimA = 0;
	testOtherFaceDimB = 1;
	signsB.x = -signsB.x;
	scalesB.x = -scalesB.x;

	distSqr = EdgeTest0110(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA,
		matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (distSqr < minDistSqr)
	{

		SetNewMin(0, 1, 1, 0,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}

	if (done)
		return;

	signsA.x = -signsA.x;
	scalesA.x = -scalesA.x;

	distSqr = EdgeTest0110(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA,
		matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (distSqr < minDistSqr)
	{
		SetNewMin(0, 1, 1, 0,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}

	if (done)
		return;

	signsB.y = -signsB.y;
	scalesB.y = -signsB.y;

	distSqr = EdgeTest0110(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA,
		matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (distSqr < minDistSqr)
	{
		SetNewMin(0, 1, 1, 0,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}

	if (done)
		return;


	signsA.x = -signsA.x;
	scalesA.x = -scalesA.x;

	distSqr = EdgeTest0110(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA,
		matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (distSqr < minDistSqr)
	{
		SetNewMin(0, 1, 1, 0,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}

	if (done)
		return;


	testOtherFaceDimA = 1;
	testOtherFaceDimB = 1;

	signsB.y = -signsB.y;
	scalesB.y = -scalesB.y;

	distSqr = EdgeTest1010(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA,
		matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (distSqr < minDistSqr)
	{
		SetNewMin(1, 0, 1, 0,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}

	if (done)
		return;

	signsA.y = -signsA.y;
	scalesA.y = -scalesA.y;

	distSqr = EdgeTest1010(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA,
		matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (distSqr < minDistSqr)
	{
		SetNewMin(1, 0, 1, 0,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}

	if (done)
		return;



	signsB.y = -signsB.y;
	scalesB.y = -scalesB.y;

	distSqr = EdgeTest1010(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA,
		matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (distSqr < minDistSqr)
	{
		SetNewMin(1, 0, 1, 0,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}

	if (done)
		return;


	signsA.y = -signsA.y;
	scalesA.y = -scalesA.y;

	distSqr = EdgeTest1010(done, tA, tB, hA, hB, faceOffsetAB, faceOffsetBA,
		matrixAB, matrixBA, signsA, signsB, scalesA, scalesB);

	if (distSqr < minDistSqr)
	{
		SetNewMin(1, 0, 1, 0,
			scalesA, scalesB, tA, tB, testOtherFaceDimA, testOtherFaceDimB,
			localPointA, localPointB, otherFaceDimA, otherFaceDimB, distSqr, minDistSqr);
	}


}


inline float VertexAFaceBTest(
	bool& inVoronoi,
	float& t0,
	float& t1,
	const D3DXVECTOR3& hB,
	D3DXVECTOR3 faceOffsetAB,
	D3DXVECTOR3 faceOffsetBA,
	const D3DXMATRIX& matrixAB,
	const D3DXMATRIX& matrixBA,
	D3DXVECTOR3 signsA,
	D3DXVECTOR3 scalesA)
{
	D3DXVECTOR3 corner = faceOffsetBA + GameMath::GetCol3x3(0, matrixBA)*scalesA.x + GameMath::GetCol3x3(1, matrixBA)*scalesA.y;

	t0 = corner.x;
	t1 = corner.y;

	if (t0 > hB.x)
		t0 = hB.x;
	else if (t0 < -hB.x)
		t0 = -hB.x;

	if (t1>hB.y)
		t1 = hB.y;
	else if (t1 < -hB.y)
		t1 = -hB.y;

	D3DXVECTOR3 facePointA = GameMath::MulPerElem(
		faceOffsetAB +
		GameMath::GetCol3x3(0, matrixAB)*t0 +
		GameMath::GetCol3x3(1, matrixAB)*t1 -
		scalesA, signsA);

	inVoronoi = ((facePointA.x >= voronoiTol*facePointA.z) &&
				 (facePointA.y >= voronoiTol*facePointA.x) &&
				 (facePointA.z >= voronoiTol*facePointA.y));

	return SQR(corner.x - t0) + SQR(corner.y - t1) + SQR(corner.z);


}

#define VertexAFaceBSetNewMin()	   \
{								   \
	minDistSqr = distSqr;		   \
	localPointB.x = t0;			   \
	localPointB.y = t1;			   \
	localPointA.x = scalesA.x;	   \
	localPointA.y = scalesA.y;	   \
}								   \

void VertexAFaceBTests(
	bool& done,
	float& minDistSqr,
	D3DXVECTOR3& localPointA,
	D3DXVECTOR3& localPointB,
	const D3DXVECTOR3& hB,
	D3DXVECTOR3 faceOffsetAB,
	D3DXVECTOR3 faceOffsetBA,
	const D3DXMATRIX& matrixAB,
	const D3DXMATRIX& matrixBA,
	D3DXVECTOR3 signsA,
	D3DXVECTOR3 scalesA,
	bool first)
{
	float t0, t1;
	float distSqr;

	distSqr = VertexAFaceBTest(done, t0, t1, hB, faceOffsetAB, faceOffsetBA, matrixAB, matrixBA, signsA, scalesA);
	if (first)
	{
		VertexAFaceBSetNewMin();
	}
	else if (distSqr < minDistSqr)
	{
		VertexAFaceBSetNewMin();
	}

	if (done)
		return;

	signsA.x = -signsA.x;
	scalesA.x = -scalesA.x;

	distSqr = VertexAFaceBTest(done, t0, t1, hB, faceOffsetAB, faceOffsetBA, matrixAB, matrixBA, signsA, scalesA);

	if (distSqr>minDistSqr)
	{
		VertexAFaceBSetNewMin();
	}

	if (done)
		return;

	signsA.y = -signsA.y;
	scalesA.y = -scalesA.y;

	distSqr = VertexAFaceBTest(done, t0, t1, hB, faceOffsetAB, faceOffsetBA, matrixAB, matrixBA, signsA, scalesA);

	if (distSqr < minDistSqr)
	{
		VertexAFaceBSetNewMin();
	}

	if (done)
		return;

	signsA.x = -signsA.x;
	scalesA.x = -scalesA.x;

	distSqr = VertexAFaceBTest(done, t0, t1, hB, faceOffsetAB, faceOffsetBA, matrixAB, matrixBA, signsA, scalesA);

	if (distSqr < minDistSqr)
	{
		VertexAFaceBSetNewMin();
	}


}

inline float VertexBFaceATest(bool& inVoronoi,
	float& t0, float& t1,
	const D3DXVECTOR3& hA,
	D3DXVECTOR3 faceOffsetAB, D3DXVECTOR3 faceOffsetBA,
	const D3DXMATRIX& matrixAB, const D3DXMATRIX& matrixBA,
	D3DXVECTOR3 signsB, D3DXVECTOR3 scalesB)
{

	D3DXVECTOR3 corner =
		faceOffsetAB + GameMath::GetCol3x3(0, matrixAB)*scalesB.x + GameMath::GetCol3x3(1, matrixAB)*scalesB.y;

	t0 = corner.x;
	t1 = corner.y;

	if (t0 > hA.x)
		t0 = hA.x;
	else if (t0 < -hA.x)
		t0 = -hA.x;

	if (t1>hA.y)
		t1 = hA.y;
	else if (t1 < -hA.y)
		t1 = -hA.y;

	D3DXVECTOR3 facePointB =
		GameMath::MulPerElem(
		faceOffsetBA +
		GameMath::GetCol3x3(0, matrixBA)*t0 +
		GameMath::GetCol3x3(1, matrixBA)*t1 -
		scalesB, signsB);

	inVoronoi = ((facePointB.x >= voronoiTol*facePointB.z) &&
				 (facePointB.y >= voronoiTol*facePointB.x) &&
				 (facePointB.z >= voronoiTol*facePointB.y));
	return SQR(corner.x - t0) + SQR(corner.y - t1) + SQR(corner.z);

}

#define VertexBFaceASetNewMin()		\
{									\
	minDistSqr = distSqr;			\
	localPointA.x = t0;				\
	localPointA.y = t1;				\
	localPointB.x = scalesB.x;		\
	localPointB.y = scalesB.y;		\
}									\

void VertexBFaceATests(
	bool& done,
	float& minDistSqr,
	D3DXVECTOR3& localPointA,
	D3DXVECTOR3& localPointB,
	const D3DXVECTOR3& hA,
	D3DXVECTOR3& faceOffsetAB,
	D3DXVECTOR3& faceOffsetBA,
	const D3DXMATRIX& matrixAB,
	const D3DXMATRIX& matrixBA,
	D3DXVECTOR3 signsB,
	D3DXVECTOR3 scalesB,
	bool first)
{
	float t0, t1;
	float distSqr;

	distSqr = VertexBFaceATest(done, t0, t1, hA, faceOffsetAB, faceOffsetBA, matrixAB, matrixBA, signsB, scalesB);

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

	signsB.x = -signsB.x;
	scalesB.x = -scalesB.x;

	distSqr = VertexBFaceATest(done, t0, t1, hA, faceOffsetAB, faceOffsetBA, matrixAB, matrixBA, signsB, scalesB);
	
	if (distSqr < minDistSqr)
	{
		VertexBFaceASetNewMin();
	}


	if (done)
		return;

	signsB.y = -signsB.y;
	scalesB.y = -scalesB.y;

	distSqr = VertexBFaceATest(done, t0, t1, hA, faceOffsetAB, faceOffsetBA, matrixAB, matrixBA, signsB, scalesB);

	if (distSqr < minDistSqr)
	{
		VertexBFaceASetNewMin();
	}

	if (done)
		return;

	signsB.x = -signsB.x;
	scalesB.x = -scalesB.x;
	distSqr = VertexBFaceATest(done, t0, t1, hA, faceOffsetAB, faceOffsetBA, matrixAB, matrixBA, signsB, scalesB);
	if (distSqr < minDistSqr)
	{
		VertexBFaceASetNewMin();
	}


}




float ContactBoxBox(
	D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
	void* shapeA, const D3DXMATRIX& transformA,
	void* shapeB, const D3DXMATRIX& transformB,
	float distanceThreshold)
{

	Box& boxA = *((Box*)shapeA);
	Box& boxB = *((Box*)shapeB);

	D3DXVECTOR3 iden[3] =
	{
		D3DXVECTOR3(1.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 1.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 1.0f),
	};


	D3DXMATRIX transformAB, transformBA;	//使うのは3x4
	D3DXMATRIX matrixAB, matrixBA;			//使うのは3x3
	D3DXVECTOR3 offsetAB, offsetBA;

	transformAB = transformB* GameMath::OrthoInverse3x4(transformA);
	transformBA = GameMath::OrthoInverse3x4(transformAB);

	matrixAB = GameMath::Get3x3(transformAB);
	offsetAB = GameMath::GetTranslation(transformAB);
	matrixBA = GameMath::Get3x3(transformBA);
	offsetBA = GameMath::GetTranslation(transformBA);

	D3DXMATRIX absMatrixAB = GameMath::AbsPerElem(matrixAB);
	D3DXMATRIX absMatrixBA = GameMath::AbsPerElem(matrixBA);

	BoxSepAxisType axisType;
	D3DXVECTOR3 axisA = GameMath::GetVec3(0.0f);
	D3DXVECTOR3 axisB = GameMath::GetVec3(0.0f);
	float gap, maxGap;
	int faceDimA, faceDimB, edgeDimA, edgeDimB;
	faceDimA = faceDimB = edgeDimA = edgeDimB = 0;

	D3DXVECTOR3 gapsA = GameMath::AbsPerElem(offsetAB) - boxA.half - GameMath::Rotate(boxB.half, absMatrixAB);

	if (AxisTestFirst(0, gapsA.x,A_AXIS,iden[0],faceDimA,axisA,axisType,maxGap,gap,distanceThreshold))
		return gap;
	if (AxisTest(1, gapsA.y, A_AXIS, iden[1], faceDimA, axisA, axisType, maxGap, gap, distanceThreshold))
		return gap;
	if (AxisTest(2, gapsA.z, A_AXIS, iden[2], faceDimA, axisA, axisType, maxGap, gap, distanceThreshold))
		return gap;
		

	D3DXVECTOR3 gapsB = GameMath::AbsPerElem(offsetBA) - boxB.half - GameMath::Rotate(boxA.half, absMatrixBA);

	if (AxisTest(0, gapsB.x, B_AXIS, iden[0], faceDimB, axisB, axisType, maxGap, gap, distanceThreshold))
		return gap;
	if (AxisTest(1, gapsB.y, B_AXIS, iden[1], faceDimB, axisB, axisType, maxGap, gap, distanceThreshold))
		return gap;
	if (AxisTest(2, gapsB.z, B_AXIS, iden[2], faceDimB, axisB, axisType, maxGap, gap, distanceThreshold))
		return gap;


	//0割り対策 0.00001加算
	absMatrixAB = GameMath::Add3x3(absMatrixAB, GameMath::Get3x3(1.0e-5f));
	absMatrixBA = GameMath::Add3x3(absMatrixBA, GameMath::Get3x3(1.0e-5f));

	D3DXMATRIX lsqrs, projOffset, projAhalf, projBhalf;

	//lsqrsの各列にベクトルセット
	GameMath::SetCol3x3(0, lsqrs,
		GameMath::MulPerElem(GameMath::GetVec3(matrixBA, 2), GameMath::GetVec3(matrixBA, 2)) +
		GameMath::MulPerElem(GameMath::GetVec3(matrixBA, 1), GameMath::GetVec3(matrixBA, 1)));

	GameMath::SetCol3x3(1, lsqrs,
		GameMath::MulPerElem(GameMath::GetVec3(matrixBA, 2), GameMath::GetVec3(matrixBA, 2)) +
		GameMath::MulPerElem(GameMath::GetVec3(matrixBA, 0), GameMath::GetVec3(matrixBA, 0)));

	GameMath::SetCol3x3(2, lsqrs,
		GameMath::MulPerElem(GameMath::GetVec3(matrixBA, 1), GameMath::GetVec3(matrixBA, 1)) +
		GameMath::MulPerElem(GameMath::GetVec3(matrixBA, 0), GameMath::GetVec3(matrixBA, 0)));

	//projOffsetの各列にベクトルセット
	GameMath::SetCol3x3(0, projOffset,
		GameMath::GetVec3(matrixBA, 1)*offsetAB.z - GameMath::GetVec3(matrixBA, 2)*offsetAB.y);

	GameMath::SetCol3x3(1, projOffset,
		GameMath::GetVec3(matrixBA, 2)*offsetAB.x - GameMath::GetVec3(matrixBA, 0)*offsetAB.z);

	GameMath::SetCol3x3(2, projOffset,
		GameMath::GetVec3(matrixBA, 0)*offsetAB.y - GameMath::GetVec3(matrixBA, 1)*offsetAB.x);

	//projAhalfの各列にベクトルセット
	GameMath::SetCol3x3(0, projAhalf,
		GameMath::GetVec3(absMatrixBA, 1)*boxA.half.z + GameMath::GetVec3(absMatrixBA, 2)*boxA.half.y);

	GameMath::SetCol3x3(1, projAhalf,
		GameMath::GetVec3(absMatrixBA, 2)*boxA.half.x + GameMath::GetVec3(absMatrixBA, 0)*boxA.half.z);

	GameMath::SetCol3x3(2, projAhalf,
		GameMath::GetVec3(absMatrixBA, 0)*boxA.half.y + GameMath::GetVec3(absMatrixBA, 1)*boxA.half.x);



	//projBhalfの各列にベクトルセット
	GameMath::SetCol3x3(0, projBhalf,
		GameMath::GetVec3(absMatrixAB, 1)*boxB.half.z + GameMath::GetVec3(absMatrixAB, 2)*boxB.half.y);

	GameMath::SetCol3x3(1, projBhalf,
		GameMath::GetVec3(absMatrixAB, 2)*boxB.half.x + GameMath::GetVec3(absMatrixAB, 0)*boxB.half.z);

	GameMath::SetCol3x3(2, projBhalf,
		GameMath::GetVec3(absMatrixAB, 0)*boxB.half.y + GameMath::GetVec3(absMatrixAB, 1)*boxB.half.x);


	// |projOffset| - projAhalf - projBhalfの転置行列
	D3DXMATRIX gapsAxB = GameMath::Sub3x3(
		GameMath::Sub3x3(GameMath::AbsPerElem(projOffset), projAhalf),
		GameMath::Transpose3x3(projBhalf));

	CrossAxisTest(0, 0, 0, iden[0], matrixAB, gapsAxB, lsqrs, edgeDimA, edgeDimB, axisA, axisType, maxGap, gap, distanceThreshold);
	CrossAxisTest(0, 1, 1, iden[0], matrixAB, gapsAxB, lsqrs, edgeDimA, edgeDimB, axisA, axisType, maxGap, gap, distanceThreshold);
	CrossAxisTest(0, 2, 2, iden[0], matrixAB, gapsAxB, lsqrs, edgeDimA, edgeDimB, axisA, axisType, maxGap, gap, distanceThreshold);
	CrossAxisTest(1, 0, 0, iden[1], matrixAB, gapsAxB, lsqrs, edgeDimA, edgeDimB, axisA, axisType, maxGap, gap, distanceThreshold);
	CrossAxisTest(1, 1, 1, iden[1], matrixAB, gapsAxB, lsqrs, edgeDimA, edgeDimB, axisA, axisType, maxGap, gap, distanceThreshold);
	CrossAxisTest(1, 2, 2, iden[1], matrixAB, gapsAxB, lsqrs, edgeDimA, edgeDimB, axisA, axisType, maxGap, gap, distanceThreshold);
	CrossAxisTest(2, 0, 0, iden[2], matrixAB, gapsAxB, lsqrs, edgeDimA, edgeDimB, axisA, axisType, maxGap, gap, distanceThreshold);
	CrossAxisTest(2, 1, 1, iden[2], matrixAB, gapsAxB, lsqrs, edgeDimA, edgeDimB, axisA, axisType, maxGap, gap, distanceThreshold);
	CrossAxisTest(2, 2, 2, iden[2], matrixAB, gapsAxB, lsqrs, edgeDimA, edgeDimB, axisA, axisType, maxGap, gap, distanceThreshold);

	int dimA[3], dimB[3];


	switch (axisType)
	{

	case A_AXIS:
	{
		if (D3DXVec3Dot(&axisA, &offsetAB) < 0.0f)
		{
			axisA = -axisA;
		}
		axisB = GameMath::Rotate(-axisA, matrixBA);
		D3DXVECTOR3 absAxisB = GameMath::AbsPerElem(axisB);

		if ((absAxisB.x > absAxisB.y) && (absAxisB.x > absAxisB.z))
		{
			faceDimB = 0;
		}
		else if (absAxisB.y>absAxisB.z)
		{
			faceDimB = 1;
		}
		else
		{
			faceDimB = 2;
		}	

		dimA[2] = faceDimA;
		dimA[0] = (faceDimA + 1) % 3;
		dimA[1] = (faceDimA + 2) % 3;
		dimB[2] = faceDimB;
		dimB[0] = (faceDimB + 1) % 3;
		dimB[1] = (faceDimB + 2) % 3;
	}
	break;
	case B_AXIS:
	{
		if (D3DXVec3Dot(&axisB, &offsetBA) < 0.0f)
		{
			axisB = -axisB;
		}
		axisA = GameMath::Rotate(-axisB, matrixAB);

		D3DXVECTOR3 absAxisA = GameMath::AbsPerElem(axisA);

		if ((absAxisA.x > absAxisA.y) && absAxisA.x > absAxisA.z)
		{
			faceDimA = 0;
		}
		else if (absAxisA.y > absAxisA.z)
		{
			faceDimA = 1;
		}
		else
		{
			faceDimA = 2;
		}

		dimA[2] = faceDimA;
		dimA[0] = (faceDimA + 1) % 3;
		dimA[1] = (faceDimA + 2) % 3;
		dimB[2] = faceDimB;
		dimB[0] = (faceDimB + 1) % 3;
		dimB[1] = (faceDimB + 2) % 3;
	}
	break;
	case CROSS_AXIS:
	{
		if (D3DXVec3Dot(&axisA, &offsetAB) < 0.0f)
		{
			axisA = -axisA;
		}
		axisB = GameMath::Rotate(-axisA, matrixBA);

		D3DXVECTOR3 absAxisA = GameMath::AbsPerElem(axisA);
		D3DXVECTOR3 absAxisB = GameMath::AbsPerElem(axisB);

		dimA[1] = edgeDimA;
		dimB[1] = edgeDimB;

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
			if (absAxisA.x > absAxisA.y)
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

		switch (edgeDimB)
		{
		case 0:
			if (absAxisB.y > absAxisB.z)
			{
				dimB[0] = 2;
				dimB[2] = 1;
			}
			else
			{
				dimB[0] = 1;
				dimB[2] = 2;
			}
			break;
		case 1:
			if (absAxisB.z > absAxisB.x)
			{
				dimB[0] = 0;
				dimB[2] = 2;
			}
			else
			{
				dimB[0] = 2;
				dimB[2] = 0;
			}
			break;
		case 2:
			if (absAxisB.x > absAxisB.y)
			{
				dimB[0] = 1;
				dimB[2] = 0;
			}
			else
			{
				dimB[0] = 0;
				dimB[2] = 1;
			}
			break;

		}

	}
	break;
	}

	D3DXMATRIX apermCol, bpermCol;
	
	//apermColの各列にベクトルセット
	GameMath::SetCol3x3(0, apermCol, iden[dimA[0]]);
	GameMath::SetCol3x3(1, apermCol, iden[dimA[1]]);
	GameMath::SetCol3x3(2, apermCol, iden[dimA[2]]);

	//bpermColの各列にベクトルセット
	GameMath::SetCol3x3(0, bpermCol, iden[dimB[0]]);
	GameMath::SetCol3x3(1, bpermCol, iden[dimB[1]]);
	GameMath::SetCol3x3(2, bpermCol, iden[dimB[2]]);

	D3DXMATRIX apermRow, bpermRow;
	apermRow = GameMath::Transpose3x3(apermCol);
	bpermRow = GameMath::Transpose3x3(bpermCol);

	//apermRow * matrixAB * bpermCol
	D3DXMATRIX matrixABperm = bpermCol * matrixAB * apermRow;
	D3DXMATRIX matrixBAperm = GameMath::Transpose3x3(matrixABperm);

	D3DXVECTOR3 offsetABperm, offsetBAperm;

	offsetABperm = GameMath::Rotate(offsetAB, apermRow);
	offsetBAperm = GameMath::Rotate(offsetBA, bpermRow);

	D3DXVECTOR3 halfAperm, halfBperm;

	halfAperm = GameMath::Rotate(boxA.half, apermRow);
	halfBperm = GameMath::Rotate(boxB.half, bpermRow);

	D3DXVECTOR3 signsAperm, signsBperm, scalesAperm, scalesBperm, faceOffsetABperm, faceOffsetBAperm;

	signsAperm = GameMath::CopySignPerElem(GameMath::GetVec3(1.0f), GameMath::Rotate(axisA, apermRow));
	signsBperm = GameMath::CopySignPerElem(GameMath::GetVec3(1.0f), GameMath::Rotate(axisB, bpermRow));

	scalesAperm = GameMath::MulPerElem(signsAperm, halfAperm);
	scalesBperm = GameMath::MulPerElem(signsBperm, halfBperm);

	faceOffsetABperm = offsetABperm + GameMath::GetCol3x3(2, matrixABperm)*scalesBperm.z;
	faceOffsetABperm.z = faceOffsetABperm.z - scalesAperm.z;

	faceOffsetBAperm = offsetBAperm + GameMath::GetCol3x3(2, matrixBAperm)*scalesAperm.z;
	faceOffsetBAperm.z = faceOffsetBAperm.z - scalesAperm.z;

	if (maxGap < 0.0f)
	{
		faceOffsetABperm -= GameMath::Rotate(axisA, apermRow)*maxGap*1.01f;
		faceOffsetBAperm -= GameMath::Rotate(axisB, bpermRow)*maxGap*1.01f;
	}


	D3DXVECTOR3 localPointAperm, localPointBperm;
	float minDistSqr;
	bool done;

	D3DXVECTOR3 hAperm(halfAperm), hBperm(halfBperm);

	localPointAperm.z = scalesAperm.z;
	localPointBperm.z = scalesBperm.z;

	scalesAperm.z = 0;
	scalesBperm.z = 0;

	int otherFaceDimA, otherFaceDimB;

	switch (axisType)
	{
	case A_AXIS:
		VertexBFaceATests(done, minDistSqr, localPointAperm, localPointBperm,
			hAperm, faceOffsetABperm, faceOffsetBAperm,
			matrixABperm, matrixBAperm, signsBperm, scalesBperm, true);

		if (done)
			break;

		VertexAFaceBTests(done, minDistSqr, localPointAperm, localPointBperm,
			hBperm, faceOffsetABperm, faceOffsetBAperm,
			matrixABperm, matrixBAperm, signsAperm, scalesAperm, false);

		if (done)
			break;

		EdgeTests(done, minDistSqr, localPointAperm, localPointBperm,
			otherFaceDimA, otherFaceDimB, hAperm, hBperm, faceOffsetABperm, faceOffsetBAperm,
			matrixABperm, matrixBAperm, signsAperm, signsBperm, scalesAperm, scalesBperm, false);

		break;
	case B_AXIS:
		VertexAFaceBTests(done, minDistSqr, localPointAperm, localPointBperm,
			hBperm, faceOffsetABperm, faceOffsetBAperm,
			matrixABperm, matrixBAperm, signsAperm, scalesAperm, true);

		if (done)
			break;

		VertexBFaceATests(done, minDistSqr, localPointAperm, localPointBperm,
			hAperm, faceOffsetABperm, faceOffsetBAperm,
			matrixABperm, matrixBAperm, signsBperm, scalesBperm, false);

		if (done)
			break;

		EdgeTests(done, minDistSqr, localPointAperm, localPointBperm,
			otherFaceDimA, otherFaceDimB, hAperm, hBperm, faceOffsetABperm, faceOffsetBAperm,
			matrixABperm, matrixBAperm, signsAperm, signsBperm, scalesAperm, scalesBperm, false);


		break;
	case CROSS_AXIS:
		EdgeTests(done, minDistSqr, localPointAperm, localPointBperm,
			otherFaceDimA, otherFaceDimB, hAperm, hBperm, faceOffsetABperm, faceOffsetBAperm,
			matrixABperm, matrixBAperm, signsAperm, signsBperm, scalesAperm, scalesBperm, true);

		if (done)
			break;

		VertexBFaceATests(done, minDistSqr, localPointAperm, localPointBperm,
			halfAperm, faceOffsetABperm, faceOffsetBAperm,
			matrixABperm, matrixBAperm, signsBperm, scalesBperm, false);

		if (done)
			break;

		VertexAFaceBTests(done, minDistSqr, localPointAperm, localPointBperm,
			hBperm, faceOffsetABperm, faceOffsetBAperm,
			matrixABperm, matrixBAperm, signsAperm, scalesAperm, false);

		break;

	}

	pointA = GameMath::Rotate(localPointAperm, apermCol);
	pointB = GameMath::Rotate(localPointBperm, bpermCol);

	normal = GameMath::Rotate(axisA, transformA);


	if (maxGap < 0.0f)
		return maxGap;

	return sqrtf(minDistSqr);

}