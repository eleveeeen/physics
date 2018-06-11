#include"Ragdoll.h"


#include"../../../../framework/GameManager/GameAnimation.h"

Ragdoll::Ragdoll()
{

}

Ragdoll::~Ragdoll()
{
	Finalize();
}

void Ragdoll::Initialize(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot)
{
	InitRagdoll(pos, rot);
}

//ƒ‰ƒOƒh[ƒ‹‰Šú‰»
void Ragdoll::InitRagdoll(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot)
{

	//ƒ‰ƒOƒh[ƒ‹‚Ìƒpƒ‰ƒ[ƒ^
	const float radius = 0.1f;
	const eShapeType ragdollShape = eCapsule;

	const eMotionType mType = eMotionType::eActive;

	//Ž¿—Ê‚ÌŠî€
	const float mass = 0.1f;

	//‚»‚ê‚¼‚ê‚Ì„‘Ì‚ÌŽ¿—Ê
	const float massHead = mass * 1.4f;
	const float massBody1 = mass * 1.8f;
	const float massBody2 = mass * 1.8f;
	const float massRightArm1 = mass * 1.4f;
	const float massRightArm2 = mass * 1.4f;
	const float massLeftArm1 = mass * 1.4f;
	const float massLeftArm2 = mass * 1.4f;
	const float massRightLeg1 = mass * 1.4f;
	const float massRightLeg2 = mass * 1.4f;
	const float massLeftLeg1 = mass * 1.4f;
	const float massLeftLeg2 = mass * 1.4f;

	//ƒ[ƒJƒ‹‚Ì‰ñ“]
	D3DXQUATERNION lRot(0, 0, 0, 1);

	RigidParam param;


	//“ª
	param.Reset();
	param.pos = pos + GameMath::Rotate(D3DXVECTOR3(0.0f, 2.22f, 0.0f), rot);
	param.rot = lRot * rot;
	param.radius = 0.15f;
	param.mass = massHead;
	param.type = mType;

	int rigidHead = GetGameAnimMgr()->CreateRigidBody(param, eSphere);

	//135

	int rigidBody, rigidBody2, rigidBody3;
	//“·‘Ì
	//ã‰º‚Å2•ªŠ„
	{

	//	D3DXVECTOR3 pos(0, 1.3, 0);

		D3DXQuaternionRotationAxis(&lRot, &D3DXVECTOR3(0, 0, 1), D3DXToRadian(90));
		//ã	‹¹•Ó‚è
		param.Reset();
		param.rot = lRot * rot;
		param.pos = pos + GameMath::Rotate(D3DXVECTOR3(0, 1.75, 0), rot);

		param.halfLength = 0.15F;
		param.radius = 0.15f;
		param.mass = massBody1;

		param.type = mType;

		rigidBody = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);

		//‰º	• •Ó‚è
		param.Reset();
		param.pos = pos + GameMath::Rotate(D3DXVECTOR3(0, 1.14, 0), rot);
		param.rot = lRot * rot;
		param.radius = 0.15f;

		param.halfLength = 0.15f;
		param.mass = massBody2;

		param.type = mType;

		rigidBody2 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);
	}


	//‰E˜r1
	param.Reset();

	param.pos = pos + GameMath::Rotate(D3DXVECTOR3(0.4, 2, 0), rot);

	D3DXQuaternionRotationAxis(&lRot, &D3DXVECTOR3(0, 0, 1), D3DXToRadian(0));
	param.rot = lRot * rot;

	param.halfLength = 0.1f;
	param.radius = 0.1f;
	param.mass = massRightArm1;

	param.type = mType;

	int rigidRightArm1 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);



	//‰E˜r2
	param.Reset();

	param.pos = pos + GameMath::Rotate(D3DXVECTOR3(0.83, 2, 0), rot);
	D3DXQuaternionRotationAxis(&lRot, &D3DXVECTOR3(0, 0, 1), D3DXToRadian(0));
	param.rot = lRot * rot;

	param.halfLength = 0.1f;
	param.radius = 0.1f;
	param.mass = massRightArm2;

	param.type = mType;

	int rigidRightArm2 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);



	//¶˜r1
	param.Reset();

	param.pos = pos + GameMath::Rotate(D3DXVECTOR3(-0.4, 2, 0), rot);

	D3DXQuaternionRotationAxis(&lRot, &D3DXVECTOR3(0, 0, 1), D3DXToRadian(0));
	param.rot = lRot * rot;

	param.halfLength = 0.1f;
	param.radius = 0.1f;
	param.mass = massLeftArm1;

	param.type = mType;

	int rigidLeftArm1 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);



	//¶˜r2
	param.Reset();

	param.pos = pos + GameMath::Rotate(D3DXVECTOR3(-0.83, 2, 0), rot);
	D3DXQuaternionRotationAxis(&lRot, &D3DXVECTOR3(0, 0, 1), D3DXToRadian(0));
	param.rot = lRot * rot;

	param.halfLength = 0.1f;
	param.radius = 0.1f;
	param.mass = massLeftArm2;

	param.type = mType;

	int rigidLeftArm2 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);



	//‰E‘å‘Ú
	param.Reset();

	param.pos = pos + GameMath::Rotate(D3DXVECTOR3(0.15f, 0.72f, 0), rot);
	D3DXQuaternionRotationAxis(&lRot, &D3DXVECTOR3(0, 0, 1), D3DXToRadian(-75));
	param.rot = lRot * rot;

	param.halfLength = 0.1f;
	param.radius = 0.1f;
	param.mass = massRightLeg1;

	param.type = mType;

	int rigidRightLeg1 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);



	//‰E‰º‘Ú
	param.Reset();

	param.pos = pos + GameMath::Rotate(D3DXVECTOR3(0.19f, 0.28f, 0), rot);
	D3DXQuaternionRotationAxis(&lRot, &D3DXVECTOR3(0, 0, 1), D3DXToRadian(90));
	param.rot = lRot * rot;

	param.halfLength = 0.10f;
	param.radius = 0.10f;
	param.mass = massRightLeg2;

	param.type = mType;

	int rigidRightLeg2 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);



	//¶‘å‘Ú
	param.Reset();


	param.pos = pos + GameMath::Rotate(D3DXVECTOR3(-0.15f, 0.72f, 0), rot);
	D3DXQuaternionRotationAxis(&lRot, &D3DXVECTOR3(0, 0, 1), D3DXToRadian(75));
	param.rot = lRot * rot;

	param.halfLength = 0.1f;
	param.radius = 0.1f;
	param.mass = massLeftLeg1;

	param.type = mType;

	int rigidLeftLeg1 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);



	//¶‰º‘Ú
	param.Reset();
	param.pos = pos + GameMath::Rotate(D3DXVECTOR3(-0.19f, 0.28f, 0), rot);
	D3DXQuaternionRotationAxis(&lRot, &D3DXVECTOR3(0, 0, 1), D3DXToRadian(90));
	param.rot = lRot * rot;

	param.halfLength = 0.10f;
	param.radius = 0.10f;
	param.mass = massLeftLeg2;



	param.type = mType;

	int rigidLeftLeg2 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);

	const signed char wormStarting = 1;
	const float damping = 0.001f;

	//„‘Ì‚Æ„‘Ì‚ðŒq‚°‚éƒWƒ‡ƒCƒ“ƒg‚ÌÝ’è

	int jointID;
	//“ª-“·‘Ì
	{
		SwingTwistJointInitParam jparam;
		jparam.anchorPoint = pos + GameMath::Rotate(D3DXVECTOR3(0, 2, 0), rot);
		jparam.twistAxis = D3DXVECTOR3(0, 1, 0);

		jointID = GetGameAnimMgr()->CreateJoint(rigidBody, rigidHead, jparam);

		Joint& joint = GetPhysicsMgr()->GetJoint(jointID);

		joint.constraints[0].warmStarting = wormStarting;
		joint.constraints[1].warmStarting = wormStarting;
		joint.constraints[2].warmStarting = wormStarting;
		joint.constraints[3].damping = damping;
		joint.constraints[4].damping = damping;
		joint.constraints[5].damping = damping;

	
	}

	//“·‘Ì-‰E˜r1
	{
		SwingTwistJointInitParam jparam;
		jparam.anchorPoint = pos + GameMath::Rotate(D3DXVECTOR3(0.1, 2, 0), rot);
		jparam.twistAxis = D3DXVECTOR3(-1, 0, 0);
		jparam.swingLowerAngle = 0.0f;
		jparam.swingUpperAngle = 1.7f;

		jointID = GetGameAnimMgr()->CreateJoint(rigidBody, rigidRightArm1, jparam);

		Joint& joint = GetPhysicsMgr()->GetJoint(jointID);

		joint.constraints[0].warmStarting = wormStarting;
		joint.constraints[1].warmStarting = wormStarting;
		joint.constraints[2].warmStarting = wormStarting;
		joint.constraints[3].damping = damping;
		joint.constraints[4].damping = damping;
		joint.constraints[5].damping = damping;

	}

	//‰E˜r1-‰E˜r2
	{

		HingeJointInitParam jparam;
		jparam.anchorPoint = pos + GameMath::Rotate(D3DXVECTOR3(0.55, 2, 0), rot);
		jparam.axis = D3DXVECTOR3(0, -1, 0);

		jparam.lowerAngle = 0.0f;
		jparam.upperAngle = 2.2f;

		jointID = GetGameAnimMgr()->CreateJoint(rigidRightArm1, rigidRightArm2, jparam);

		Joint& joint = GetPhysicsMgr()->GetJoint(jointID);

		joint.constraints[0].warmStarting = wormStarting;
		joint.constraints[1].warmStarting = wormStarting;
		joint.constraints[2].warmStarting = wormStarting;
		joint.constraints[3].damping = damping;
		joint.constraints[4].damping = damping;
		joint.constraints[5].damping = damping;

	}


	//“·‘Ì-¶˜r1
	{
		SwingTwistJointInitParam jparam;
		jparam.anchorPoint = pos + GameMath::Rotate(D3DXVECTOR3(-0.1, 2, 0), rot);
		jparam.twistAxis = D3DXVECTOR3(1, 0, 0);
		jparam.swingLowerAngle = 0.0f;
		jparam.swingUpperAngle = 1.7f;

		jointID = GetGameAnimMgr()->CreateJoint(rigidBody, rigidLeftArm1, jparam);

		Joint& joint = GetPhysicsMgr()->GetJoint(jointID);

		joint.constraints[0].warmStarting = wormStarting;
		joint.constraints[1].warmStarting = wormStarting;
		joint.constraints[2].warmStarting = wormStarting;
		joint.constraints[3].damping = damping;
		joint.constraints[4].damping = damping;
		joint.constraints[5].damping = damping;

	}

	//¶˜r1-¶˜r2
	{
		HingeJointInitParam jparam;
		jparam.anchorPoint = pos + GameMath::Rotate(D3DXVECTOR3(-0.55, 2, 0), rot);;
		jparam.axis = D3DXVECTOR3(0, 1, 0);
		jparam.lowerAngle = 0.0f;
		jparam.upperAngle = 2.2f;

		jointID = GetGameAnimMgr()->CreateJoint(rigidLeftArm1, rigidLeftArm2, jparam);

		Joint& joint = GetPhysicsMgr()->GetJoint(jointID);

		joint.constraints[0].warmStarting = wormStarting;
		joint.constraints[1].warmStarting = wormStarting;
		joint.constraints[2].warmStarting = wormStarting;
		joint.constraints[3].damping = damping;
		joint.constraints[4].damping = damping;
		joint.constraints[5].damping = damping;


	}

	//“·‘Ì
	{

		HingeJointInitParam jparam;

	
		jparam.anchorPoint = pos + GameMath::Rotate(D3DXVECTOR3(0, 1.45, 0), rot);;

		jparam.axis = D3DXVECTOR3(-1, 0, 0);
		jparam.lowerAngle = -1.2f;
		jparam.upperAngle = 0.3f;

		jointID = GetGameAnimMgr()->CreateJoint(rigidBody2, rigidBody, jparam);

		Joint& joint = GetPhysicsMgr()->GetJoint(jointID);

		joint.constraints[0].warmStarting = wormStarting;
		joint.constraints[1].warmStarting = wormStarting;
		joint.constraints[2].warmStarting = wormStarting;
		joint.constraints[3].damping = damping;
		joint.constraints[4].damping = damping;
		joint.constraints[5].damping = damping;

	}


	//˜•Ó‚è-‰E‘å‘Ú
	{

		//BallJointInitParam jparam;
		UniversalJointInitParam jparam;
		jparam.anchorPoint = pos + GameMath::Rotate(D3DXVECTOR3(0.15f, 0.94f, 0), rot);;
		jparam.axis = D3DXVECTOR3(0, -1, 0);
		jparam.swing1LowerAngle = -0.26f;
		jparam.swing1UpperAngle = 0.26f;
		jparam.swing2LowerAngle = -0.78f;
		jparam.swing2UpperAngle = 0.78f;

		jointID = GetGameAnimMgr()->CreateJoint(rigidBody2, rigidRightLeg1, jparam);

		Joint& joint = GetPhysicsMgr()->GetJoint(jointID);

		joint.constraints[0].warmStarting = wormStarting;
		joint.constraints[1].warmStarting = wormStarting;
		joint.constraints[2].warmStarting = wormStarting;
		joint.constraints[3].damping = damping;
		joint.constraints[4].damping = damping;
		joint.constraints[5].damping = damping;

	

	}
	//‰E‘å‘Ú-‰E‰º‘Ú
	{
		HingeJointInitParam jparam;
		jparam.anchorPoint = pos + GameMath::Rotate(D3DXVECTOR3(0.19f, 0.49f, 0),rot);
		jparam.axis = D3DXVECTOR3(1, 0, 0);
		jparam.lowerAngle = 0.0f;
		jparam.upperAngle = 2.2f;

		jointID = GetGameAnimMgr()->CreateJoint(rigidRightLeg1, rigidRightLeg2, jparam);
		Joint& joint = GetPhysicsMgr()->GetJoint(jointID);

		joint.constraints[0].warmStarting = wormStarting;
		joint.constraints[1].warmStarting = wormStarting;
		joint.constraints[2].warmStarting = wormStarting;
		joint.constraints[3].damping = damping;
		joint.constraints[4].damping = damping;
		joint.constraints[5].damping = damping;

	
	}

	//˜•Ó‚è-¶‘å‘Ú
	{

		//BallJointInitParam jparam;
		UniversalJointInitParam jparam;
		jparam.anchorPoint = pos + GameMath::Rotate(D3DXVECTOR3(-0.15f, 0.99f, 0), rot);
		jparam.axis = D3DXVECTOR3(0, -1, 0);
		jparam.swing1LowerAngle = -0.26f;
		jparam.swing1UpperAngle = 0.26f;
		jparam.swing2LowerAngle = -0.78f;
		jparam.swing2UpperAngle = 0.78f;

		jointID = GetGameAnimMgr()->CreateJoint(rigidBody2, rigidLeftLeg1, jparam);

		Joint& joint = GetPhysicsMgr()->GetJoint(jointID);

		joint.constraints[0].warmStarting = wormStarting;
		joint.constraints[1].warmStarting = wormStarting;
		joint.constraints[2].warmStarting = wormStarting;
		joint.constraints[3].damping = damping;
		joint.constraints[4].damping = damping;
		joint.constraints[5].damping = damping;


	}
	//¶‘å‘Ú-¶‰º‘Ú
	{
		HingeJointInitParam jparam;
		jparam.anchorPoint = pos + GameMath::Rotate(D3DXVECTOR3(-0.19f, 0.49f, 0), rot);
		jparam.axis = D3DXVECTOR3(1, 0, 0);
		jparam.lowerAngle = 0.0f;
		jparam.upperAngle = 2.2f;

		jointID = GetGameAnimMgr()->CreateJoint(rigidLeftLeg1, rigidLeftLeg2, jparam);

		Joint& joint = GetPhysicsMgr()->GetJoint(jointID);

		joint.constraints[0].warmStarting = wormStarting;
		joint.constraints[1].warmStarting = wormStarting;
		joint.constraints[2].warmStarting = wormStarting;
		joint.constraints[3].damping = damping;
		joint.constraints[4].damping = damping;
		joint.constraints[5].damping = damping;

	}


	PhysicsSimulation& sim = *GetPhysicsMgr();

	GetRenderPhysicsMgr()->Link(sim.GetState(rigidHead), sim.GetCollidable(rigidHead));
	GetRenderPhysicsMgr()->Link(sim.GetState(rigidBody), sim.GetCollidable(rigidBody));
	GetRenderPhysicsMgr()->Link(sim.GetState(rigidBody2), sim.GetCollidable(rigidBody2));
	GetRenderPhysicsMgr()->Link(sim.GetState(rigidRightArm1), sim.GetCollidable(rigidRightArm1));
	GetRenderPhysicsMgr()->Link(sim.GetState(rigidRightArm2), sim.GetCollidable(rigidRightArm2));
	GetRenderPhysicsMgr()->Link(sim.GetState(rigidRightLeg1), sim.GetCollidable(rigidRightLeg1));
	GetRenderPhysicsMgr()->Link(sim.GetState(rigidRightLeg2), sim.GetCollidable(rigidRightLeg2));
	GetRenderPhysicsMgr()->Link(sim.GetState(rigidLeftArm1), sim.GetCollidable(rigidLeftArm1));
	GetRenderPhysicsMgr()->Link(sim.GetState(rigidLeftArm2), sim.GetCollidable(rigidLeftArm2));
	GetRenderPhysicsMgr()->Link(sim.GetState(rigidLeftLeg1), sim.GetCollidable(rigidLeftLeg1));
	GetRenderPhysicsMgr()->Link(sim.GetState(rigidLeftLeg2), sim.GetCollidable(rigidLeftLeg2));

}