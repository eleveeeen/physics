#include"AnimationFunc.h"
#include"../../DebugConsole/DebugConsole.h"

//�A�j���[�V��������
bool AnimationFuncDummy(LinkAnimation& anim)
{
	return true;
}

//�L�[�t���[��
bool AnimationFuncKeyframe(LinkAnimation& anim)
{
	

	anim.bone->relative.rot = anim.keyframe->rot;
	anim.bone->relative.pos = anim.keyframe->pos;


	return true;
}

//�����쓮
bool AnimationFuncPhysics(LinkAnimation& anim)
{

	
	//���΂̉�]��� = �W���C���g�̉�]���i��΁j * ��΂̃{�[���I�t�Z�b�g * ���΂̏�����]���
	anim.bone->absolute.rot = anim.state->GetOrientation();
	anim.bone->relative.rot = GameMath::Conj(anim.bone->relativeOffset.rot) * anim.bone->absolute.rot * anim.bone->absoluteOffset.rot;

	anim.bone->absolute.pos = anim.state->GetPosition()+GameMath::Rotate(anim.joint->anchorB, (anim.state->GetOrientation()));
	
	

	return true;
}


//animation function table
static AnimationFunc funcTblAnimation[eAnimType::AnimCount] =
{
	AnimationFuncDummy,
	AnimationFuncPhysics, 
	AnimationFuncKeyframe
};

AnimationFunc GetAnimationFunc(int animType)
{
	return funcTblAnimation[animType];
}