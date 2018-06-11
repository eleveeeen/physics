#pragma once
//�{�[���̍\����


#include<d3dx9.h>

//�K�w���
struct BoneTree
{
	int parentID;	//�e�̃C���f�b�N�X�ԍ�	�Ȃ����-1
	int childID;	//�q
	int siblingID;	//�Z��
	int myID;		//���g
	BoneTree()
	{
		parentID = -1;
		childID = -1;
		siblingID = -1;
		myID = -1;
	}
};

//��]�A�ړ����
struct BoneData
{
	D3DXQUATERNION rot;
	D3DXVECTOR3 pos;
};

