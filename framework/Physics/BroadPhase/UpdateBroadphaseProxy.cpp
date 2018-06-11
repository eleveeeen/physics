#include"../Collision/aabb.h"
#include"UpdateBroadphaseProxy.h"
#include"../../Utility/GameMath.h"

// < 
static inline bool LessThan(const D3DXVECTOR3& v1, const D3DXVECTOR3& v2)
{
	return GameMath::MaxElem(v2 - v1) > 0.0f;
}

int UpdateBroadphaseProxy(
	BroadphaseProxy& proxy,
	const RigidState& state,
	const Collidable& coll,
	const D3DXVECTOR3& worldCenter,
	const D3DXVECTOR3& worldExtent,
	UInt32 axis)
{
	int ret = 0;

	D3DXMATRIX rot = GameMath::QuaternionToMatrix(state.GetOrientation());
	D3DXVECTOR3 center = state.GetPosition() + GameMath::Rotate(coll.GetCenter(), rot);
	D3DXVECTOR3 half = GameMath::Rotate(coll.GetHalf(), GameMath::AbsPerElem(rot));

	D3DXVECTOR3 minRig = center - half;
	D3DXVECTOR3 maxRig = center + half;

	D3DXVECTOR3 minWld = worldCenter - half;
	D3DXVECTOR3 maxWld = worldCenter + half;

	if (maxWld < minRig || maxRig < minWld)
	{
		ret = -1;
	}

	int aabbMin[3], aabbMax[3];
	GameMath::ConvertCoordWorldToLocal(worldCenter, worldExtent, minRig, maxRig, aabbMin, aabbMax);

	SetXMin(proxy, aabbMin[0]);
	SetXMax(proxy, aabbMax[0]);
	SetYMin(proxy, aabbMin[1]);
	SetYMax(proxy, aabbMax[1]);
	SetZMin(proxy, aabbMin[2]);
	SetZMax(proxy, aabbMax[2]);

	SetKey(proxy, aabbMin[axis]);
	SetObjectID(proxy, state.GetRigidBodyID());
	SetMotionMask(proxy, state.GetContactFilterSelf());
	SetTarget(proxy, state.GetContactFilterTarget());

	return ret;

}

int UpdateBroadphaseProxy(
	BroadphaseProxy& proxy,
	const RigidState& state,
	const D3DXVECTOR3& objectCenter,
	const D3DXVECTOR3& objectHalf,
	const D3DXVECTOR3& worldCenter,
	const D3DXVECTOR3& worldExtent,
	unsigned int axis)
{
	int ret = 0;

	D3DXVECTOR3 minRig = objectCenter - objectHalf;
	D3DXVECTOR3 maxRig = objectCenter + objectHalf;

	D3DXVECTOR3 minWld = worldCenter - worldExtent;
	D3DXVECTOR3 maxWld = worldCenter + worldExtent;

	if (maxWld < minRig || maxRig < minWld)
	{
		ret = -1;
	}

	D3DXVECTOR3 aabbMin, aabbMax;

	GameMath::ConvertCoordWorldToLocal(worldCenter, worldExtent, minRig, maxRig, aabbMin, aabbMax);

	SetXMin(proxy, aabbMin.x);
	SetXMax(proxy, aabbMax.x);
	SetYMin(proxy, aabbMin.y);
	SetYMax(proxy, aabbMax.y);
	SetZMin(proxy, aabbMin.z);
	SetZMax(proxy, aabbMax.z);
	
	SetKey(proxy, GameMath::GetElem(aabbMin, axis));
	SetObjectID(proxy, state.GetRigidBodyID());
	SetMotionMask(proxy, state.GetMotionMask());
	SetSelf(proxy, state.GetContactFilterSelf());
	SetTarget(proxy, state.GetContactFilterTarget());
	
	return ret;

}



int UpdateBroadphaseProxy(
	BroadphaseProxy& proxyX,
	BroadphaseProxy& proxyY,
	BroadphaseProxy& proxyZ,
	BroadphaseProxy& proxyXb,
	BroadphaseProxy& proxyYb,
	BroadphaseProxy& proxyZb,
	const RigidState& state,
	const Collidable& coll,
	const D3DXVECTOR3& worldCenter,
	const D3DXVECTOR3& worldExtent)
{
	int ret = 0;

	D3DXMATRIX rot = GameMath::QuaternionToMatrix(state.GetOrientation());
	D3DXVECTOR3 center = state.GetPosition() + GameMath::Rotate(coll.GetCenter(), rot);

	D3DXVECTOR3 half = GameMath::Rotate(coll.GetHalf(), GameMath::AbsPerElem(rot));

	D3DXVECTOR3 minRig = center - half;
	D3DXVECTOR3 maxRig = center + half;

	D3DXVECTOR3 minWld = worldCenter - worldExtent;
	D3DXVECTOR3 maxWld = worldCenter + worldExtent;

	if (GameMath::MaxElem(minRig - maxWld) > 0.0f || GameMath::MaxElem(minWld - maxRig) > 0.0f)
	{
		ret = -1;
	}

	int aabbMin[3], aabbMax[3];

	GameMath::ConvertCoordWorldToLocal(worldCenter, worldExtent, minRig, maxRig, aabbMin, aabbMax);


	//AABB‚Ìî•ñ‚ðƒZƒbƒg
	BroadphaseProxy proxy;
	SetXMin(proxy, aabbMin[0]);
	SetXMax(proxy, aabbMax[0]);
	SetYMin(proxy, aabbMin[1]);
	SetYMax(proxy, aabbMax[1]);
	SetZMin(proxy, aabbMin[2]);
	SetZMax(proxy, aabbMax[2]);

	SetObjectID(proxy, state.GetRigidBodyID());
	SetMotionMask(proxy, state.GetMotionMask());
	SetSelf(proxy, state.GetContactFilterSelf());
	SetTarget(proxy, state.GetContactFilterTarget());


	proxyX = proxy;
	proxyY = proxy;
	proxyZ = proxy;
	proxyXb = proxy;
	proxyYb = proxy;
	proxyZb = proxy;

	SetKey(proxyX,  aabbMin[0]);
	SetKey(proxyXb, aabbMax[0]);
	SetKey(proxyY,  aabbMin[1]);
	SetKey(proxyYb, aabbMax[1]);
	SetKey(proxyZ,  aabbMin[2]);
	SetKey(proxyZb, aabbMax[2]);

	return ret;
}


int UpdateBroadphaseProxy(
	BroadphaseProxy& proxyX,
	BroadphaseProxy& proxyY,
	BroadphaseProxy& proxyZ,
	BroadphaseProxy& proxyXb,
	BroadphaseProxy& proxyYb,
	BroadphaseProxy& proxyZb,
	const RigidState& state,
	const D3DXVECTOR3& objectCenter,
	const D3DXVECTOR3& objectHalf,
	const D3DXVECTOR3& worldCenter,
	const D3DXVECTOR3& worldExtent)
{
	int ret = 0;


	D3DXVECTOR3 minRig = objectCenter - objectHalf;
	D3DXVECTOR3 maxRig = objectCenter + objectHalf;

	D3DXVECTOR3 minWld = worldCenter - worldExtent;
	D3DXVECTOR3 maxWld = worldCenter + worldExtent;

	if (GameMath::MaxElem(minRig - maxWld) > 0.0f || GameMath::MaxElem(minWld - maxRig) > 0.0f)
	{
		ret = -1;
	}

	int aabbMin[3], aabbMax[3];

	GameMath::ConvertCoordWorldToLocal(worldCenter, worldExtent, minRig, maxRig, aabbMin, aabbMax);

	BroadphaseProxy proxy;
	SetXMin(proxy, aabbMin[0]);
	SetXMax(proxy, aabbMax[0]);
	SetYMin(proxy, aabbMin[1]);
	SetYMax(proxy, aabbMax[1]);
	SetZMin(proxy, aabbMin[2]);
	SetZMax(proxy, aabbMax[2]);

	SetObjectID(proxy, state.GetRigidBodyID());
	SetMotionMask(proxy, state.GetMotionMask());
	SetSelf(proxy, state.GetContactFilterSelf());
	SetTarget(proxy, state.GetContactFilterTarget());


	proxyX = proxy;
	proxyY = proxy;
	proxyZ = proxy;
	proxyXb = proxy;
	proxyYb = proxy;
	proxyZb = proxy;

	SetKey(proxyX, aabbMin[0]);
	SetKey(proxyXb, aabbMax[0]);
	SetKey(proxyY, aabbMin[1]);
	SetKey(proxyYb, aabbMax[1]);
	SetKey(proxyZ, aabbMin[2]);
	SetKey(proxyZb, aabbMax[2]);

	return ret;
}