#ifndef _RayCast_h_
#define _RayCast_h_

#include"../RigidBody/RigidState.h"
#include"Collidable.h"
#include"Ray.h"
#include"../BroadPhase/BroadPhaseProxy.h"

//レイキャストに関連するヘッダー


//レイキャストのパラメータ
struct RayCastParam
{
	RigidState* offsetRigidStates;
	Collidable* offsetCollidables;
	BroadphaseProxy* proxiesX;
	BroadphaseProxy* proxiesY;
	BroadphaseProxy* proxiesZ;
	BroadphaseProxy* proxiesXb;
	BroadphaseProxy* proxiesYb;
	BroadphaseProxy* proxiesZb;
	UInt32 numProxies;

	D3DXVECTOR3 rangeCenter;
	D3DXVECTOR3 rangeExtent;
};



//レイキャストの処理
//input情報とパラメータを元にoutput情報を取得
void CastRay(const RayInput& rayInput, RayOutput& rayOutput, const RayCastParam& param);

#endif