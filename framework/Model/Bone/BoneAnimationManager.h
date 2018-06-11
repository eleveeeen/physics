#ifndef _BoneAnimationManager_h_
#define _BoneAnimationManager_h_
//ボーンのアニメーションに関するヘッダー
//ここでやるのはアニメーションの更新結果をボーンに適用するだけで
//更新処理自体はそれぞれのところでやる

#include"ModelBone.h"
#include"../../Physics/Solver/joint.h"
#include"../../Physics/RigidBody/RigidState.h"
#include"../Animation/KeyframeAnimation.h"



//アニメーションの状態
//剛体の状態とできるだけ合わせておく
enum eAnimType
{
	AnimNone = 0,		//アニメーション無し
	AnimPhysics,		//物理駆動 ragdollとか
	AnimKeyframe,		//キーフレームアニメーション
	AnimCount
};

//ボーンと剛体を紐付ける
struct LinkAnimation
{
	const Joint* joint;
	const RigidState* state;
	const Keyframe* keyframe;
	BoneObject* bone;
	eAnimType animType;

	LinkAnimation()
	{
		Reset();
	}

	void Reset()
	{
		joint = nullptr;
		state = nullptr;
		bone = nullptr;
		keyframe = nullptr;
		animType = eAnimType::AnimNone;
	}

};



//ボーンアニメーション管理クラス
class BoneAnimationManager
{
private://定数定義
	static const int MaxLinks = 500;
private://型定義
	//更新処理用のリスト
	struct LinkList
	{
		LinkAnimation linkData;
		LinkList* next;
		LinkList* back;

		LinkList()
		{
			next = nullptr;
			back = nullptr;
		}



		void Reset()
		{
			next = nullptr;
			back = nullptr;
			linkData.Reset();

		}
		const int GetAnimType()const
		{
			return (int)linkData.animType;
		}

		const Joint* GetJoint()const
		{
			return linkData.joint;
		}

		const RigidState* GetRigidBody()const
		{
			return linkData.state;
		}

		const Keyframe* GetKeyframe()const
		{
			return linkData.keyframe;
		}

		void SetBone(BoneObject& bone)
		{
			linkData.bone = &bone;
		}
		void SetJoint(const Joint& joint)
		{
			linkData.joint = &joint;
		}
		void SetRigidbody(const RigidState& state)
		{
			linkData.state = &state;
		}
		void SetKeyframe(const Keyframe& keyframe)
		{
			linkData.keyframe = &keyframe;
		}



		void SetAnimType(const eAnimType animType)
		{
			linkData.animType = animType;
		}


	};

private:
	LinkList links[MaxLinks];

	int idPool[MaxLinks];
	int numLinks;
	int numPool;
	LinkList listTop;	//先頭はダミーデータを持たせておく
	LinkList* listEnd;

public:

	BoneAnimationManager()
	{
		
		listEnd = nullptr;
		numLinks = 0;
		numPool = 0;
	}

public://新規登録

	//とりあえずボーンのみで新規登録
	int NewLinkBone(BoneObject& bone);

	//物理データのみで新規登録
	int NewLinkPhysics(BoneObject& bone, const RigidState& state, const Joint& joint, eAnimType animType = eAnimType::AnimPhysics);


	//キーフレームデータのみで新規登録
	int NewLinkKeyframe(BoneObject& bone, const Keyframe& keyframe, eAnimType animType = eAnimType::AnimKeyframe);
	

	//すべてのデータで新規登録
	int NewLink(BoneObject& bone, const RigidState& state, const Joint& joint, const Keyframe& keyframe, eAnimType animType);
	
private:
	void Link(LinkList& link)
	{
		if (listTop.next)
		{
			listEnd->next = &link;
			listEnd = listEnd->next;
		}
		else
		{
			listTop.next = &link;
			listEnd = &link;
		}
	}

public://追加登録	

	//物理を追加登録　既に紐付いていた場合はfalseを返す
	bool AddLinkPhysics(int linkID, const RigidState& state, const Joint& joint);

	//キーフレームを追加登録　既に紐付いていた場合はfalseを返す
	bool AddLinkKeyframe(int linkID, const Keyframe& keyframe);

public://上書き登録	

	//物理を上書き	おそらく使用する機会はない
	bool OverrideLinkPhysics(int linkID, const RigidState& state, const Joint& joint)
	{
		links[linkID].SetRigidbody(state);
		links[linkID].SetJoint(joint);
	}

	//キーフレームを上書き
	bool OverrideLinkKeyframe(int linkID, const Keyframe& keyframe)
	{
		links[linkID].SetKeyframe(keyframe);
	}

public://複製	必要になれば作成


public://解放

	//すべての情報を解放する
	void Unlink(const int linkID);
	//物理情報を解放
	void UnlinkPhysics(const int linkID);
	//キーフレーム情報を解放
	void UnlinkKeyframe(const int linkID);
	

public:
	void ChangeAnimation(const int linkID,const eAnimType type) //アニメーションの状態を変更
	{
		links[linkID].SetAnimType(type);
	}

	void Update();//稼働しているすべてのアニメーションを更新
	
//指定したidのアニメーションだけ更新	
	
	void Update(const int linkID);					//単体の場合
	void Update(const int numIDs, const int* ids);	//複数体の場合

public:


private:


};


#endif