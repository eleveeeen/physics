#ifndef _UpdateBroadphaseProxy_h_
#define _UpdateBroadphaseProxy_h_

#include"BroadPhasePair.h"
#include"BroadPhaseProxy.h"

#include"../RigidBody/RigidState.h"
#include"../Collision/Collidable.h"

//For single axis

int UpdateBroadphaseProxy(
	BroadphaseProxy& proxy,
	const RigidState& state,
	const Collidable& coll,
	const D3DXVECTOR3& worldCenter,
	const D3DXVECTOR3& worldExtent,
	unsigned int axis);

int UpdateBroadphaseProxy(
	BroadphaseProxy& proxy,
	const RigidState& state,
	const D3DXVECTOR3& objectCenter,
	const D3DXVECTOR3& objectHalf,
	const D3DXVECTOR3& worldCenter,
	const D3DXVECTOR3& worldExtent,
	unsigned int axis);

//For all axis

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
	const D3DXVECTOR3& worldExtent);

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
	const D3DXVECTOR3& worldExtent);

#endif