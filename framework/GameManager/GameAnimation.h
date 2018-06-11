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

//ゲーム内部で使うアニメーションのマネージャ
//シングルトン
class GameAnimationManager
{
private:
	KeyframeManager* keyframe;
	BoneAnimationManager* bone;
	PhysicsSimulation* physics;
	RenderPhysicsManager* renderPhysics;

public:
	//インスタンス取得
	static GameAnimationManager* GetInstance()
	{
		static GameAnimationManager singleton;
		return &singleton;
	}
public://セッター
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

public://ゲッター
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


public://更新	
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

public://マウス

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



public://生成
	//剛体生成
	int CreateRigidBody(const RigidParam& param, const eShapeType type);

	int CreateDummyBody(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot)
	{
		return	physics->CreateDummy(pos, rot);
	}

	//固定ジョイント生成
	int CreateJoint(const int rigidIDA, const int rigidIDB, FixJointInitParam& param, const float bias = 0.1f);
	//ボールジョイント生成
	int CreateJoint(const int rigidIDA, const int rigidIDB, BallJointInitParam& param);
	//ヒンジジョイント生成
	int CreateJoint(const int rigidIDA, const int rigidIDB, HingeJointInitParam& param);
	//スライダージョイント生成
	int CreateJoint(const int rigidIDA, const int rigidIDB, SliderJointInitParam& param);
	//肩ジョイント生成
	int CreateJoint(const int rigidIDA, const int rigidIDB, SwingTwistJointInitParam& param);
	//2軸を回転させるジョイント生成
	int CreateJoint(const int rigidIDA, const int rigidIDB, UniversalJointInitParam& param);


public://ボーンと紐付
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

public://デフォルトのメッシュと紐付
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

//物理処理
inline PhysicsSimulation* GetPhysicsMgr()
{
	return GameAnimationManager::GetInstance()->GetPhysicsManager();
}
inline void SetPhysicsMgr(PhysicsSimulation* manager)
{
	GameAnimationManager::GetInstance()->SetPhysicsManager(manager);
}

//剛体描画
inline RenderPhysicsManager* GetRenderPhysicsMgr()
{
	return GameAnimationManager::GetInstance()->GetRenderPhysicsManager();
}

inline void SetRenderPhysicsMgr(RenderPhysicsManager* manager)
{
	GameAnimationManager::GetInstance()->SetRenderPhysicsManager(manager);
}


//ボーン
inline BoneAnimationManager* GetBoneMgr()
{
	return GameAnimationManager::GetInstance()->GetBoneManager();
}
inline void SetBoneMgr(BoneAnimationManager* manager)
{
	GameAnimationManager::GetInstance()->SetBoneManager(manager);
}


//キーフレーム
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