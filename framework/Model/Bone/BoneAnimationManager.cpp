#include"BoneAnimationManager.h"
#include"../Animation/AnimationFunc.h"
void BoneAnimationManager::Unlink(const int linkID)
{
	idPool[numPool] = linkID;
	++numPool;

	LinkAnimation& link = links[linkID].linkData;
	link.bone = nullptr;
	link.joint = nullptr;
	link.keyframe = nullptr;
	link.state = nullptr;

	LinkList* back = links[linkID].back;
	LinkList* next = links[linkID].next;

	//�擪�͕K���_�~�[�f�[�^�ɂȂ邩��null�����肷��K�v�͂Ȃ��͂�
	back->next = next;

	if (next)
		next->back = back;

}

void BoneAnimationManager::UnlinkPhysics(const int linkID)
{
	
	LinkAnimation& link = links[linkID].linkData;
	
	//�L�[�t���[��������Ȃ畨����񂾂����
	if (link.keyframe)
	{
		link.joint = nullptr;
		link.state = nullptr;
	}
	//�Ȃ����烊���N��񂲂Ɖ��
	else
	{
		Unlink(linkID);
	}


}

void BoneAnimationManager::UnlinkKeyframe(const int linkID)
{

	LinkAnimation& link = links[linkID].linkData;

	//����������Ȃ�L�[�t���[���������
	if (link.joint && link.state)
	{
		link.keyframe = nullptr;
	}
	else
	{
		Unlink(linkID);
	}
}

int BoneAnimationManager::NewLinkBone(BoneObject& bone)
{
	//	idPool��id�������Ă��Ȃ��Ȃ�	 link�̐���id�ɂ���link�̐����C���N�������g : idPool�ɓ����Ă���id���������pool�̐����f�N�������g
	int linkID = (numPool == 0) ? numLinks++ : idPool[--numPool];

	LinkList& link = links[linkID];
	link.Reset();

	link.SetBone(bone);

	link.SetAnimType(eAnimType::AnimNone);

	Link(link);


	return linkID;
}

int BoneAnimationManager::NewLinkPhysics(BoneObject& bone, const RigidState& state, const Joint& joint, eAnimType type)
{
	//	idPool��id�������Ă��Ȃ��Ȃ�	 link�̐���id�ɂ���link�̐����C���N�������g : idPool�ɓ����Ă���id���������pool�̐����f�N�������g
	int linkID = (numPool == 0) ? numLinks++ : idPool[--numPool];

	LinkList& link = links[linkID];
	link.Reset();

	link.SetBone(bone);

	link.SetJoint(joint);

	link.SetRigidbody(state);

	link.SetAnimType(type);

	Link(link);

	bone.physicsflag = true;

	
	return linkID;
}

int BoneAnimationManager::NewLinkKeyframe(BoneObject& bone, const Keyframe& keyframe, eAnimType type)
{

	//	idPool��id�������Ă��Ȃ��Ȃ�	 link�̐���id�ɂ���link�̐����C���N�������g : idPool�ɓ����Ă���id���������pool�̐����f�N�������g
	int linkID = (numPool == 0) ? numLinks++ : idPool[--numPool];

	LinkList& link = links[linkID];
	link.Reset();

	link.SetBone(bone);

	link.SetKeyframe(keyframe);

	link.SetAnimType(type);

	Link(link);


	return linkID;


}



int BoneAnimationManager::NewLink(BoneObject& bone, const RigidState& state, const Joint& joint, const Keyframe& keyframe, eAnimType type)
{


	//	idPool��id�������Ă��Ȃ��Ȃ�	 link�̐���id�ɂ���link�̐����C���N�������g : idPool�ɓ����Ă���id���������pool�̐����f�N�������g
	int linkID = (numPool == 0) ? numLinks++ : idPool[--numPool];
	
	LinkList& link = links[linkID];
	link.Reset();

	link.SetBone(bone);

	link.SetJoint(joint);

	link.SetRigidbody(state);

	link.SetKeyframe(keyframe);

	link.SetAnimType(type);

	Link(link);

	
	return linkID;
}

void BoneAnimationManager::Update()
{
	for (auto now = this->listTop.next; now;now=now->next)
	{
		GetAnimationFunc(now->GetAnimType())(now->linkData);
	}

}

void BoneAnimationManager::Update(const int linkID)
{
	LinkList& link = links[linkID];
	GetAnimationFunc(link.GetAnimType())(link.linkData);
}


void BoneAnimationManager::Update(const int numIDs,const int* ids)
{
	for (int i = 0; i < numIDs; ++i)
	{
		LinkList& link = links[ids[i]];
		GetAnimationFunc(link.GetAnimType())(link.linkData);
	}
}


bool BoneAnimationManager::AddLinkKeyframe(int linkID, const Keyframe& keyframe)
{
	//���ɓo�^����Ă����珈���������I��
	if (links[linkID].GetKeyframe())
	{
		return false;
	}

	links[linkID].SetKeyframe(keyframe);


	return true;
}

bool BoneAnimationManager::AddLinkPhysics(int linkID, const RigidState& state, const Joint& joint)
{
	if (links[linkID].GetJoint() || links[linkID].GetRigidBody())
	{
		return false;
	}
	

	links[linkID].SetRigidbody(state);
	links[linkID].SetJoint(joint);
	
	return true;
}