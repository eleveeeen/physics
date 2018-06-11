#pragma once

//仮のmsaaのヘッダー
//早いうちに消す
#include<d3dx9.h>
#include<D3DX11.h>
#include"../Shader/Shader/ShaderManager.h"
class MSAAObj
{

private:
	ID3D11Buffer* v[2];//頂点座標,UV
	ID3D11Buffer* idx;//インデックスバッファ

	ID3D11DeviceContext* pContext;

	int vsID;
	int gsID;
	int psID;


public:

	void Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
	{
		pContext = context;
		LayoutOption layout[] =
		{
			POSITION,
			TEXCOORD,
			LAYOUTEND,
		};

		vsID = 2;
		psID = gsID = 1;
		//頂点バッファ作成

		D3DXVECTOR3 vertexArray[4] = {
			D3DXVECTOR3(-1, 1, 0),
			D3DXVECTOR3(1, 1, 0),
			D3DXVECTOR3(1, -1, 0),
			D3DXVECTOR3(-1, -1, 0)
		};

		D3DXVECTOR2 uvArray[4] = {
			D3DXVECTOR2(0, 0),
			D3DXVECTOR2(1, 0),
			D3DXVECTOR2(1, 1),
			D3DXVECTOR2(0, 1)
		};

		UINT idxArray[] = {
			0, 1, 2, 0, 2, 3
		};

		D3D11_BUFFER_DESC dc;
		dc.Usage = D3D11_USAGE_DEFAULT;
		dc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		dc.CPUAccessFlags = 0;
		dc.MiscFlags = 0;
		dc.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA sub;
		sub.SysMemPitch = 0;
		sub.SysMemSlicePitch = 0;

		//頂点
		dc.ByteWidth = sizeof(D3DXVECTOR3) * 4;
		sub.pSysMem = vertexArray;
		device->CreateBuffer(&dc, &sub, &v[0]);
		//UV
		dc.ByteWidth = sizeof(D3DXVECTOR2) * 4;
		sub.pSysMem = uvArray;
		device->CreateBuffer(&dc, &sub, &v[1]);


		//インデックスバッファの定義
		D3D11_BUFFER_DESC idxDesc;
		idxDesc.Usage = D3D11_USAGE_DEFAULT;
		idxDesc.ByteWidth = sizeof(UINT) * 6;
		idxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		idxDesc.CPUAccessFlags = 0;
		idxDesc.MiscFlags = 0;
		idxDesc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA idxSub;
		idxSub.pSysMem = idxArray;
		idxSub.SysMemPitch = 0;
		idxSub.SysMemSlicePitch = 0;

		device->CreateBuffer(&idxDesc, &idxSub, &idx);

	}

	void Draw()
	{
		GetShaderManager().GetVertexShader(2)->SetShader(pContext);
		GetShaderManager().GetGeometryShader(1)->SetShader(pContext);
		GetShaderManager().GetPixelShader(1)->SetShader(pContext);

		UINT stride[] = { 12, 8 };
		UINT offset[] = { 0, 0 };


		pContext->IASetVertexBuffers(0, 2, v, stride, offset);
		pContext->IASetIndexBuffer(idx, DXGI_FORMAT_R32_UINT, 0);

		pContext->DrawIndexed(6, 0, 0);
	}

	MSAAObj()
	{
		idx = nullptr;
		v[0] = nullptr;
		v[1] = nullptr;
	}
	~MSAAObj()
	{
		MACRO::SAFE_RELEASE(idx);
		MACRO::SAFE_RELEASE_ARRAY(v, 2);

	}


};