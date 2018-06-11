#ifndef _RefreshContacts_h_
#define _RefreshContacts_h_

#include"../RigidBody/RigidState.h"
#include"../Solver/ConstraintPair.h"
#include"../Collision/ContactManifold.h"
#include"../Task/TaskManager.h"

struct RefreshContactsParam
{
	ConstraintPair* contactPairs;
	UInt32 numContactPairs;
	ContactManifold* offsetContactManifolds;
	RigidState* offsetRigidStates;
	UInt32 numRigidBodies;
};

int RefreshContacts(RefreshContactsParam& param);

int RefreshContacts(RefreshContactsParam& param, TaskManager* taskManager);

#endif