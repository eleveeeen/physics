
#include"Door.h"
#include"../../../../framework/ResourceManager/Model/ModelManager.h"
#include"../../../../framework/GameManager/GameAnimation.h"

Door::Door()
{

}

Door::~Door()
{
	Finalize();
}


void Door::Initialize(const D3DXVECTOR3& initPos, const D3DXQUATERNION& initRot, const float lowerAngle, const float upperAngle)
{
	//ゲームオブジェクトとモデル紐付
	int id = GetModelManager().CreateModel("door");
	SetModelID(id);

	SetTransform(initRot, initPos);

	SetCubeMap(GetCubeMapManager().Create());


	id = GetModelManager().CreateModel("goldPillar");
	pillar.data.modelID = id;

	//lowerとupperを逆にしていた場合を考慮してminmaxで直しておく
	InitRigidBody(initPos, initRot,
		min(lowerAngle, upperAngle),
		max(lowerAngle, upperAngle));


}

void Door::Update()
{

	//剛体の情報に合わせて姿勢更新
	D3DXVECTOR3 pos;
	D3DXQUATERNION rot;
	pos = GetPhysicsMgr()->GetState(rigidBox).GetPosition();
	rot = GetPhysicsMgr()->GetState(rigidBox).GetOrientation();
	SetTransform(rot, pos);

	pos = GetPhysicsMgr()->GetState(rigidCapsule).GetPosition();
	rot = GetPhysicsMgr()->GetState(rigidCapsule).GetOrientation();

	D3DXMatrixRotationQuaternion(&pillar.data.transform, &rot);
	memcpy(&pillar.data.transform.m[3], pos, sizeof(float) * 3);




	Draw();
	
}

void Door::Draw()
{
	GetGraphicsPipeline().Set(data);
	GetGraphicsPipeline().Set(pillar);
}


void Door::InitRigidBody(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot, const float lowerAngle, const float upperAngle)
{

	const float mass = 0.025f;

	D3DXVECTOR3 anchor;//ドアと柱をつなげるポイント


	//ドア部分
	//モデルのサイズをそのまま使用したら
	//地面に引っかかるから少し小さめに
	{
		const D3DXVECTOR3 offset(0.0f, 0.72f, 0.0f);
		RigidParam param;
		param.Reset();
		param.pos = pos + offset;
		param.rot = rot;
		param.mass = mass;
		param.x = 0.375f;
		param.y = 0.72f;
		param.z = 0.1f;
		param.friction = 0.2f;
		param.restitution = 0.5f;
		param.type = eMotionType::eActive;
		rigidBox = GetGameAnimMgr()->CreateRigidBody(param, eBox);
	}


	//柱部分
	{
		const D3DXVECTOR3 offset = GameMath::Rotate(D3DXVECTOR3(0.5f, 0.75f, 0.0f),rot);
		D3DXQUATERNION q;
		D3DXQuaternionRotationAxis(&q, &D3DXVECTOR3(0, 0, 1), D3DXToRadian(90.0f));
		RigidParam param;
		param.Reset();
		param.pos = pos + offset;
		param.rot = q;
		param.mass = mass;
		param.halfLength = 0.75f;
		param.radius = 0.1f;

		param.type = eMotionType::eFixed;
		rigidCapsule = GetGameAnimMgr()->CreateRigidBody(param, eCapsule);

		anchor = param.pos;
	}


	//ドアと柱をつなげるヒンジジョイント
	int jointID;
	{
		
		
		const D3DXVECTOR3 offset(0, 0.4f, 0);
		HingeJointInitParam jparam;
		jparam.anchorPoint = anchor + offset;
		jparam.axis = D3DXVECTOR3(0, 1, 0);
		jparam.lowerAngle = D3DXToRadian(lowerAngle);
		jparam.upperAngle = D3DXToRadian(upperAngle);

		jointID = GetGameAnimMgr()->CreateJoint(rigidBox, rigidCapsule, jparam);

		jparam.anchorPoint = anchor - offset;
		jointID = GetGameAnimMgr()->CreateJoint(rigidBox, rigidCapsule, jparam);


		Joint& joint = GetPhysicsMgr()->GetJoint(jointID);

		joint.constraints[0].warmStarting = 1;
		joint.constraints[1].warmStarting = 1;
		joint.constraints[2].warmStarting = 1;
		joint.constraints[3].damping = 0.001f;
		joint.constraints[4].damping = 0.001f;
		joint.constraints[5].damping = 0.001f;

	}





}