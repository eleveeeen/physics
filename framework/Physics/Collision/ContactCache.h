#ifndef _ContactCache_h_
#define _ContactCache_h_

#include"../Base/Common.h"
#include"SubData.h"

#define MAX_CACHED_CONTACT_POINTS 4

struct CachedContactPoint
{
	SubData subData;
	float distance;
	D3DXVECTOR3 normal;
	D3DXVECTOR3 localPointA;
	D3DXVECTOR3 localPointB;

	UInt8 shapeIDA;
	UInt8 shapeIDB;

	void Reset()
	{
		shapeIDA = shapeIDB = 0;
		subData = SubData();
		distance = FLT_MAX;
	}

};

class ContactCache
{
private:
	UInt32 numContacts;
	CachedContactPoint cachedContactPoints[MAX_CACHED_CONTACT_POINTS];
public:
	ContactCache() :numContacts(0){};

	void AddContactPoint(
		float newDistance,
		const D3DXVECTOR3& newNormal,
		const D3DXVECTOR3& newPointA,
		const D3DXVECTOR3& newPointB,
		SubData subData);

	void AddContactPoint(const CachedContactPoint& cp);

	int GetNumContacts()const
	{
		return (int)numContacts;
	}

	CachedContactPoint& GetContactPoint(int i)
	{
		return cachedContactPoints[i];
	}

	const CachedContactPoint& GetContactPoint(int i)const
	{
		return cachedContactPoints[i];
	}

	float GetDistance(int i)
	{
		return cachedContactPoints[i].distance;
	}

	const D3DXVECTOR3& GetNormal(int i)const
	{
		return cachedContactPoints[i].normal;
	}

	const D3DXVECTOR3& GetLocalPointA(int i)const
	{
		return cachedContactPoints[i].localPointA;
	}

	const D3DXVECTOR3& GetLocalPointB(int i)const
	{
		return cachedContactPoints[i].localPointB;
	}

	const SubData& GetSubData(int i)const
	{
		return cachedContactPoints[i].subData;
	}



private:
	int FindNearestContactPoint(const D3DXVECTOR3& newPointA, const D3DXVECTOR3& newPointB, const D3DXVECTOR3& newNormal);
	int Sort4ContactPoints(const D3DXVECTOR3& newPoint, float newDistance);

};



#endif