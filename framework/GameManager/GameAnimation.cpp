#include"GameAnimation.h"


int GameAnimationManager::CreateRigidBody(const RigidParam& param, const eShapeType type)
{
	switch (type)
	{
	case eShapeType::eSphere:
	{
		return physics->CreateSphere(param);
	}
	case eShapeType::eBox:
	{
		return physics->CreateBox(param);
	}
	case eShapeType::eCapsule:
	{
		return physics->CreateCapsule(param);
	}
	case eShapeType::eCylinder:
	{
		return physics->CreateCylinder(param);
	}

	}

	return -1;

}


int GameAnimationManager::CreateJoint(const int rigidIDA, const int rigidIDB, FixJointInitParam& param,const float bias)
{

	return physics->CreateFixJoint(rigidIDA, rigidIDB, param.anchorPoint, bias);
}

int GameAnimationManager::CreateJoint(const int rigidIDA, const int rigidIDB, BallJointInitParam& param)
{
	return physics->CreateBallJoint(rigidIDA, rigidIDB, param.anchorPoint);

}

int GameAnimationManager::CreateJoint(const int rigidIDA, const int rigidIDB, HingeJointInitParam& param)
{
	return physics->CreateHingeJoint(rigidIDA, rigidIDB, param.anchorPoint, param.axis, param.lowerAngle, param.upperAngle);
}

int GameAnimationManager::CreateJoint(const int rigidIDA, const int rigidIDB, SliderJointInitParam& param)
{
	return physics->CreateSliderJoint(rigidIDA, rigidIDB, param.anchorPoint, param.direction, param.lowerDistance, param.upperDistance);
}

int GameAnimationManager::CreateJoint(const int rigidIDA, const int rigidIDB, SwingTwistJointInitParam& param)
{
	return physics->CreateSwingTwistJoint(rigidIDA, rigidIDB, param.anchorPoint, param.twistAxis,
		param.twistLowerAngle, param.twistUpperAngle, param.swingLowerAngle, param.swingUpperAngle);

}

int GameAnimationManager::CreateJoint(const int rigidIDA, const int rigidIDB, UniversalJointInitParam& param)
{
	return physics->CreateUniversalJoint(rigidIDA, rigidIDB, param.anchorPoint,
		param.axis, param.upVec, param.swing1LowerAngle, param.swing1UpperAngle,
		param.swing2LowerAngle, param.swing2UpperAngle);
}