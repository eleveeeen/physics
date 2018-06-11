#ifndef _RayCast_h_
#define _RayCast_h_

#include"../RigidBody/RigidState.h"
#include"Collidable.h"
#include"Ray.h"
#include"../BroadPhase/BroadPhaseProxy.h"

//���C�L���X�g�Ɋ֘A����w�b�_�[


//���C�L���X�g�̃p�����[�^
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



//���C�L���X�g�̏���
//input���ƃp�����[�^������output�����擾
void CastRay(const RayInput& rayInput, RayOutput& rayOutput, const RayCastParam& param);

#endif