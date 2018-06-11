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

	//�X�t�B�A�}�b�v�̏����쐬���ăZ�b�g
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
	const BYTE changeShadingMode = VK_1;	//�@���̏�Ԑ؂�ւ�
	const BYTE changePhysicsMode = VK_2;	//�����̏�Ԑ؂�ւ�
	const BYTE stepRunning = VK_3;			//�����̃X�e�b�v���s enablePhysics��false�̎��̂ݗL��
	const BYTE sceneReset = VK_F5;			//�V�[���̃��Z�b�g

	//�`��p�p�C�v���C�������Z�b�g
	GetGraphicsPipeline().Crear();

	UpdateGameObject();


	//�����̏�Ԑ؂�ւ�
	if (InputKeyManager()->GetKeyTrigger(changePhysicsMode))
	{
		enablePhysics = !enablePhysics;
	}

	//�@���̏�Ԑ؂�ւ�
	if (InputKeyManager()->GetKeyTrigger(changeShadingMode))
	{
		shadingGouraud = !shadingGouraud;
	}


	//�����X�V
	if (enablePhysics)
	{
		physics.Simulate();
	}
	else
	{
		//�X�e�b�v���s
		if (InputKeyManager()->GetKeyTrigger(stepRunning))
			physics.Simulate();
	}


	//�L�[�t���[���X�V
	keyframe.Update();

	//�{�[���X�V
	bone.Update();

	//���̕`��ݒ�
	renderPhysics.Update();



	//�V�[�����Z�b�g
	if (InputKeyManager()->GetKeyTrigger(sceneReset))
	{
		return new GameScene;
	}

	return this;
}


void GameScene::Render()
{

	//�L���[�u�}�b�v�ւ̕`��J�n
	GraphicsManager::GetInstance().DrawCubeMap();

	//�`��J�n
	GraphicsManager::GetInstance().RenderStart();

	GraphicsManager::GetInstance().SetShadingMode((GraphicsManager::ShadingMode)shadingGouraud);

	GraphicsManager::GetInstance().Draw3D();

	GraphicsManager::GetInstance().RenderEnd();

}

//�V�[���̃Q�[���I�u�W�F�N�g������
void GameScene::InitializeGameObject()
{


	//�n�ʐݒ�
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
		//���O�h�[���̏������W
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

//�V�[���̃Q�[���I�u�W�F�N�g�X�V
void GameScene::UpdateGameObject()
{
	//�v���C���[�X�V
	player.Update();

	//�X�J�C�h�[���X�V
	stage.Update();


	//�t�B�[���h�X�V
	field.Update();

	//��[������X�V
	for (int i = 0; i < numYuuki; ++i)
	{
		yuuki[i].Update();
	}

	for (int i = 0; i < numDoor; ++i)
	{
		door[i].Update();
	}
}


//�V�[���̃Q�[���I�u�W�F�N�g�I��
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