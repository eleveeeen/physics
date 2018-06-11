#ifndef _CollisionDetection_h_
#define _CollisionDetection_h_

#include"../RigidBody/RigidState.h"
#include"Collidable.h"
#include"ContactManifold.h"
#include"../Solver/ConstraintPair.h"
#include"../Task/TaskManager.h"

struct DetectCollisionParam
{
	ConstraintPair* contactPairs;
	UInt32 numContactPairs;
	ContactManifold* offsetContactManifolds;
	RigidState* offsetRigidStates;
	Collidable* offsetCollidables;
	UInt32 numRigidBodies;
};

int DetectCollision(DetectCollisionParam& param);

int DetectCollision(DetectCollisionParam& param, TaskManager* taskManager);



#endif