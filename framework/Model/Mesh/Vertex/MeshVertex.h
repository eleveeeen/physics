#pragma once
//メッシュの頂点データ
#include<D3DX11.h>

#include"../../../Utility/Macro.h"
#include"../../../DebugConsole/DebugConsole.h"
struct MeshVertex
{
	ID3D11Buffer* vertexBuffer[15];
	int numBuffers;//頂点バッファの数

	MeshVertex()
	{
		SecureZeroMemory(vertexBuffer, sizeof(ID3D11Buffer*) * 15);
		numBuffers = 0;
	}

	~MeshVertex()
	{
		MACRO::SAFE_RELEASE_ARRAY(vertexBuffer, numBuffers);
	}

	//頂点データ、バイト幅、頂点数を渡してバッファ作成
	//失敗したら-1を返す
	int CreateBuffer(ID3D11Device* dev, const void* vertices, unsigned int stride, unsigned int numVertices)
	{
		if (numBuffers > 15)
			return -1;//error
		

		D3D11_BUFFER_DESC dc;
		dc.ByteWidth = stride * numVertices;
		dc.Usage = D3D11_USAGE_DEFAULT;
		dc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		dc.CPUAccessFlags = 0;
		dc.MiscFlags = 0;
		dc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sub;
		sub.pSysMem = vertices;
		sub.SysMemPitch = 0;
		sub.SysMemSlicePitch = 0;

#ifdef _DEBUG

		HRESULT hr;
		hr = dev->CreateBuffer(&dc, &sub, &vertexBuffer[numBuffers]);
		if (FAILED(hr))
		{
			DEBUG::PrintfColor(EConsoleColor::H_RED, "頂点バッファの作成に失敗\n");
			DEBUG::PrintfColor(EConsoleColor::H_RED, "Slot:%d\nStride:%d\nVertices:%d\n", numBuffers, stride, numVertices);

			return -1;
		}


#else	//リリース時は失敗しないのを出ない前提で
		dev->CreateBuffer(&dc, &sub, &vertexBuffer[numBuffers]);
#endif


		//一応何番目に入ったかを返す
		return numBuffers++;
	}

};