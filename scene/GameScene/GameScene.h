#pragma once

#include"../../framework/BaseScene/BaseScene.h"
#include"GameObject\Player\PlayerObject.h"
#include"GameObject\Yuuki\Yuuki.h"
#include"GameObject\Stage\GameStage.h"
#include"GameObject\SceneObjects\Door.h"
#include"GameObject\SceneObjects\Field.h"
#include"GameObject\SceneObjects\Ragdoll.h"

#include"../../framework/Physics/Physics.h"
#include"../../framework/Model/Bone/BoneAnimationManager.h"
#include"../../framework/Model/Animation/KeyframeAnimation.h"
#include"../../framework/GameManager/RenderPhysicsManager.h"
//タイトルのシーン

class GameScene :public BaseScene
{
private://定数定義
	static const int numRagdoll = 10; //シーンに置くラグドールの数
	static const int numYuuki = 2;//シーンに置くゆーきたんの数
	static const int numDoor = 2; //シーンに置くドアの数

private://メンバ変数

	static bool enablePhysics;	//物理が有効かどうかのフラグ 初期値はtrue
	static bool shadingGouraud;	//頂点法線か面法線かのフラグ 初期値はtrue シーンリセット時に初期化

private://メンバ変数
	GameUI ui;
	PlayerObject player;

	Yuuki yuuki[numYuuki];
	Door door[numDoor];
	Ragdoll ragdoll[numRagdoll];
	GameStage stage;
	GameField field;

	KeyframeManager keyframe;
	BoneAnimationManager bone;
	PhysicsSimulation physics;
	RenderPhysicsManager renderPhysics;


public://メンバ関数

	void SceneStart();
	void SceneEnd();
	int Initialize();
	int Finalize();
	BaseScene* Update();
	void Render();

private://メンバ関数
	
	void UpdateGameObject();
	void InitializeGameObject();
	void FinalizeGameObject();
};