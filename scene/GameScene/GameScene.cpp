#include"GameScene.h"
#include"../../framework/Input/CInputKeyboard.h"
#include"../SceneList.h"
#include"../../framework/Graphics/GraphicsManager.h"

#include"../../framework/GameManager/GameAnimation.h"

bool GameScene::enablePhysics = true;
bool GameScene::shadingGouraud = true;

void GameScene::SceneStart()
{
	Initialize();
}

void GameScene::SceneEnd()
{
	Finalize();
}

int GameScene::Initialize()
{

	shadingGouraud = true;

	//スフィアマップの情報を作成してセット
	int id = GetTextureManager().CreateTexture("domeSphere.png");
	GetTextureManager().PSSetTexture(id, PSMappingType::SphereMap);

	
	SetPhysicsMgr(&physics);
	SetKeyframeMgr(&keyframe);
	SetBoneMgr(&bone);
	SetRenderPhysicsMgr(&renderPhysics);


	InitializeGameObject();

	return 0;
}

int GameScene::Finalize()
{

	FinalizeGameObject();

	return 0;
}


BaseScene* GameScene::Update()
{
	const BYTE changeShadingMode = VK_1;	//法線の状態切り替え
	const BYTE changePhysicsMode = VK_2;	//物理の状態切り替え
	const BYTE stepRunning = VK_3;			//物理のステップ実行 enablePhysicsがfalseの時のみ有効
	const BYTE sceneReset = VK_F5;			//シーンのリセット

	//描画用パイプラインをリセット
	GetGraphicsPipeline().Crear();

	UpdateGameObject();


	//物理の状態切り替え
	if (InputKeyManager()->GetKeyTrigger(changePhysicsMode))
	{
		enablePhysics = !enablePhysics;
	}

	//法線の状態切り替え
	if (InputKeyManager()->GetKeyTrigger(changeShadingMode))
	{
		shadingGouraud = !shadingGouraud;
	}


	//物理更新
	if (enablePhysics)
	{
		physics.Simulate();
	}
	else
	{
		//ステップ実行
		if (InputKeyManager()->GetKeyTrigger(stepRunning))
			physics.Simulate();
	}


	//キーフレーム更新
	keyframe.Update();

	//ボーン更新
	bone.Update();

	//剛体描画設定
	renderPhysics.Update();



	//シーンリセット
	if (InputKeyManager()->GetKeyTrigger(sceneReset))
	{
		return new GameScene;
	}

	return this;
}


void GameScene::Render()
{

	//キューブマップへの描画開始
	GraphicsManager::GetInstance().DrawCubeMap();

	//描画開始
	GraphicsManager::GetInstance().RenderStart();

	GraphicsManager::GetInstance().SetShadingMode((GraphicsManager::ShadingMode)shadingGouraud);

	GraphicsManager::GetInstance().Draw3D();

	GraphicsManager::GetInstance().RenderEnd();

}

//シーンのゲームオブジェクト初期化
void GameScene::InitializeGameObject()
{


	//地面設定
	const float worldScale = 150.0f;
	physics.SetWorldSize(worldScale);
	stage.Initialize(worldScale);

	player.Initialize();

	field.Initialize();

	for (int i = 0; i < numYuuki; ++i)
	{
		D3DXQUATERNION q;
		D3DXQuaternionRotationAxis(&q, &D3DXVECTOR3(0, 1, 0), D3DXToRadian(36)*i);

		yuuki[i].Initialize(D3DXVECTOR3(i * 2, 0, 0), q, true);
	}
	
	{
		//ラグドールの初期座標
		const D3DXVECTOR3 initPos[numRagdoll] = {
			D3DXVECTOR3(0, 3, 0),
			D3DXVECTOR3(3, 3, 0),
			D3DXVECTOR3(0, 3, 3),
			D3DXVECTOR3(-3, 3, 0),
			D3DXVECTOR3(0, 3, -3),
			D3DXVECTOR3(3, 3, 3),
			D3DXVECTOR3(-3, 30, 1),
			D3DXVECTOR3(1.5, 10, 2),
			D3DXVECTOR3(3, 40, 1),
			D3DXVECTOR3(0, 40, 0),
		};
		for (int i = 0; i < numRagdoll; ++i)
		{
			ragdoll[i].Initialize(initPos[i]);
		}

	}

	{
		const float angle = 120.0f;
		D3DXQUATERNION q;
		D3DXVECTOR3 offset(-1.65, 0, -4);
		D3DXQuaternionRotationAxis(&q, &D3DXVECTOR3(0, 1, 0), D3DXToRadian(0));
		door[0].Initialize(D3DXVECTOR3(0.4, 0, 0) + offset, q, -angle, 0.0f);
		D3DXQuaternionRotationAxis(&q, &D3DXVECTOR3(0, 1, 0), D3DXToRadian(180));
		door[1].Initialize(D3DXVECTOR3(-0.4, 0, 0) + offset , q, angle, 0.0f);

	}

}

//シーンのゲームオブジェクト更新
void GameScene::UpdateGameObject()
{
	//プレイヤー更新
	player.Update();

	//スカイドーム更新
	stage.Update();


	//フィールド更新
	field.Update();

	//ゆーきたん更新
	for (int i = 0; i < numYuuki; ++i)
	{
		yuuki[i].Update();
	}

	for (int i = 0; i < numDoor; ++i)
	{
		door[i].Update();
	}
}


//シーンのゲームオブジェクト終了
void GameScene::FinalizeGameObject()
{
	stage.Finalize();

	field.Finalize();

	ragdoll[0].Finalize();

	player.Finalize();

	for (int i = 0; i < numYuuki; ++i)
	{
		yuuki[i].Finalize();
	}

	for (int i = 0; i < numRagdoll; ++i)
	{
		ragdoll[i].Finalize();
	}

	for (int i = 0; i < numDoor; ++i)
	{
		door[i].Finalize();
	}

}