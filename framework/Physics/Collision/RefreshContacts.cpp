#include"../Sort/Sort.h"
#include"RefreshContacts.h"

int CheckParamOfRefreshContacts(RefreshContactsParam& param)
{
	if (!param.contactPairs || !param.offsetContactManifolds || !param.offsetRigidStates)
		return -1;
	if (!PTR_IS_ALIGNED16(param.contactPairs) || !PTR_IS_ALIGNED16(param.offsetContactManifolds) ||
		!PTR_IS_ALIGNED16(param.offsetRigidStates))
		return -2;

	return 0;
}

int RefreshContacts(RefreshContactsParam& param)
{
	int ret = CheckParamOfRefreshContacts(param);
	if (ret != 0)
		return ret;

	ConstraintPair* contactPairs = param.contactPairs;
	UInt32 numContactPairs = param.numContactPairs;
	ContactManifold* offsetContactManifolds = param.offsetContactManifolds;
	RigidState* offsetRigidStates = param.offsetRigidStates;
	UInt32 numRigidBodies = param.numRigidBodies;

	for (UInt32 i = 0; i < numContactPairs; i++)
	{
		BroadphasePair& pair = contactPairs[i];

		UInt32 iContact = GetContactID(pair);
		UInt32 iA = GetObjectIDA(pair);
		UInt32 iB = GetObjectIDB(pair);

		ContactManifold& contact = offsetContactManifolds[iContact];

		RigidState& instA = offsetRigidStates[iA];
		RigidState& instB = offsetRigidStates[iB];

		contact.Refresh(
			instA.GetPosition(), instA.GetOrientation(),
			instB.GetPosition(), instB.GetOrientation());

	}

	return 0;

}