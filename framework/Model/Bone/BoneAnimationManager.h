#ifndef _BoneAnimationManager_h_
#define _BoneAnimationManager_h_
//�{�[���̃A�j���[�V�����Ɋւ���w�b�_�[
//�����ł��̂̓A�j���[�V�����̍X�V���ʂ��{�[���ɓK�p���邾����
//�X�V�������̂͂��ꂼ��̂Ƃ���ł��

#include"ModelBone.h"
#include"../../Physics/Solver/joint.h"
#include"../../Physics/RigidBody/RigidState.h"
#include"../Animation/KeyframeAnimation.h"



//�A�j���[�V�����̏��
//���̂̏�ԂƂł��邾�����킹�Ă���
enum eAnimType
{
	AnimNone = 0,		//�A�j���[�V��������
	AnimPhysics,		//�����쓮 ragdoll�Ƃ�
	AnimKeyframe,		//�L�[�t���[���A�j���[�V����
	AnimCount
};

//�{�[���ƍ��̂�R�t����
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



//�{�[���A�j���[�V�����Ǘ��N���X
class BoneAnimationManager
{
private://�萔��`
	static const int MaxLinks = 500;
private://�^��`
	//�X�V�����p�̃��X�g
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
	LinkList listTop;	//�擪�̓_�~�[�f�[�^���������Ă���
	LinkList* listEnd;

public:

	BoneAnimationManager()
	{
		
		listEnd = nullptr;
		numLinks = 0;
		numPool = 0;
	}

public://�V�K�o�^

	//�Ƃ肠�����{�[���݂̂ŐV�K�o�^
	int NewLinkBone(BoneObject& bone);

	//�����f�[�^�݂̂ŐV�K�o�^
	int NewLinkPhysics(BoneObject& bone, const RigidState& state, const Joint& joint, eAnimType animType = eAnimType::AnimPhysics);


	//�L�[�t���[���f�[�^�݂̂ŐV�K�o�^
	int NewLinkKeyframe(BoneObject& bone, const Keyframe& keyframe, eAnimType animType = eAnimType::AnimKeyframe);
	

	//���ׂẴf�[�^�ŐV�K�o�^
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

public://�ǉ��o�^	

	//������ǉ��o�^�@���ɕR�t���Ă����ꍇ��false��Ԃ�
	bool AddLinkPhysics(int linkID, const RigidState& state, const Joint& joint);

	//�L�[�t���[����ǉ��o�^�@���ɕR�t���Ă����ꍇ��false��Ԃ�
	bool AddLinkKeyframe(int linkID, const Keyframe& keyframe);

public://�㏑���o�^	

	//�������㏑��	�����炭�g�p����@��͂Ȃ�
	bool OverrideLinkPhysics(int linkID, const RigidState& state, const Joint& joint)
	{
		links[linkID].SetRigidbody(state);
		links[linkID].SetJoint(joint);
	}

	//�L�[�t���[�����㏑��
	bool OverrideLinkKeyframe(int linkID, const Keyframe& keyframe)
	{
		links[linkID].SetKeyframe(keyframe);
	}

public://����	�K�v�ɂȂ�΍쐬


public://���

	//���ׂĂ̏����������
	void Unlink(const int linkID);
	//�����������
	void UnlinkPhysics(const int linkID);
	//�L�[�t���[���������
	void UnlinkKeyframe(const int linkID);
	

public:
	void ChangeAnimation(const int linkID,const eAnimType type) //�A�j���[�V�����̏�Ԃ�ύX
	{
		links[linkID].SetAnimType(type);
	}

	void Update();//�ғ����Ă��邷�ׂẴA�j���[�V�������X�V
	
//�w�肵��id�̃A�j���[�V���������X�V	
	
	void Update(const int linkID);					//�P�̂̏ꍇ
	void Update(const int numIDs, const int* ids);	//�����̂̏ꍇ

public:


private:


};


#endif