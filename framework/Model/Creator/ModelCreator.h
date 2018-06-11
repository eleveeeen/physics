#pragma once

#include<D3DX11.h>
#include<string>
#include"../Mesh/Polygon/MeshPolygon.h"
class ModelBase;
//モデルの作成
//プログラム内でモデルを作成したいときに使用
//スキンメッシュの対応は未定
namespace ModelCreator
{
	//モデルのマテリアル作成用パラメータ
	struct MaterialParam
	{
		FaceMat* mat;		//マテリアル情報
		int numMaterial;	//マテリアル数
	};

	//モデル作成用パラメータ
	struct ModelParam
	{
		//頂点バッファ用データ
		float* pos;		//座標
		float* norm;		//法線
		float* uv;		//UV
		int numVertices;	//頂点数

		//インデックスバッファ用データ
		int* index;	//頂点インデックス
		int numIndex;	//インデックス数

		//マテリアル用データ
		//なければデフォルトのマテリアルを割り当てる
		MaterialParam* matParam;

		//モデル名
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