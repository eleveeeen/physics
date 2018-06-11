#ifndef _GameAnimation_h_
#define _GameAnimation_h_

#include"../Model/Animation/KeyframeAnimation.h"
#include"../Model/Bone/BoneAnimationManager.h"
#include"../Physics/Physics.h"
#include"../Physics/Solver/JointBall.h"
#include"../Physics/Solver/jointSwingTwist.h"
#include"../Physics/Solver/jointHinge.h"
#include"../Physics/Solver/JointFix.h"
#include"../Physics/Solver/JointSlider.h"
#include"../Physics/Solver/jointUniversal.h"
#include"RenderPhysicsManager.h"

//�Q�[�������Ŏg���A�j���[�V�����̃}�l�[�W��
//�V���O���g��
class GameAnimationManager
{
private:
	KeyframeManager* keyframe;
	BoneAnimationManager* bone;
	PhysicsSimulation* physics;
	RenderPhysicsManager* renderPhysics;

public:
	//�C���X�^���X�擾
	static GameAnimationManager* GetInstance()
	{
		static GameAnimationManager singleton;
		return &singleton;
	}
public://�Z�b�^�[
	void SetKeyframeManager(KeyframeManager* manager)
	{
		keyframe = manager;
	}

	void SetBoneManager(BoneAnimationManager* manager)
	{
		bone = manager;
	}

	void SetPhysicsManager(PhysicsSimulation* manager)
	{
		physics = manager;
	}

	void SetRenderPhysicsManager(RenderPhysicsManager* manager)
	{
		renderPhysics = manager;
	}

public://�Q�b�^�[
	KeyframeManager* GetKeyframeManager()const
	{
		return keyframe;
	}

	BoneAnimationManager* GetBoneManager()const
	{
		return bone;
	}

	PhysicsSimulation* GetPhysicsManager()const
	{
		return physics;
	}

	RenderPhysicsManager* GetRenderPhysicsManager()const
	{
		return renderPhysics;
	}


public://�X�V	
	void PhysicsUpdate()
	{
		physics->Simulate();
	}
	void KeyframeUpdate()
	{
		keyframe->Update();
	}
	void BoneUpdate()
	{
		bone->Update();
	}
	void RenderPhysics()
	{
		renderPhysics->Update();
	}

public://�}�E�X

	D3DXVECTOR3 PickStart(const D3DXVECTOR3& np, const D3DXVECTOR3& fp)
	{

		return physics->PickStart(np, fp);
	}

	void PickUpdate(const D3DXVECTOR3& p)
	{
		physics->PickUpdate(p);
	}

	void PickEnd()
	{
		physics->PickEnd();
	}



public://����
	//���̐���
	int CreateRigidBody(const RigidParam& param, const eShapeType type);

	int CreateDummyBody(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot)
	{
		return	physics->CreateDummy(pos, rot);
	}

	//�Œ�W���C���g����
	int CreateJoint(const int rigidIDA, const int rigidIDB, FixJointInitParam& param, const float bias = 0.1f);
	//�{�[���W���C���g����
	int CreateJoint(const int rigidIDA, const int rigidIDB, BallJointInitParam& param);
	//�q���W�W���C���g����
	int CreateJoint(const int rigidIDA, const int rigidIDB, HingeJointInitParam& param);
	//�X���C�_�[�W���C���g����
	int CreateJoint(const int rigidIDA, const int rigidIDB, SliderJointInitParam& param);
	//���W���C���g����
	int CreateJoint(const int rigidIDA, const int rigidIDB, SwingTwistJointInitParam& param);
	//2������]������W���C���g����
	int CreateJoint(const int rigidIDA, const int rigidIDB, UniversalJointInitParam& param);


public://�{�[���ƕR�t
	int NewLink(BoneObject& boneData, const int rigidID, const int jointID, const int keyframeID, eAnimType type)
	{
		return bone->NewLink(boneData, physics->GetState(rigidID), physics->GetJoint(jointID),
			keyframe->GetKeyframe(keyframeID), type);
	}
	
	int NewLinkPhysics(BoneObject& boneData, const int rigidID, const int jointID, eAnimType type = eAnimType::AnimPhysics)
	{
		return bone->NewLinkPhysics(boneData, physics->GetState(rigidID), physics->GetJoint(jointID),type);
	}

	int NewLinkKeyframe(BoneObject& boneData, const int keyframeID, eAnimType type = eAnimType::AnimKeyframe)
	{
		return bone->NewLinkKeyframe(boneData, keyframe->GetKeyframe(keyframeID), type);
	}

	int NewLinkBone(BoneObject& boneData);

public://�f�t�H���g�̃��b�V���ƕR�t
	int RenderLink(const int rigidID)
	{

		return renderPhysics->Link(physics->GetState(rigidID), physics->GetCollidable(rigidID));
	}

private:
	GameAnimationManager()
	{
		keyframe = nullptr;
		bone = nullptr;
		physics = nullptr;
	}
};

//��������
inline PhysicsSimulation* GetPhysicsMgr()
{
	return GameAnimationManager::GetInstance()->GetPhysicsManager();
}
inline void SetPhysicsMgr(PhysicsSimulation* manager)
{
	GameAnimationManager::GetInstance()->SetPhysicsManager(manager);
}

//���̕`��
inline RenderPhysicsManager* GetRenderPhysicsMgr()
{
	return GameAnimationManager::GetInstance()->GetRenderPhysicsManager();
}

inline void SetRenderPhysicsMgr(RenderPhysicsManager* manager)
{
	GameAnimationManager::GetInstance()->SetRenderPhysicsManager(manager);
}


//�{�[��
inline BoneAnimationManager* GetBoneMgr()
{
	return GameAnimationManager::GetInstance()->GetBoneManager();
}
inline void SetBoneMgr(BoneAnimationManager* manager)
{
	GameAnimationManager::GetInstance()->SetBoneManager(manager);
}


//�L�[�t���[��
inline KeyframeManager* GetKeyframeMgr()
{
	return GameAnimationManager::GetInstance()->GetKeyframeManager();
}
inline void SetKeyframeMgr(KeyframeManager* manager)
{
	GameAnimationManager::GetInstance()->SetKeyframeManager(manager);
}



inline GameAnimationManager* GetGameAnimMgr()
{
	return GameAnimationManager::GetInstance();
}


#endif