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
//�^�C�g���̃V�[��

class GameScene :public BaseScene
{
private://�萔��`
	static const int numRagdoll = 10; //�V�[���ɒu�����O�h�[���̐�
	static const int numYuuki = 2;//�V�[���ɒu����[������̐�
	static const int numDoor = 2; //�V�[���ɒu���h�A�̐�

private://�����o�ϐ�

	static bool enablePhysics;	//�������L�����ǂ����̃t���O �����l��true
	static bool shadingGouraud;	//���_�@�����ʖ@�����̃t���O �����l��true �V�[�����Z�b�g���ɏ�����

private://�����o�ϐ�
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


public://�����o�֐�

	void SceneStart();
	void SceneEnd();
	int Initialize();
	int Finalize();
	BaseScene* Update();
	void Render();

private://�����o�֐�
	
	void UpdateGameObject();
	void InitializeGameObject();
	void FinalizeGameObject();
};