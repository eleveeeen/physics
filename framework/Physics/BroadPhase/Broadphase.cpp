#include"../Sort/Sort.h"
#include"../BroadPhase/BroadPhaseProxy.h"
#include"../BroadPhase/UpdateBroadphaseProxy.h"
#include"../BroadPhase/BroadPhase.h"
#include"CheckCollidable.h"
#include"../Base/Common.h"

UInt32 GetWorkBytesOfUpdateBroadphaseProxies(UInt32 numRigidBodies)
{
	return 16 + ALLOC_BYTES_ALIGN16(sizeof(BroadphaseProxy)*numRigidBodies);
}


UInt32 GetWorkBytesOfUpdateBroadphaseProxies(UInt32 numRigidBodies, UInt32 maxTasks)
{

	return 16 + ALLOC_BYTES_ALIGN16(sizeof(BroadphaseProxy)*numRigidBodies) * 6;
}

UInt32 GetWorkBytesOfFindPairs(UInt32 maxPairs, UInt32 maxTasks)
{
	return 16 + ALLOC_BYTES_ALIGN16(sizeof(BroadphasePair)*maxPairs)*maxTasks;
}

UInt32 GetPairBytesOfFindPairs(UInt32 maxPairs)
{
	return ALLOC_BYTES_ALIGN16(sizeof(BroadphasePair)*maxPairs);
}

UInt32 GetWorkBytesOfDecomposePairs(UInt32 numPreviousPairs, UInt32 numCurrentPairs, int maxTasks)
{
	return 16 +
		ALLOC_BYTES_ALIGN16(sizeof(BroadphasePair)*numPreviousPairs) +
		ALLOC_BYTES_ALIGN16(sizeof(BroadphasePair)*numPreviousPairs)*maxTasks +
		ALLOC_BYTES_ALIGN16(sizeof(BroadphasePair)*numCurrentPairs)*maxTasks;

}

int CheckParamOfUpdateBroadphaseProxies(const UpdateBroadphaseProxiesParam& param)
{

	if (!param.workBuff ||
		!param.proxiesX || !param.proxiesY || !param.proxiesZ ||
		!param.proxiesXb || !param.proxiesYb || !param.proxiesZb ||
		!param.offsetRigidStates || !param.offsetCollidables)
		return -1;
	
	if (param.outOfWorldBehavior&(UInt32)0xfffffffc)
		return -1;

	if (!PTR_IS_ALIGNED16(param.proxiesX) || !PTR_IS_ALIGNED16(param.proxiesY) || !PTR_IS_ALIGNED16(param.proxiesZ) ||
		!PTR_IS_ALIGNED16(param.proxiesXb) || !PTR_IS_ALIGNED16(param.proxiesYb) || !PTR_IS_ALIGNED16(param.proxiesZb))
		return -2;
	
	if (AVAILABLE_BYTES_ALIGN16(param.workBuff, param.workBytes) < GetWorkBytesOfUpdateBroadphaseProxies(param.numRigidBodies))
		return -3;

	return 0;

}

int CheckParamOfFindPairs(const FindPairsParam& param, int maxTasks)
{
	if (!param.workBuff || !param.pairBuff || !param.proxies || param.axis > 2 )
		return -1;

	if (!PTR_IS_ALIGNED16(param.proxies) || !PTR_IS_ALIGNED16(param.pairBuff))
		return -2;

	if (AVAILABLE_BYTES_ALIGN16(param.workBuff, param.workBytes) < GetWorkBytesOfFindPairs(param.maxPairs, maxTasks))
		return -3;
	
	if (AVAILABLE_BYTES_ALIGN16(param.pairBuff, param.pairBytes) < GetPairBytesOfFindPairs(param.maxPairs))
		return -3;

	return 0;

}

UInt32 GetPairBytesOfDecomposePairs(UInt32 numPreviousPairs, UInt32 numCurrentPairs)
{
	return sizeof(BroadphasePair)*((numPreviousPairs << 1) + numCurrentPairs);
}


int CheckParamOfDecomposePairs(const DecomposePairParam& param, int maxTasks)
{
	if (!param.workBuff || !param.pairBuff || !param.previousPairs || !param.currentPairs)
		return -1;

	if (!PTR_IS_ALIGNED16(param.previousPairs) || !PTR_IS_ALIGNED16(param.currentPairs) || !PTR_IS_ALIGNED16(param.pairBuff))
		return -2;

	if (AVAILABLE_BYTES_ALIGN16(param.workBuff, param.workBytes) < GetWorkBytesOfDecomposePairs(param.numPreviousPairs, param.numCurrentPairs, maxTasks))
		return -3;

	if (AVAILABLE_BYTES_ALIGN16(param.pairBuff, param.pairBytes) < GetPairBytesOfDecomposePairs(param.numPreviousPairs, param.numCurrentPairs))
		return -3;

	return 0;
}


int UpdateBroadphaseProxies(UpdateBroadphaseProxiesParam& param, UpdateBroadphaseProxiesResult& result)
{
	int ret = CheckParamOfUpdateBroadphaseProxies(param);
	if (ret)
		return ret;

	result.numOutOfWorldProxies = 0;

	for (int i = 0; i < (int)param.numRigidBodies; i++)
	{
		RigidState& state = param.offsetRigidStates[i];

		int chk = UpdateBroadphaseProxy(
			param.proxiesX[i],
			param.proxiesY[i],
			param.proxiesZ[i],
			param.proxiesXb[i],
			param.proxiesYb[i],
			param.proxiesZb[i],
			param.offsetRigidStates[i],
			param.offsetCollidables[i],
			param.worldCenter,
			param.worldExtent);

		if (state.GetContactFilterSelf() == 0 && state.GetContactFilterTarget() == 0)
		{

			SetKey(param.proxiesX[i], 0xffffffff);
			SetKey(param.proxiesY[i], 0xffffffff);
			SetKey(param.proxiesZ[i], 0xffffffff);
			SetKey(param.proxiesXb[i], 0xffffffff);
			SetKey(param.proxiesYb[i], 0xffffffff);
			SetKey(param.proxiesZb[i], 0xffffffff);

			result.numOutOfWorldProxies++;

		}
		else if (chk == -1)
		{
			if (param.outOfWorldBehavior & OUT_OF_WORLD_BEHAVIOR_FIX_MOTION)
			{
				state.SetMotionType(eMotionType::eFixed);
				SetMotionMask(param.proxiesX[i], state.GetMotionMask());
				SetMotionMask(param.proxiesY[i], state.GetMotionMask());
				SetMotionMask(param.proxiesZ[i], state.GetMotionMask());
				SetMotionMask(param.proxiesXb[i], state.GetMotionMask());
				SetMotionMask(param.proxiesYb[i], state.GetMotionMask());
				SetMotionMask(param.proxiesZb[i], state.GetMotionMask());
			}

			if (param.outOfWorldBehavior & OUT_OF_WORLD_BEHAVIOR_REMOVE_PROXY)
			{
				SetKey(param.proxiesX[i], 0xffffffff);
				SetKey(param.proxiesY[i], 0xffffffff);
				SetKey(param.proxiesZ[i], 0xffffffff);
				SetKey(param.proxiesXb[i], 0xffffffff);
				SetKey(param.proxiesYb[i], 0xffffffff);
				SetKey(param.proxiesZb[i], 0xffffffff);
				result.numOutOfWorldProxies++;
			}
		}
	}

	HeapManager pool((UInt8*)param.workBuff, param.workBytes);

	BroadphaseProxy* workProxies = (BroadphaseProxy*)pool.Allocate(sizeof(BroadphaseProxy)*param.numRigidBodies);

	Sort(param.proxiesX, workProxies, param.numRigidBodies);
	Sort(param.proxiesY, workProxies, param.numRigidBodies);
	Sort(param.proxiesZ, workProxies, param.numRigidBodies);
	Sort(param.proxiesXb, workProxies, param.numRigidBodies);
	Sort(param.proxiesYb, workProxies, param.numRigidBodies);
	Sort(param.proxiesZb, workProxies, param.numRigidBodies);

	pool.Deallocate(workProxies);

	return 0;
	
}

int FindPairs(FindPairsParam& param, FindPairResult& result)
{
	int ret = CheckParamOfFindPairs(param, 0);

	if (ret)
		return ret;

	void* workBuff = param.workBuff;
	UInt32 workBytes = param.workBytes;
	BroadphaseProxy* proxies = param.proxies;
	UInt32 numProxies = param.numProxies;
	UInt32 maxPairs = param.maxPairs;
	int axis = param.axis;

	BroadphasePair* pairs = (BroadphasePair*)PTR_ALIGN16(param.pairBuff);

	UInt32 numPairs = 0;

	for (UInt32 i = 0; i < numProxies; i++)
	{
		for (UInt32 j = i + 1; j < numProxies; j++)
		{
			BroadphaseProxy proxyA, proxyB;
			if (GetObjectID(proxies[i]) < GetObjectID(proxies[j]))
			{
				proxyA = proxies[i];
				proxyB = proxies[j];
			}
			else
			{
				proxyA = proxies[j];
				proxyB = proxies[i];
			}

			
			if (GetXYZMax(proxyA, axis) < GetXYZMin(proxyB, axis))
				break;

			if (CheckCollidableBroadphase(proxyA, proxyB))
			{
				if (numPairs >= maxPairs)
					return -1;

				BroadphasePair& pair = pairs[numPairs++];
				SetActive(pair, true);
				SetObjectIDA(pair, GetObjectID(proxyA));
				SetObjectIDB(pair, GetObjectID(proxyB));
				SetMotionMaskA(pair, GetMotionMask(proxyA));
				SetMotionMaskB(pair, GetMotionMask(proxyB));

				SetKey(pair, CreateUniqueKey(GetObjectID(proxyA), GetObjectID(proxyB)));

			}

		}
	}

	Sort(pairs, (BroadphasePair*)workBuff, numPairs);

	result.pairs = pairs;
	result.numPairs = numPairs;

	return 0;
}

int DecomposePairs(DecomposePairParam& param, DecomposePairResult& result)
{
	int ret = CheckParamOfDecomposePairs(param, 0);
	if (ret != 0)
		return ret;


	BroadphasePair* previousPairs = param.previousPairs;
	UInt32 numPreviousPairs = param.numPreviousPairs;
	BroadphasePair* currentPairs = param.currentPairs;
	UInt32 numCurrentPairs = param.numCurrentPairs;

	BroadphasePair* outNewPairs = (BroadphasePair*)PTR_ALIGN16(param.pairBuff);
	BroadphasePair* outKeepPairs = outNewPairs + numCurrentPairs;
	BroadphasePair* outRemovePairs = outKeepPairs + numPreviousPairs;

	UInt32 nNew = 0;
	UInt32 nKeep = 0;
	UInt32 nRemove = 0;

	UInt32 oldID = 0;
	UInt32 newID = 0;

	while (oldID < numPreviousPairs&&newID < numCurrentPairs)
	{
		//remove
		if (GetKey(currentPairs[newID]) > GetKey(previousPairs[oldID]))
		{
			outRemovePairs[nRemove] = previousPairs[oldID];
			nRemove++;
			oldID++;
		}
		//keep
		else if (GetKey(currentPairs[newID])==GetKey(previousPairs[oldID]))
		{
			outKeepPairs[nKeep] = currentPairs[newID];
			SetContactID(outKeepPairs[nKeep], GetContactID(previousPairs[oldID]));
			nKeep++;
			oldID++;
			newID++;
		}
		//new
		else
		{
			outNewPairs[nNew] = currentPairs[newID];
			nNew++;
			newID++;
		}
	}

	//all new
	if (newID < numCurrentPairs)
	{
		while (newID < numCurrentPairs)
		{
			outNewPairs[nNew] = currentPairs[newID];

			newID++;
			nNew++;
		}
	}
	//all remove
	else if (oldID < numPreviousPairs)
	{
		while (oldID < numPreviousPairs)
		{
			outRemovePairs[nRemove] = previousPairs[oldID];
	
			oldID++;
			nRemove++;
		}

	}

	result.outNewPairs = outNewPairs;
	result.outKeepPairs = outKeepPairs;
	result.outRemovePairs = outRemovePairs;
	result.numOutNewPairs = nNew;
	result.numOutKeepPairs = nKeep;
	result.numOutRemovePairs = nRemove;

	return 0;
}