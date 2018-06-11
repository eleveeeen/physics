#pragma once
//�`��p�f�[�^�̍\����
#include<d3dx9.h>
#include"../Model/Bone/BoneObjectEx.h"
#include"../Utility/GameMath.h"

struct DrawObject
{
	D3DXMATRIX transform;	//���W�Ƃ���]
	int modelID;			//���f��
	BoneObjectEx* bone;		//�{�[�����@�Ȃ����null
	int cubeMap;			//�L���[�u�}�b�v��ID�@�Ȃ����-1

	DrawObject()
	{
		transform = GameMath::IdentityMatrix();
		modelID = 0;
		bone = nullptr;
		cubeMap = -1;
	}


};

//2D�p
struct DrawFlatObject
{
	D3DXVECTOR2 leftTop;//�|���S���̍���̍��W
	D3DXVECTOR2 size;//�|���S���̕�

	D3DXVECTOR2 texSize;//uv�̃T�C�Y
	D3DXVECTOR2 uv;//�I�t�Z�b�g

	float rot;//��]���@radian����Ȃ��Ċp�x
	int tex;
};