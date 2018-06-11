#include"ContactManifold.h"
#include"../../Utility/GameMath.h"
#define CONTACT_SAME_POINT			0.01f
#define CONTACT_THRESHOLD_NORMAL	0.01f	//衝突点の法線方向での閾値
#define CONTACT_THRESHOLD_TANGENT	0.002f	//衝突点の平面上での閾値

int ContactManifold::FindNearestContactPoint(const D3DXVECTOR3& newPointA, const D3DXVECTOR3& newPointB, const D3DXVECTOR3& newNormal, SubData subData)
{
	int nearestIdx = -1;

	float minDiff = CONTACT_SAME_POINT;
	for (int i = 0; i < numContacts; i++)
	{

		float diffA = D3DXVec3LengthSq(&(GameMath::GetVec3(contactPoints[i].localPointA) - newPointA));
		float diffB = D3DXVec3LengthSq(&(GameMath::GetVec3(contactPoints[i].localPointB) - newPointB));

		if (contactPoints[i].subData.type == SubData::MESH_INFO&&
			contactPoints[i].subData.facetLocal.islandID == subData.facetLocal.islandID&&
			contactPoints[i].subData.facetLocal.facetID == subData.facetLocal.facetID&&
			diffA < minDiff&&diffB < minDiff)
			return i;

		if (diffA < minDiff && diffB < minDiff &&
			D3DXVec3Dot(&newNormal, &GameMath::GetVec3(contactPoints[i].constraintRow[0].normal)) > 0.99f)
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

int ContactManifold::Sort4ContactPoints(const D3DXVECTOR3& newCP, float newDistance)
{
	int maxPenetrationIndex = -1;
	float maxPenetration = newDistance;

	//最も深い衝突点は排除対象から外す
	for (int i = 0; i < numContacts; i++)
	{
		if (contactPoints[i].distance < maxPenetration)
		{
			maxPenetrationIndex = i;
			maxPenetration = contactPoints[i].distance;
		}
	}

	float res[4] = { 0.0f };

	D3DXVECTOR3 newP(newCP);
	D3DXVECTOR3 p[4];
	p[0] = GameMath::GetVec3(contactPoints[0].localPointA);
	p[1] = GameMath::GetVec3(contactPoints[1].localPointA);
	p[2] = GameMath::GetVec3(contactPoints[2].localPointA);
	p[3] = GameMath::GetVec3(contactPoints[3].localPointA);

	if (maxPenetrationIndex != 0)
	{
		res[0] = CalcArea4Points(newP, p[1], p[2], p[3]);
	}

	if (maxPenetrationIndex != 1)
	{
		res[1] = CalcArea4Points(newP, p[0], p[2], p[3]);
	}

	if (maxPenetrationIndex != 2)
	{
		res[2] = CalcArea4Points(newP, p[0], p[1], p[3]);
	}

	if (maxPenetrationIndex != 3)
	{
		res[3] = CalcArea4Points(newP, p[0], p[1], p[2]);
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

void ContactManifold::AddContactPoint(
	float newDistance,
	const D3DXVECTOR3& newNormal,
	const D3DXVECTOR3& newPointA,
	const D3DXVECTOR3& newPointB,
	SubData subData)
{
	int id = FindNearestContactPoint(newPointA, newPointB, newNormal, subData);

	//衝突点新規追加
	if (id < 0 && numContacts < NUMCONTACTS_PER_BODIES)
	{
		id = numContacts;
		numContacts++;
		contactPoints[id].Reset();
	}
	//ソート
	else if (id < 0)
	{
		id = Sort4ContactPoints(newPointA, newDistance);
		contactPoints[id].Reset();
	}

	contactPoints[id].distance = newDistance;
	contactPoints[id].subData = subData;
	GameMath::StoreVec3(newPointA, contactPoints[id].localPointA);
	GameMath::StoreVec3(newPointB, contactPoints[id].localPointB);
	GameMath::StoreVec3(newNormal, contactPoints[id].constraintRow[0].normal);

}


void ContactManifold::AddContactPoint(const ContactPoint& cp)
{
	D3DXVECTOR3 pA = GameMath::GetVec3(cp.localPointA);
	D3DXVECTOR3 pB = GameMath::GetVec3(cp.localPointB);

	int id = FindNearestContactPoint(pA, pB, GameMath::GetVec3(cp.constraintRow[0].normal), cp.subData);

	if (id >= 0)
	{
		D3DXVECTOR3 n1 = GameMath::GetVec3(contactPoints[id].constraintRow[0].normal);
		D3DXVECTOR3 n2 = GameMath::GetVec3(cp.constraintRow[0].normal);

		//継続
		if (fabsf(D3DXVec3Dot(&n1, &n2)) > 0.99f)
		{
			contactPoints[id].distance = cp.distance;
			contactPoints[id].localPointA[0] = cp.localPointA[0];
			contactPoints[id].localPointA[1] = cp.localPointA[1];
			contactPoints[id].localPointA[2] = cp.localPointA[2];
			contactPoints[id].localPointB[0] = cp.localPointB[0];
			contactPoints[id].localPointB[1] = cp.localPointB[1];
			contactPoints[id].localPointB[2] = cp.localPointB[2];
			contactPoints[id].constraintRow[0].normal[0] = cp.constraintRow[0].normal[0];
			contactPoints[id].constraintRow[0].normal[1] = cp.constraintRow[0].normal[1];
			contactPoints[id].constraintRow[0].normal[2] = cp.constraintRow[0].normal[2];

		}
		//更新
		else
		{
			contactPoints[id] = cp;
		}
	}
	//衝突点新規追加
	else if (numContacts < NUMCONTACTS_PER_BODIES)
	{
		contactPoints[numContacts] = cp;
		numContacts++;
	}
	else
	{
		id = Sort4ContactPoints(pA, cp.distance);

		contactPoints[id] = cp;
	}
}

void ContactManifold::Merge(const ContactManifold& contact)
{
	for (int i = 0; i < contact.GetNumContacts(); i++)
	{
		AddContactPoint(contact.GetContactPoint(i));
	}
}

void ContactManifold::Refresh(const D3DXVECTOR3& posA, const D3DXQUATERNION& rotA, const D3DXVECTOR3& posB, const D3DXQUATERNION& rotB)
{

	//衝突点更新
	//衝突点間の距離が閾値を超えたら消去
	for (int i = 0; i < (int)numContacts; i++)
	{
		D3DXVECTOR3 normal = GameMath::GetVec3(contactPoints[i].constraintRow[0].normal);
		D3DXVECTOR3 cpA = posA + GameMath::Rotate(GameMath::GetVec3(contactPoints[i].localPointA), rotA);
		D3DXVECTOR3 cpB = posB + GameMath::Rotate(GameMath::GetVec3(contactPoints[i].localPointB), rotB);
	
		float distance = D3DXVec3Dot(&normal, &(cpA - cpB));

		//貫通深度がプラスになったかチェック　（離れたか）
		if (distance > CONTACT_THRESHOLD_NORMAL)
		{
			RemoveContactPoint(i);
			i--;
			continue;
		}

		contactPoints[i].distance = distance;

		//深度方向を除去して点の距離をチェック
		cpA = cpA - contactPoints[i].distance*normal;
		float distanceAB = D3DXVec3LengthSq(&(cpA - cpB));
		if (distanceAB > CONTACT_THRESHOLD_TANGENT)
		{
			RemoveContactPoint(i);
			i--;
			continue;
		}


		if (contactPoints[i].duration < 255)
		{
			contactPoints[i].duration++;
		}

	}
	if (numContacts>0 && duration < 65535)
	{
		duration++;
	}

}