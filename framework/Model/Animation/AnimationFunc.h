#ifndef _AnimationFunc_h_
#define _AnimationFunc_h_
#include"../Bone/Bone.h"
#include"../Bone/BoneAnimationManager.h"
//�{�[���ό`�̊֐����

typedef bool(*AnimationFunc)(LinkAnimation& anim);

//�A�j���[�V�����̃^�C�v�̊֐����擾
AnimationFunc GetAnimationFunc(int animType);


#endif