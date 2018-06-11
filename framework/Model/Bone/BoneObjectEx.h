#pragma once

#include"BoneObject.h"
//�{�[���̊g���N���X
//�v���C���[�Ƃ��̃{�[���͂��ׂĂ����
#include"../../Utility/Macro.h"

class BoneObjectEx
{

public:
	BoneObject* bone;
	int numBones;
	int boneID;

public:
	BoneObjectEx()
	{
		bone = nullptr;
	}

	~BoneObjectEx()
	{
		MACRO::SAFE_DELETE_ARRAY(bone);
	}


	void UpdateBone();


};