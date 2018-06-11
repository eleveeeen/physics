#ifndef _BroadPhase_h_
#define _BroadPhase_h_

#include"BroadPhasePair.h"
#include"BroadPhaseProxy.h"
#include"../RigidBody/RigidState.h"
#include"../Collision/Collidable.h"
#include"../Task/TaskManager.h"

#define OUT_OF_WORLD_BEHAVIOR_FIX_MOTION	0x01
#define OUT_OF_WORLD_BEHAVIOR_REMOVE_PROXY	0x02

struct UpdateBroadphaseProxiesParam
{
	void* workBuff;
	unsigned int workBytes;
	BroadphaseProxy* proxiesX;
	BroadphaseProxy* proxiesY;
	BroadphaseProxy* proxiesZ;
	BroadphaseProxy* proxiesXb;
	BroadphaseProxy* proxiesYb;
	BroadphaseProxy* proxiesZb;
	RigidState* offsetRigidStates;
	Collidable* offsetCollidables;
	unsigned int numRigidBodies;
	unsigned int outOfWorldBehavior;
	D3DXVECTOR3 worldCenter;
	D3DXVECTOR3 worldExtent;

	UpdateBroadphaseProxiesParam() :outOfWorldBehavior(0){};

};

struct UpdateBroadphaseProxiesResult
{
	int numOutOfWorldProxies;
};

unsigned int GetWorkBytesOfUpdateBroadphaseProxies(unsigned int numRigidBodies);

unsigned int GetWorkBytesOfUpdateBroadphaseProxies(unsigned int numRigidBodies, unsigned int maxTasks);


int UpdateBroadphaseProxies(UpdateBroadphaseProxiesParam& param, UpdateBroadphaseProxiesResult& result);
int UpdateBroadphaseProxies(UpdateBroadphaseProxiesParam& param, UpdateBroadphaseProxiesResult& result, TaskManager* taskManager);

struct FindPairsParam
{
	void* workBuff;
	unsigned int workBytes;
	void* pairBuff;
	unsigned int pairBytes;
	BroadphaseProxy* proxies;
	unsigned int numProxies;
	unsigned int maxPairs;
	unsigned int axis;
};



struct FindPairResult
{
	BroadphasePair* pairs;
	unsigned int numPairs;
};

unsigned int GetWorkBytesOfFindPairs(unsigned int maxPairs, unsigned int maxTasks = 1);
unsigned int GetPairBytesOfFindPairs(unsigned int maxPairs);

int FindPairs(FindPairsParam& param, FindPairResult& result);
int FindPairs(FindPairsParam& param, FindPairResult& result, TaskManager* taskManager);

struct DecomposePairParam
{
	void* workBuff;
	unsigned int workBytes;
	void* pairBuff;
	unsigned int pairBytes;
	BroadphasePair* previousPairs;
	unsigned int numPreviousPairs;
	BroadphasePair* currentPairs;
	unsigned int numCurrentPairs;
};

struct DecomposePairResult
{
	BroadphasePair* outNewPairs;
	unsigned int numOutNewPairs;
	BroadphasePair* outKeepPairs;
	unsigned int numOutKeepPairs;
	BroadphasePair* outRemovePairs;
	unsigned int numOutRemovePairs;
};


unsigned int GetWorkBytesOfDecomposePairs(unsigned int numPreviousPairs, unsigned int numCurrentPairs, int maxTasks = 1);
unsigned int GetPairBytesOfDecomposePairs(unsigned int numPreviousPairs, unsigned int numCurrentPairs);

int DecomposePairs(DecomposePairParam& param, DecomposePairResult& result);
int DecomposePairs(DecomposePairParam& param, DecomposePairResult& result, TaskManager* taskManager);


#endif