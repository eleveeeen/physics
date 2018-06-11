#include"Field.h"

#include"../../../../framework/ResourceManager/Model/ModelManager.h"
#include"../../../../framework/GameManager/GameAnimation.h"
#include"../../../../framework/Graphics/CubeMap/CubeMapManager.h"
GameField::GameField()
{

}

GameField::~GameField()
{

}


void GameField::Initialize()
{

	const char* nameList[] =
	{
		"ironPillar",		//柱1 鉄
		"copperPillar",		//柱2 銅
		"silverPillar",		//柱3 銀
		"platinumPillar",	//柱4 白金
		"yellowPillar",		//柱5 黄色
		"yellowPillar",		//柱6 黄色
		"lightBluePillar",	//柱7 水色
		"lightBluePillar",	//柱8 水色
	};
	
	for (int i = 0; i < 8; ++i)
	{
		int id = GetModelManager().CreateModel(nameList[i]);
		capsule[i].data.modelID = id;
		D3DXMatrixIdentity(&capsule[i].data.transform);
	}

	InitRigidBody();

}

void GameField::Update()
{



	for (int i = 0; i < 8; ++i)
	{
		//剛体の情報に合わせて姿勢更新
		D3DXVECTOR3 pos;
		D3DXQUATERNION rot;
		pos = GetPhysicsMgr()->GetState(rigidID[i]).GetPosition();
		rot = GetPhysicsMgr()->GetState(rigidID[i]).GetOrientation();

		D3DXMatrixRotationQuaternion(&capsule[i].data.transform, &rot);
		memcpy(&capsule[i].data.transform.m[3], pos, sizeof(float) * 3);

		GetGraphicsPipeline().Set(capsule[i]);

	}
}

void GameField::InitRigidBody()
{

	const float mass = 0.001f;
	
	const eMotionType type = eMotionType::eFixed;

	const float halfLength = 2;
	const float radius =1;

	const float pillarXZ = 3.8f;
	//カプセル1
	//柱
	{
		D3DXVECTOR3 pos(-pillarXZ, 0, -pillarXZ);
		D3DXQUATERNION rot;
		D3DXQuaternionRotationAxis(&rot, &D3DXVECTOR3(0, 0, 1), D3DXToRadian(90.0f));
		RigidParam param;
		param.Reset();
		param.pos = pos;
		param.rot = rot;
		param.mass = mass;
		param.halfLength = halfLength;
		param.radius = radius;

		param.type = eMotionType::eFixed;
		rigidID[0] = GetGameAnimMgr()->CreateRigidBody(param, eCapsule);

	}

	//カプセル2
	//柱
	{
		D3DXVECTOR3 pos(-pillarXZ, 0, pillarXZ);
		D3DXQUATERNION rot;
		D3DXQuaternionRotationAxis(&rot, &D3DXVECTOR3(0, 0, 1), D3DXToRadian(90.0f));
		RigidParam param;
		param.Reset();
		param.pos = pos;
		param.rot = rot;
		param.mass = mass;
		param.halfLength = halfLength;
		param.radius = radius;

		param.type = eMotionType::eFixed;
		rigidID[1] = GetGameAnimMgr()->CreateRigidBody(param, eCapsule);

	}


	//カプセル3
	//柱
	{
		D3DXVECTOR3 pos(pillarXZ+0.3f, 0, -pillarXZ);
		D3DXQUATERNION rot;
		D3DXQuaternionRotationAxis(&rot, &D3DXVECTOR3(0, 0, 1), D3DXToRadian(90.0f));
		RigidParam param;
		param.Reset();
		param.pos = pos;
		param.rot = rot;
		param.mass = mass;
		param.halfLength = halfLength;
		param.radius = radius;
		param.type = eMotionType::eFixed;
		rigidID[2] = GetGameAnimMgr()->CreateRigidBody(param, eCapsule);

	}

	//カプセル4
	//柱
	{
		D3DXVECTOR3 pos(pillarXZ, 0, pillarXZ);
		D3DXQUATERNION rot;
		D3DXQuaternionRotationAxis(&rot, &D3DXVECTOR3(0, 0, 1), D3DXToRadian(90.0f));
		RigidParam param;
		param.Reset();
		param.pos = pos;
		param.rot = rot;
		param.mass = mass;
		param.halfLength = halfLength;
		param.radius = radius;

		param.type = eMotionType::eFixed;
		rigidID[3] = GetGameAnimMgr()->CreateRigidBody(param, eCapsule);

	}

	//カプセル5
	//左の柵的なもの
	{
		D3DXVECTOR3 pos(-3.8, 0, -0);
		D3DXQUATERNION rot;
		D3DXQuaternionRotationAxis(&rot, &D3DXVECTOR3(0, 1, 0), D3DXToRadian(90.0f));
		RigidParam param;
		param.Reset();
		param.pos = pos;
		param.rot = rot;
		param.mass = mass;
		param.halfLength = halfLength;
		param.radius = radius;

		param.type = eMotionType::eFixed;
		rigidID[4] = GetGameAnimMgr()->CreateRigidBody(param, eCapsule);

	}

	//カプセル5
	//右の柵的なもの
	{
		D3DXVECTOR3 pos(3.8, 0, -0);
		D3DXQUATERNION rot;
		D3DXQuaternionRotationAxis(&rot, &D3DXVECTOR3(0, 1, 0), D3DXToRadian(90.0f));
		RigidParam param;
		param.Reset();
		param.pos = pos;
		param.rot = rot;
		param.mass = mass;
		param.halfLength = halfLength;
		param.radius = radius;

		param.type = eMotionType::eFixed;
		rigidID[5] = GetGameAnimMgr()->CreateRigidBody(param, eCapsule);

	}

	//カプセル6
	//奥の柵的なもの
	{
		D3DXVECTOR3 pos(0, 0, 4);
		D3DXQUATERNION rot(0, 0, 0, 1);
		RigidParam param;
		param.Reset();
		param.pos = pos;
		param.rot = rot;
		param.mass = mass;
		param.halfLength = halfLength;
		param.radius = radius;

		param.type = eMotionType::eFixed;
		rigidID[6] = GetGameAnimMgr()->CreateRigidBody(param, eCapsule);

	}


	//カプセル7
	//手前の柵的なもの
	{
		D3DXVECTOR3 pos(2.5, 0, -4.4);
		D3DXQUATERNION rot(0, 0, 0, 1);
		RigidParam param;
		param.Reset();
		param.pos = pos;
		param.rot = rot;
		param.mass = mass;
		param.halfLength = halfLength;
		param.radius = radius;

		param.type = eMotionType::eFixed;
		rigidID[7] = GetGameAnimMgr()->CreateRigidBody(param, eCapsule);


	}




	//衝突判定用の剛体表示
#ifdef _DEBUG

	PhysicsSimulation& sim = *GetPhysicsMgr();

	for (int i = 0; i < 8; ++i)
	{
	//	GetRenderPhysicsMgr()->Link(sim.GetState(rigidID[i]), sim.GetCollidable(rigidID[i]));

	}

#endif
}