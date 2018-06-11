#pragma once 
//�{�[���N���X

#include"Bone.h"
#include"../../Utility/GameMath.h"

class BoneObject
{

public://�����o�ϐ�
	BoneData absoluteOffset;
	BoneData relativeOffset;
	BoneData absolute;
	BoneData relative;
	BoneTree tree;
	bool physicsflag;
public:
	
	BoneObject()
	{
		physicsflag = false;
	}

	//�����p���̊m��
	void DetermineOffset()
	{
		D3DXQuaternionNormalize(&absolute.rot, &absolute.rot);
		absoluteOffset.rot = GameMath::Conj(absolute.rot);
		absoluteOffset.pos = -absolute.pos;

		D3DXMATRIX mat;
		D3DXMatrixRotationQuaternion(&mat, &absolute.rot);

		D3DXQuaternionNormalize(&relative.rot, &relative.rot);
		relativeOffset.rot = GameMath::Conj(relative.rot);
		relativeOffset.pos = -relative.pos;
	}


	//�����p���ɖ߂�
	void ResetPosture()
	{
		absolute.rot = GameMath::Conj(absoluteOffset.rot);
		absolute.pos = -absoluteOffset.pos;
		relative.rot = GameMath::Conj(relativeOffset.rot);
		relative.pos = -relativeOffset.pos;
	}

	//���g�Ǝw�肵���{�[���̑��΃{�[���Z�o
	BoneData CalcRelativeBone(const BoneData& bone)
	{
		BoneData ret;

		ret.rot = absolute.rot * GameMath::Conj(bone.rot);

		ret.pos = GameMath::QuaterniontoVec3(
			bone.rot *
			GameMath::Vec3toQuaternion(absolute.pos - bone.pos) *
			GameMath::Conj(bone.rot));

		return ret;
	}

	void SetAbsoluteBone(const float* bone)
	{
		absolute.rot.x = bone[0];
		absolute.rot.y = bone[1];
		absolute.rot.z = bone[2];
		absolute.rot.w = bone[3];
		absolute.pos.x = bone[4];
		absolute.pos.y = bone[5];
		absolute.pos.z = bone[6];
	}

};