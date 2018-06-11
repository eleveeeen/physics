#include"ModelCreator.h"
#include"../ModelBase.h"
#include"../StandardModel.h"
#include"../SkinnedModel.h"
#include"../../DebugConsole/DebugConsole.h"

namespace ModelCreator
{
	//正しい情報が入っているのか確認
	bool CheckParam(const ModelParam& param)
	{
		bool ret = true;
		if (param.numIndex <= 0)
		{
			DEBUG::PrintfColor(EConsoleColor::H_YELLOW, "頂点数未設定\n");
			ret = false;
		}
		if (param.numVertices <= 0)
		{
			DEBUG::PrintfColor(EConsoleColor::H_YELLOW, "インデックス数未設定\n");
			ret = false;
		}

		if (param.pos == nullptr)
		{
			DEBUG::PrintfColor(EConsoleColor::H_YELLOW, "座標情報未設定\n");
			ret = false;
		}

		if (param.norm == nullptr)
		{
			DEBUG::PrintfColor(EConsoleColor::H_YELLOW, "法線情報未設定\n");
			ret = false;
		}


		if (param.uv == nullptr)
		{
			DEBUG::PrintfColor(EConsoleColor::H_YELLOW, "UV情報未設定\n");
			ret = false;
		}


		if (param.index == nullptr)
		{
			DEBUG::PrintfColor(EConsoleColor::H_YELLOW, "インデックス情報未設定\n");
			ret = false;
		}

		return ret;
	}

	ModelBase* Create(const ModelParam& param, ID3D11Device* pDev)
	{
		if (!CheckParam(param))
		{
			DEBUG::PrintfColor(EConsoleColor::H_RED, "モデル作成失敗\n");
			return nullptr;
		}
		
		
		ModelBase* model = new StandardModel;

		//頂点バッファ作成

		model->CreateVertexBuffer(pDev, param.pos, sizeof(D3DXVECTOR3), param.numVertices);

		model->CreateVertexBuffer(pDev, param.norm, sizeof(D3DXVECTOR3), param.numVertices);

		model->CreateVertexBuffer(pDev, param.uv, sizeof(D3DXVECTOR2), param.numVertices);

		//インデックスバッファ作成
		model->CreateIndexBuffer(pDev, param.index, param.numIndex);



		//マテリアル作成

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