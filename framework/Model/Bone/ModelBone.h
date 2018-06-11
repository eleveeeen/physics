#pragma once


#include"BoneObject.h"
#include"../../Utility/Macro.h"
#include<string>
#include<d3dx9.h>

class ModelBone
{
private:
	D3DXMATRIX* animMatrix; //アニメーション行列
	BoneObject* bones;		//ボーン配列
	int numBones;			//ボーンの数
public:
	std::string name;		//ボーン名


	ModelBone()
	{
		animMatrix = nullptr;
		bones = nullptr;
		numBones = 0;
	}

	~ModelBone()
	{
		MACRO::SAFE_DELETE_ARRAY(animMatrix);
		MACRO::SAFE_DELETE_ARRAY(bones);
	}

	BoneObject* Copy()
	{
		BoneObject* copy = new BoneObject[numBones];
		memcpy(copy, bones, sizeof(BoneObject)*numBones);

		return copy;
	}

	void Create(int num, BoneObject* bone)
	{
		numBones = num;
		bones = bone;
		animMatrix = new D3DXMATRIX[num];
		for (int i = 0; i < num; ++i)
		{
			animMatrix[i] = GameMath::IdentityMatrix();
		}
	}

	int GetBoneNum()const
	{
		return numBones;
	}

	void Update(const BoneObject* bone);

};