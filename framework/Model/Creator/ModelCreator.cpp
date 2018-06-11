#include"ModelCreator.h"
#include"../ModelBase.h"
#include"../StandardModel.h"
#include"../SkinnedModel.h"
#include"../../DebugConsole/DebugConsole.h"

namespace ModelCreator
{
	//��������񂪓����Ă���̂��m�F
	bool CheckParam(const ModelParam& param)
	{
		bool ret = true;
		if (param.numIndex <= 0)
		{
			DEBUG::PrintfColor(EConsoleColor::H_YELLOW, "���_�����ݒ�\n");
			ret = false;
		}
		if (param.numVertices <= 0)
		{
			DEBUG::PrintfColor(EConsoleColor::H_YELLOW, "�C���f�b�N�X�����ݒ�\n");
			ret = false;
		}

		if (param.pos == nullptr)
		{
			DEBUG::PrintfColor(EConsoleColor::H_YELLOW, "���W��񖢐ݒ�\n");
			ret = false;
		}

		if (param.norm == nullptr)
		{
			DEBUG::PrintfColor(EConsoleColor::H_YELLOW, "�@����񖢐ݒ�\n");
			ret = false;
		}


		if (param.uv == nullptr)
		{
			DEBUG::PrintfColor(EConsoleColor::H_YELLOW, "UV��񖢐ݒ�\n");
			ret = false;
		}


		if (param.index == nullptr)
		{
			DEBUG::PrintfColor(EConsoleColor::H_YELLOW, "�C���f�b�N�X��񖢐ݒ�\n");
			ret = false;
		}

		return ret;
	}

	ModelBase* Create(const ModelParam& param, ID3D11Device* pDev)
	{
		if (!CheckParam(param))
		{
			DEBUG::PrintfColor(EConsoleColor::H_RED, "���f���쐬���s\n");
			return nullptr;
		}
		
		
		ModelBase* model = new StandardModel;

		//���_�o�b�t�@�쐬

		model->CreateVertexBuffer(pDev, param.pos, sizeof(D3DXVECTOR3), param.numVertices);

		model->CreateVertexBuffer(pDev, param.norm, sizeof(D3DXVECTOR3), param.numVertices);

		model->CreateVertexBuffer(pDev, param.uv, sizeof(D3DXVECTOR2), param.numVertices);

		//�C���f�b�N�X�o�b�t�@�쐬
		model->CreateIndexBuffer(pDev, param.index, param.numIndex);



		//�}�e���A���쐬

		if (param.matParam != nullptr)
		{
			const MaterialParam& matParam = *param.matParam;
			FaceMat* mat = new FaceMat[matParam.numMaterial];
			memcpy(mat, matParam.mat, sizeof(FaceMat) * matParam.numMaterial);

			model->CreateFaceMaterial(matParam.numMaterial, mat);
		}
		else
		{
			FaceMat* defaultMat = new FaceMat[1];
			defaultMat[0].startPolygonIndex = 0;
			defaultMat[0].numPolygons = param.numIndex;
			defaultMat[0].matIndex = 0;
			model->CreateFaceMaterial(1, defaultMat);
		}


		model->name = param.name;

		return model;
	}

}