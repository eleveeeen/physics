#pragma once

#include"BoneObject.h"
//ボーンの拡張クラス
//プレイヤーとかのボーンはすべてこれで
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