#include"Yuuki.h"


#include"../../../../framework/ResourceManager/Model/ModelManager.h"
#include"../../../../framework/ResourceManager/Bone/BoneManager.h"
#include"../../../../framework/Graphics/CubeMap/CubeMapManager.h"

#include"../../../../framework/GameManager/GameAnimation.h"

Yuuki::Yuuki()
{

}

Yuuki::~Yuuki()
{
	Finalize();
}

void Yuuki::Initialize(const D3DXVECTOR3& initPos,const D3DXQUATERNION& initRot,bool useCubemap)
{


	//ゲームオブジェクトとモデルを紐付
	int id = GetModelManager().CreateModel("yuuki");

	SetModelID(id);

	//ボーン複製
	BoneObjectEx* bone = GetBoneManager().Copy("Bone:yuuki");

	//ボーンがあればラグドールにする
	if (bone)
	{
		

		bone->bone[0].relative.pos += initPos;
		bone->bone[0].relative.rot *= initRot;
		bone->UpdateBone();


		SetBone(*bone);
		InitRagdoll();
	}
	else
	{
		SetTransform(initRot, initPos);
	}

	if (useCubemap)
	{
		//キューブマップ生成
		id = GetCubeMapManager().Create();
		SetCubeMap(id);
	}
}

//描画だけ
void Yuuki::Update()
{

	Draw();
}



//ラグドール初期化
void Yuuki::InitRagdoll()
{
	//それぞれのボーンのID
	//いずれここも自動化
	const int boneRoot = 0;
	const int boneUpperBody = 1;
	const int boneNeck = 4;
	const int boneHead = 5;
	const int boneTop = 10;
	const int boneLeftArm = 49;
	const int boneLeftForeArm = 50;
	const int boneLeftHand = 52;
	const int boneRightArm = 78;
	const int boneRightForeArm = 79;
	const int boneRightHand = 81;
	const int boneHip = 106;
	const int boneLeftUpLeg = 107;
	const int boneLeftLeg = 108;
	const int boneLeftFoot = 109;
	const int boneRightUpLeg = 114;
	const int boneRightLeg = 115;
	const int boneRightFoot = 116;

	//ラグドールのパラメータ
	const float radius = 0.1f;
	const eShapeType ragdollShape = eCapsule;


	//質量の基準
	const float mass = 0.1f;

	//それぞれの剛体の質量
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


	RigidParam param;
	BoneObject* bone = data.data.bone->bone;


	//剛体の中心とボーンの中心の長さ算出
	//めり込み対策として半分以下の長さにする
	auto GetRagdollLength = [&param, &bone](int id)->float
	{
		const float ratio = 0.45f;
		return D3DXVec3Length(&(param.pos - bone[id].absolute.pos)) * ratio;
	};

	const eMotionType mType = eMotionType::eActive;

	//頭
	param.Reset();
	param.pos = GameMath::Center(bone[boneNeck].absolute.pos, bone[boneTop].absolute.pos);
	param.rot = bone[boneNeck].absolute.rot;
	param.radius = GetRagdollLength(boneNeck) * 1.2f;
	param.mass = massHead;

	//	param.type = eFixed;

	int rigidHead = GetGameAnimMgr()->CreateRigidBody(param, eSphere);

	int rigidBody, rigidBody2, rigidBody3;
	//胴体
	//上下で2分割
	{

		D3DXVECTOR3 pos = GameMath::Center(bone[boneUpperBody].absolute.pos, bone[boneNeck].absolute.pos);
		//上	胸辺り
		param.Reset();
		param.rot = bone[boneUpperBody].absolute.rot;
		param.pos = pos;

		float halfLength = GetRagdollLength(boneUpperBody);

		param.halfLength = halfLength;
		param.radius = halfLength;
		param.mass = massBody1;

		param.type = mType;



		rigidBody = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);

		//下	腹辺り
		param.Reset();
		param.pos = GameMath::Center(bone[boneRoot].absolute.pos, bone[boneUpperBody].absolute.pos);
		param.pos.y -= halfLength + 0.03f;
		param.rot = bone[boneRoot].absolute.rot;
		param.radius = halfLength;
		halfLength = GetRagdollLength(boneRoot);
		param.halfLength = halfLength * 1.4f;
		param.mass = massBody2;

		param.type = mType;

		rigidBody2 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);

		//Root用のダミー
		rigidBody3 = GetGameAnimMgr()->CreateDummyBody(bone[boneRoot].absolute.pos,
			bone[boneRoot].absolute.rot);
	}


	//右腕1
	param.Reset();

	param.pos = GameMath::Center(bone[boneRightArm].absolute.pos, bone[boneRightForeArm].absolute.pos);
	param.rot = bone[boneRightArm].absolute.rot;

	param.halfLength = GetRagdollLength(boneRightArm);
	param.radius = param.halfLength;
	param.mass = massRightArm1;

	param.type = mType;

	int rigidRightArm1 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);



	//右腕2
	param.Reset();

	param.pos = GameMath::Center(bone[boneRightForeArm].absolute.pos, bone[boneRightHand].absolute.pos);
	param.rot = bone[boneRightForeArm].absolute.rot;

	param.halfLength = GetRagdollLength(boneRightForeArm);
	param.radius = param.halfLength;
	param.mass = massRightArm2;

	param.type = mType;

	int rigidRightArm2 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);



	//左腕1
	param.Reset();

	param.pos = GameMath::Center(bone[boneLeftArm].absolute.pos, bone[boneLeftForeArm].absolute.pos);
	param.rot = bone[boneLeftArm].absolute.rot;

	param.halfLength = GetRagdollLength(boneLeftArm);
	param.radius = param.halfLength;
	param.mass = massLeftArm1;

	param.type = mType;

	int rigidLeftArm1 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);



	//左腕2
	param.Reset();

	param.pos = GameMath::Center(bone[boneLeftForeArm].absolute.pos, bone[boneLeftHand].absolute.pos);
	param.rot = bone[boneLeftForeArm].absolute.rot;

	param.halfLength = GetRagdollLength(boneLeftForeArm);
	param.radius = param.halfLength;
	param.mass = massLeftArm2;

	param.type = mType;

	int rigidLeftArm2 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);



	//右大腿
	param.Reset();

	param.pos = GameMath::Center(bone[boneRightUpLeg].absolute.pos, bone[boneRightLeg].absolute.pos);
	param.rot = bone[boneRightUpLeg].absolute.rot;

	param.halfLength = GetRagdollLength(boneRightUpLeg)*0.8f;
	param.radius = radius*0.6;
	param.mass = massRightLeg1;

	param.type = mType;

	int rigidRightLeg1 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);



	//右下腿
	param.Reset();

	param.pos = GameMath::Center(bone[boneRightLeg].absolute.pos, bone[boneRightFoot].absolute.pos);
	param.rot = bone[boneRightLeg].absolute.rot;

	param.halfLength = GetRagdollLength(boneRightLeg);
	param.radius = radius*0.6;
	param.mass = massRightLeg2;

	param.type = mType;

	int rigidRightLeg2 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);



	//左大腿
	param.Reset();

	param.pos = GameMath::Center(bone[boneLeftUpLeg].absolute.pos, bone[boneLeftLeg].absolute.pos);
	param.rot = bone[boneLeftUpLeg].absolute.rot;

	param.halfLength = GetRagdollLength(boneLeftUpLeg)*0.8f;
	param.radius = radius*0.6;
	param.mass = massLeftLeg1;

	param.type = mType;

	int rigidLeftLeg1 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);



	//左下腿
	param.Reset();
	param.pos = GameMath::Center(bone[boneLeftLeg].absolute.pos, bone[boneLeftFoot].absolute.pos);
	param.rot = bone[boneLeftLeg].absolute.rot;

	param.halfLength = GetRagdollLength(boneLeftLeg);
	param.radius = radius*0.6;
	param.mass = massLeftLeg2;



	param.type = mType;

	int rigidLeftLeg2 = GetGameAnimMgr()->CreateRigidBody(param, ragdollShape);

	const signed char wormStarting = 1;
	const float damping = 0.001f;

	//剛体と剛体を繋げるジョイントの設定

	int jointID;
	//頭-胴体
	{
		SwingTwistJointInitParam jparam;
		jparam.anchorPoint = bone[boneNeck].absolute.pos;
		jparam.twistAxis = D3DXVECTOR3(0, 1, 0);

		jointID = GetGameAnimMgr()->CreateJoint(rigidBody, rigidHead, jparam);

		Joint& joint = GetPhysicsMgr()->GetJoint(jointID);

		joint.constraints[0].warmStarting = wormStarting;
		joint.constraints[1].warmStarting = wormStarting;
		joint.constraints[2].warmStarting = wormStarting;
		joint.constraints[3].damping = damping;
		joint.constraints[4].damping = damping;
		joint.constraints[5].damping = damping;

		GetGameAnimMgr()->NewLinkPhysics(bone[boneNeck], rigidHead, jointID);


	}

	//胴体-右腕1
	{
		SwingTwistJointInitParam jparam;
		jparam.anchorPoint = bone[boneRightArm].absolute.pos;
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

		GetGameAnimMgr()->NewLinkPhysics(bone[boneRightArm], rigidRightArm1, jointID);

	}

	//右腕1-右腕2
	{

		HingeJointInitParam jparam;
		jparam.anchorPoint = bone[boneRightForeArm].absolute.pos;
		jparam.axis = D3DXVECTOR3(0, 1, 0);

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

		GetGameAnimMgr()->NewLinkPhysics(bone[boneRightForeArm], rigidRightArm2, jointID);

	}


	//胴体-左腕1
	{
		SwingTwistJointInitParam jparam;
		jparam.anchorPoint = bone[boneLeftArm].absolute.pos;
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

		GetGameAnimMgr()->NewLinkPhysics(bone[boneLeftArm], rigidLeftArm1, jointID);

	}

	//左腕1-左腕2
	{
		HingeJointInitParam jparam;
		jparam.anchorPoint = bone[boneLeftForeArm].absolute.pos;
		jparam.axis = D3DXVECTOR3(0, -1, 0);
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

		GetGameAnimMgr()->NewLinkPhysics(bone[boneLeftForeArm], rigidLeftArm2, jointID);

	}

	//胴体
	{

		HingeJointInitParam jparam;

		D3DXVECTOR3 pos = GameMath::Center(
			GetPhysicsMgr()->GetState(rigidBody).GetPosition(),
			GetPhysicsMgr()->GetState(rigidBody2).GetPosition());
		jparam.anchorPoint = pos;
		jparam.anchorPoint = bone[boneUpperBody].absolute.pos;// +D3DXVECTOR3(0, 0.06f, 0);

		jparam.axis = D3DXVECTOR3(-1, 0, 0);
		jparam.lowerAngle = -0.3f;
		jparam.upperAngle = 1.2f;

		jointID = GetGameAnimMgr()->CreateJoint(rigidBody2, rigidBody, jparam);

		Joint& joint = GetPhysicsMgr()->GetJoint(jointID);

		joint.constraints[0].warmStarting = wormStarting;
		joint.constraints[1].warmStarting = wormStarting;
		joint.constraints[2].warmStarting = wormStarting;
		joint.constraints[3].damping = damping;
		joint.constraints[4].damping = damping;
		joint.constraints[5].damping = damping;


		GetGameAnimMgr()->NewLinkPhysics(bone[boneUpperBody], rigidBody, jointID);

	}

	//Root用
	{


		FixJointInitParam jparam;

		D3DXVECTOR3 pos = GameMath::Center(
			GetPhysicsMgr()->GetState(rigidBody2).GetPosition(),
			GetPhysicsMgr()->GetState(rigidBody3).GetPosition());
		jparam.anchorPoint = pos;
		jparam.anchorPoint = bone[boneRoot].absolute.pos;// +D3DXVECTOR3(0, 0.06f, 0);

		jointID = GetGameAnimMgr()->CreateJoint(rigidBody3, rigidBody2, jparam);

		Joint& joint = GetPhysicsMgr()->GetJoint(jointID);

		joint.constraints[0].warmStarting = wormStarting;
		joint.constraints[1].warmStarting = wormStarting;
		joint.constraints[2].warmStarting = wormStarting;
		joint.constraints[3].damping = damping;
		joint.constraints[4].damping = damping;
		joint.constraints[5].damping = damping;


		GetGameAnimMgr()->NewLinkPhysics(bone[boneRoot], rigidBody2, jointID);

	}

	//腰辺り-右大腿
	{

		//BallJointInitParam jparam;
		UniversalJointInitParam jparam;
		jparam.anchorPoint = bone[boneRightUpLeg].absolute.pos;
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

		GetGameAnimMgr()->NewLinkPhysics(bone[boneRightUpLeg], rigidRightLeg1, jointID);

	}
	//右大腿-右下腿
	{
		HingeJointInitParam jparam;
		jparam.anchorPoint = bone[boneRightLeg].absolute.pos;
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

		GetGameAnimMgr()->NewLinkPhysics(bone[boneRightLeg], rigidRightLeg2, jointID);

	}

	//腰辺り-左大腿
	{

		//BallJointInitParam jparam;
		UniversalJointInitParam jparam;
		jparam.anchorPoint = bone[boneLeftUpLeg].absolute.pos;
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

		GetGameAnimMgr()->NewLinkPhysics(bone[boneLeftUpLeg], rigidLeftLeg1, jointID);

	}
	//左大腿-左下腿
	{
		HingeJointInitParam jparam;
		jparam.anchorPoint = bone[boneLeftLeg].absolute.pos;
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

		GetGameAnimMgr()->NewLinkPhysics(bone[boneLeftLeg], rigidLeftLeg2, jointID);

	}

	//衝突判定用の剛体表示
#ifdef _DEBUG

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
#endif
}