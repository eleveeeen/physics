#include"ContactCache.h"
#include"../../Utility/GameMath.h"
#define CONTACT_SAME_POINT 0.01f

int ContactCache::FindNearestContactPoint(const D3DXVECTOR3& newPointA, const D3DXVECTOR3& newPointB, const D3DXVECTOR3& newNormal)
{
	int nearestIdx = -1;

	float minDiff = CONTACT_SAME_POINT;
	for (UInt32 i = 0; i < numContacts; ++i)
	{
		float diffA = D3DXVec3LengthSq(&(cachedContactPoints[i].localPointA - newPointA));
		float diffB = D3DXVec3LengthSq(&(cachedContactPoints[i].localPointB - newPointB));

		if (diffA < minDiff && diffB<minDiff&& D3DXVec3Dot(&newNormal, &cachedContactPoints[i].normal)>0.99f)
		{
			minDiff = max(diffA, diffB);
			nearestIdx = i;
		}
	}


	return nearestIdx;

}

static inline float CalcArea4Points(const D3DXVECTOR3& p0, const D3DXVECTOR3& p1, const D3DXVECTOR3& p2, const D3DXVECTOR3& p3)
{
	D3DXVECTOR3 a[3], b[3];
	a[0] = p0 - p1;
	a[1] = p0 - p2;
	a[2] = p0 - p3;
	b[0] = p2 - p3;
	b[1] = p1 - p3;
	b[2] = p1 - p2;

	D3DXVECTOR3 Ax(a[0].x, a[1].x, a[2].x);
	D3DXVECTOR3 Ay(a[0].y, a[1].y, a[2].y);
	D3DXVECTOR3 Az(a[0].z, a[1].z, a[2].z);

	D3DXVECTOR3 Bx(b[0].x, b[1].x, b[2].x);
	D3DXVECTOR3 By(b[0].y, b[1].y, b[2].y);
	D3DXVECTOR3 Bz(b[0].z, b[1].z, b[2].z);

	D3DXVECTOR3 tmpX = GameMath::MulPerElem(Ay, Bz) - GameMath::MulPerElem(Az, By);
	D3DXVECTOR3 tmpY = GameMath::MulPerElem(Az, Bx) - GameMath::MulPerElem(Ax, Bz);
	D3DXVECTOR3 tmpZ = GameMath::MulPerElem(Ax, By) - GameMath::MulPerElem(Ay, Bx);

	D3DXVECTOR3 area = GameMath::MulPerElem(tmpX, tmpX) + GameMath::MulPerElem(tmpY, tmpY) + GameMath::MulPerElem(tmpZ, tmpZ);

	return GameMath::MaxElem(area);

}

#define CalcArea4Points0 CalcArea4Points(newP, p[1], p[2], p[3])
#define CalcArea4Points1 CalcArea4Points(newP, p[0], p[2], p[3])
#define CalcArea4Points2 CalcArea4Points(newP, p[0], p[1], p[3])
#define CalcArea4Points3 CalcArea4Points(newP, p[0], p[1], p[2])
					  


int ContactCache::Sort4ContactPoints(const D3DXVECTOR3& newCP, float newDistance)
{
	int maxPenetrationIndex = -1;

	float maxPenetration = newDistance;

	for (int i = 0; i < MAX_CACHED_CONTACT_POINTS; i++)
	{
		if (cachedContactPoints[i].distance < maxPenetration)
		{
			maxPenetrationIndex = i;
			maxPenetration = cachedContactPoints[i].distance;
		}
	}

	float res[4] = { 0.0f };

	D3DXVECTOR3 newP(newCP);
	D3DXVECTOR3 p[4];

	p[0] = (D3DXVECTOR3)cachedContactPoints[0].localPointA;
	p[1] = (D3DXVECTOR3)cachedContactPoints[1].localPointA;
	p[2] = (D3DXVECTOR3)cachedContactPoints[2].localPointA;
	p[3] = (D3DXVECTOR3)cachedContactPoints[3].localPointA;

	switch (maxPenetrationIndex)
	{
	case 0:
		res[1] = CalcArea4Points1;
		res[2] = CalcArea4Points2;
		res[3] = CalcArea4Points3;
		break;
	case 1:
		res[0] = CalcArea4Points0;
		res[2] = CalcArea4Points2;
		res[3] = CalcArea4Points3;
		break;

	case 2:
		res[0] = CalcArea4Points0;
		res[1] = CalcArea4Points1;
		res[3] = CalcArea4Points3;
		break;

	case 3:
		res[0] = CalcArea4Points0;
		res[1] = CalcArea4Points1;
		res[2] = CalcArea4Points2;
		break;

	}

	int maxIndex = 0;
	float maxVal = res[0];

	if (res[1] > maxVal)
	{
		maxIndex = 1;
		maxVal = res[1];
	}

	if (res[2] > maxVal)
	{
		maxIndex = 2;
		maxVal = res[2];
	}

	if (res[3] > maxVal)
	{
		maxIndex = 3;
		maxVal = res[3];
	}

	return maxIndex;


}

void ContactCache::AddContactPoint(
	float newDistance,
	const D3DXVECTOR3& newNormal,
	const D3DXVECTOR3& newPointA,
	const D3DXVECTOR3& newPointB,
	SubData subData)
{

	int id = FindNearestContactPoint(newPointA, newPointB, newNormal);

	
	if (id < 0 && numContacts < MAX_CACHED_CONTACT_POINTS)
	{
		//衝突点を新規追加
		id = numContacts++;
		cachedContactPoints[id].Reset();

	}
	else if (id < 0)
	{
		//ソート
		id = Sort4ContactPoints(newPointA, newDistance);
		cachedContactPoints[id].Reset();
	}

	cachedContactPoints[id].distance = newDistance;
	cachedContactPoints[id].subData = subData;
	cachedContactPoints[id].normal = newNormal;
	cachedContactPoints[id].localPointA = newPointA;
	cachedContactPoints[id].localPointB = newPointB;


}

void ContactCache::AddContactPoint(const CachedContactPoint& cp)
{
	D3DXVECTOR3 pA = cp.localPointA;
	D3DXVECTOR3 pB = cp.localPointB;

	int id = FindNearestContactPoint(pA, pB, cp.normal);

	if (id >= 0)
	{
		if (cachedContactPoints[id].distance > cp.distance)
		{
			cachedContactPoints[id].distance = cp.distance;
			cachedContactPoints[id].normal = cp.normal;
			cachedContactPoints[id].localPointA = cp.localPointA;
			cachedContactPoints[id].localPointB = cp.localPointB;

		}
	}
	else if (numContacts < MAX_CACHED_CONTACT_POINTS)
	{
		cachedContactPoints[numContacts] = cp;
		numContacts++;
	}
	else
	{
		id = Sort4ContactPoints(pA, cp.distance);

		cachedContactPoints[id] = cp;
	}

	

}