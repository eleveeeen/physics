#pragma once 
//メッシュのポリゴンデータ

#include<D3DX11.h>

#include"../../../Utility/Macro.h"
#include"../../../DebugConsole/DebugConsole.h"

//マテリアルに対する面情報
struct FaceMat
{
	int numPolygons;			//ポリゴン数
	int startPolygonIndex;		//ポリゴンの先頭番号
	int matIndex;				//マテリアルのID

	FaceMat()
	{
		numPolygons = 0;
		startPolygonIndex = 0;
		matIndex = 0;

	}
};


struct MeshPolygon
{

	ID3D11Buffer* indexBuffer;
	int numMaterials;
	FaceMat* face;

	MeshPolygon()
	{
		indexBuffer = nullptr;
		numMaterials = 0;
		face = nullptr;
	}

	~MeshPolygon()
	{
		MACRO::SAFE_RELEASE(indexBuffer);
		MACRO::SAFE_DELETE_ARRAY(face);

	}

	void CreateFaceMat(int num, FaceMat* mat)
	{
		numMaterials = num;
		face = mat;
	}
	
	//インデックスバッファ作成
	int CreateBuffer(ID3D11Device* dev, const void* index, int numPolygons)
	{

		D3D11_BUFFER_DESC dc;
		//UINT * 3vert * num
		dc.ByteWidth = 12  * numPolygons;
		dc.Usage = D3D11_USAGE_DEFAULT;
		dc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		dc.CPUAccessFlags = 0;
		dc.MiscFlags = 0;
		dc.StructureByteStride = 0;


		D3D11_SUBRESOURCE_DATA sub;
		sub.pSysMem = index;
		sub.SysMemPitch = 0;
		sub.SysMemSlicePitch = 0;


#ifdef _DEBUG

		HRESULT hr;
		hr = dev->CreateBuffer(&dc, &sub, &indexBuffer);
		if (FAILED(hr))
		{
			DEBUG::PrintfColor(EConsoleColor::H_RED, "インデックスバッファの作成に失敗\n");
			DEBUG::PrintfColor(EConsoleColor::H_RED, "numPolygons:%d\n", numPolygons);

			return -1;
		}


#else	//リリース時は失敗しないのを出ない前提で
		dev->CreateBuffer(&dc, &sub, &indexBuffer);
#endif

		return 0;

	}

};