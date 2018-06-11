#include"AnimationFunc.h"
#include"../../DebugConsole/DebugConsole.h"

//アニメーション無し
bool AnimationFuncDummy(LinkAnimation& anim)
{
	return true;
}

//キーフレーム
bool AnimationFuncKeyframe(LinkAnimation& anim)
{
	

	anim.bone->relative.rot = anim.keyframe->rot;
	anim.bone->relative.pos = anim.keyframe->pos;


	return true;
}

//物理駆動
bool AnimationFuncPhysics(LinkAnimation& anim)
{

	
	//相対の回転情報 = ジョイントの回転情報（絶対） * 絶対のボーンオフセット * 相対の初期回転情報
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