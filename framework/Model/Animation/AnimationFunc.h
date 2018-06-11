#ifndef _AnimationFunc_h_
#define _AnimationFunc_h_
#include"../Bone/Bone.h"
#include"../Bone/BoneAnimationManager.h"
//ボーン変形の関数情報

typedef bool(*AnimationFunc)(LinkAnimation& anim);

//アニメーションのタイプの関数を取得
AnimationFunc GetAnimationFunc(int animType);


#endif