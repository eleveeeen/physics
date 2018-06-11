#pragma once

#include<D3DX11.h>
#include<string>
#include"../Mesh/Polygon/MeshPolygon.h"
class ModelBase;
//���f���̍쐬
//�v���O�������Ń��f�����쐬�������Ƃ��Ɏg�p
//�X�L�����b�V���̑Ή��͖���
namespace ModelCreator
{
	//���f���̃}�e���A���쐬�p�p�����[�^
	struct MaterialParam
	{
		FaceMat* mat;		//�}�e���A�����
		int numMaterial;	//�}�e���A����
	};

	//���f���쐬�p�p�����[�^
	struct ModelParam
	{
		//���_�o�b�t�@�p�f�[�^
		float* pos;		//���W
		float* norm;		//�@��
		float* uv;		//UV
		int numVertices;	//���_��

		//�C���f�b�N�X�o�b�t�@�p�f�[�^
		int* index;	//���_�C���f�b�N�X
		int numIndex;	//�C���f�b�N�X��

		//�}�e���A���p�f�[�^
		//�Ȃ���΃f�t�H���g�̃}�e���A�������蓖�Ă�
		MaterialParam* matParam;

		//���f����
		std::string name;

		void Reset()
		{
			pos = nullptr;
			norm = nullptr;
			uv = nullptr;
			numVertices = 0;

			index = nullptr;
			numIndex = 0;
			matParam = nullptr;

			name = "";
		}
	};

	ModelBase* Create(const ModelParam& param, ID3D11Device* pDev);
}