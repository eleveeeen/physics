#ifndef _ContactManifold_h_
#define _ContactManifold_h_

#include"../Base/Common.h"
#include"SubData.h"
#include"../Solver/constraintRow.h"
#include<d3dx9.h>
#define NUMCONTACTS_PER_BODIES 4

//contactPoint

struct ContactPoint
{
	UInt8 duration;
	UInt8 shapeIDA;
	UInt8 shapeIDB;

	SubData subData;
	float distance;
	float localPointA[3];
	float localPointB[3];
	ConstraintRow constraintRow[3];

	void Reset()
	{
		duration = 0;
		shapeIDA = 0;
		shapeIDB = 0;
		subData = SubData();
		distance = FLT_MAX;
		constraintRow[0].accumImpulse = 0.0f;
		constraintRow[1].accumImpulse = 0.0f;
		constraintRow[2].accumImpulse = 0.0f;
	}

};

//contact manifold

//同一ペアの衝突が続く限り保持されるコンタクト情報

class  __declspec(align(128)) ContactManifold
{
private://メンバ変数

	UInt16 rigidBodyIDA;
	UInt16 rigidBodyIDB;
	UInt16 duration;
	UInt16 numContacts;
	float compositeFriction;
	UInt32 internalFlag;
	ContactPoint contactPoints[NUMCONTACTS_PER_BODIES];
	void* userData;
	UInt32 userParam[4];




public://メンバ変数

	float GetCompositeFriction()const
	{
		return compositeFriction;
	}

	void SetCompositeFriction(float f)
	{
		compositeFriction = f;
	}

	UInt32 GetInternalFlag()const
	{
		return internalFlag;
	}

	void SetInternalFlag(UInt32 f)
	{
		internalFlag = f;
	}

	void Reset(UInt16 _rigidBodyIDA, UInt16 _rigidBodyIDB)
	{
		userData = 0;
		userParam[0] = userParam[1] = userParam[2] = userParam[3] = 0;
		numContacts = 0;
		duration = 0;
		rigidBodyIDA = _rigidBodyIDA;
		rigidBodyIDB = _rigidBodyIDB;
	}

	void AddContactPoint(
		float newDistance,
		const D3DXVECTOR3& newNormal,
		const D3DXVECTOR3& newPointA,
		const D3DXVECTOR3& newPointB,
		SubData subData);

	void AddContactPoint(const ContactPoint& cp);

	void RemoveContactPoint(const int i)
	{
		contactPoints[i] = contactPoints[numContacts - 1];
		numContacts--;
	}

	int GetNumContacts()const
	{
		return (int)numContacts;
	}

	ContactPoint& GetContactPoint(int i)
	{
		return contactPoints[i];
	}

	const ContactPoint& GetContactPoint(int i)const
	{
		return contactPoints[i];
	}

	void Refresh(const D3DXVECTOR3& posA, const D3DXQUATERNION& rotA, const D3DXVECTOR3& posB, const D3DXQUATERNION& rotB);

	void Merge(const ContactManifold& contact);

	UInt16 GetDuration()const
	{
		return duration;
	}

	UInt16 GetRigidBodyIDA()const
	{
		return rigidBodyIDA;
	}

	UInt16 GetRigidBodyIDB()const
	{
		return rigidBodyIDB;
	}


private://メンバ変数
	int Sort4ContactPoints(const D3DXVECTOR3& newPoints, float newDistance);
	int FindNearestContactPoint(const D3DXVECTOR3& newPointA, const D3DXVECTOR3& newPointB, const D3DXVECTOR3& newNormal, SubData subData);
};

#endif