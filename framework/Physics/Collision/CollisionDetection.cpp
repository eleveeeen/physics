#include"ShapeIterator.h"
#include"CollisionDetection.h"
#include"../BroadPhase/CheckCollidable.h"
#include"ContactCache.h"
#include"DetectCollisionFunc.h"
#include"..\..\Utility\GameMath.h"
#include"../../DebugConsole/DebugConsole.h"
bool CheckParamOfDetectCollision(const DetectCollisionParam& param)
{
	if (!param.contactPairs || !param.offsetContactManifolds || !param.offsetRigidStates || !param.offsetCollidables)
		return false;

	if (!PTR_IS_ALIGNED16(param.contactPairs) || !PTR_IS_ALIGNED16(param.offsetContactManifolds) || !PTR_IS_ALIGNED16(param.offsetRigidStates) || !PTR_IS_ALIGNED16(param.offsetCollidables))
		return false;

	return true;
}

#define CONTACT_THRESHOLD 0.0f

int DetectCollision(DetectCollisionParam& param)
{
	if (!CheckParamOfDetectCollision(param))
		return -1;

	ConstraintPair* contactPairs = param.contactPairs;
	UInt32 numContactPairs = param.numContactPairs;
	ContactManifold* offsetContactManifolds = param.offsetContactManifolds;
	RigidState* offsetRigidStates = param.offsetRigidStates;
	Collidable* offsetCollidables = param.offsetCollidables;
	UInt32 numRigidBodies = param.numRigidBodies;

	for (UInt32 i = 0; i < numContactPairs; ++i)
	{

		const BroadphasePair& pair = contactPairs[i];
		if (!CheckCollidableInCollision(pair))
			continue;

		UInt32 iContact = GetContactID(pair);
		UInt32 iA = GetObjectIDA(pair);
		UInt32 iB = GetObjectIDB(pair);

		
		ContactManifold& contact = offsetContactManifolds[iContact];

		RigidState& stateA = offsetRigidStates[iA];
		RigidState& stateB = offsetRigidStates[iB];

		Collidable& collA = offsetCollidables[iA];
		Collidable& collB = offsetCollidables[iB];

		D3DXMATRIX tA0 = GameMath::Get4x4(stateA.GetOrientation(), stateA.GetPosition());
		D3DXMATRIX tB0 = GameMath::Get4x4(stateB.GetOrientation(), stateB.GetPosition());

		ContactCache contactCache;

		ShapeIterator iteShapeA(collA);
		for (UInt32 j = 0; j < collA.GetNumShapes(); ++j, ++iteShapeA)
		{

			const Shape& shapeA = *iteShapeA;
			D3DXMATRIX offsetTrA = shapeA.GetOffsetTransform();
			D3DXMATRIX worldTrA = offsetTrA* tA0;

			ShapeIterator iteShapeB(collB);
			for (UInt32 k = 0; k < collB.GetNumShapes(); ++k, ++iteShapeB)
			{
				const Shape& shapeB = *iteShapeB;
				D3DXMATRIX offsetTrB = shapeB.GetOffsetTransform();
				D3DXMATRIX worldTrB = offsetTrB*tB0;

				if ((shapeA.GetContactFilterSelf()&shapeB.GetContactFilterTarget()) &&
					(shapeA.GetContactFilterTarget()&shapeB.GetContactFilterSelf())) 
				{
					GetDetectCollisionFunc(shapeA.GetType(), shapeB.GetType())(
						contactCache,
						shapeA, offsetTrA, worldTrA, j,
						shapeB, offsetTrB, worldTrB, k,
						CONTACT_THRESHOLD);
				}
			}

		}

		for (int j = 0; j < contactCache.GetNumContacts(); ++j)
		{

			const CachedContactPoint& cp = contactCache.GetContactPoint(j);

			contact.AddContactPoint(
				cp.distance,
				cp.normal,
				cp.localPointA,
				cp.localPointB,
				cp.subData);

		}

	}

	return 0;
}


