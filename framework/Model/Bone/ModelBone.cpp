#include"ModelBone.h"

#include"../../Shader/ConstantBuffer/ConstantBufferManager.h"

//アニメーション行列算出
inline D3DXMATRIX CalcAnimMatrix(const BoneObject& bone)
{
	D3DXMATRIX boneMatrix, offsetMatrix;

	
	boneMatrix = GameMath::Get4x4(bone.absolute.rot, bone.absolute.pos);
	offsetMatrix = GameMath::Get4x4(bone.absoluteOffset.rot,
		GameMath::Rotate(bone.absoluteOffset.pos, bone.absoluteOffset.rot));
	D3DXMATRIX ret = offsetMatrix*boneMatrix;

	return ret;

}

void ModelBone::Update(const BoneObject* bone)
{

	for (int i = 0; i < numBones; ++i)
	{
		animMatrix[i] = CalcAnimMatrix(bone[i]);
	}

	GetCBMatrixManager().SetBoneMatrix(animMatrix, numBones);

}

