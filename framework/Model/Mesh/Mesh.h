//メッシュクラス
#pragma once 

#include<D3DX11.h>
#include<stdio.h>
#include"../../Utility/Macro.h"
#include"Vertex\MeshVertex.h"
#include"Polygon\MeshPolygon.h"



class Mesh
{
private://メンバ変数
	MeshVertex vertex;

	MeshPolygon polygon;
	

public://メンバ関数

	//頂点バッファ作成
	int CreateVertexBuffer(ID3D11Device* dev, const void* vertices, unsigned int stride, unsigned int numVertices)
	{
		return vertex.CreateBuffer(dev, vertices, stride, numVertices);
	}

	//インデックスバッファ作成
	int CreateIndexBuffer(ID3D11Device* dev, const void* index, int numPolygons)
	{
		return polygon.CreateBuffer(dev, index, numPolygons);
	}
	//ポリゴンのマテリアル情報作成
	void CreateFaceMaterial(int num,FaceMat* mat)
	{
		polygon.CreateFaceMat(num, mat);
		
	}

public://ゲッター

	//頂点バッファ取得
	ID3D11Buffer* const* GetVertexBuffer() const
	{
		return (vertex.vertexBuffer);
	}

	//頂点バッファ数取得
	int GetNumVertexBuffer()const
	{
		return vertex.numBuffers;
	}

	//インデックスバッファ取得
	ID3D11Buffer* GetIndexBuffer()const
	{
		return polygon.indexBuffer;
	}

	//ポリゴンのマテリアル情報取得
	FaceMat* GetFaceMaterial()const
	{
		return polygon.face;
	}

	//ポリゴンのマテリアル数取得
	int GetNumFaceMaterial()const
	{
		return polygon.numMaterials;
	}


};
