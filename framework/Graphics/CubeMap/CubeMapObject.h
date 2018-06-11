#pragma once
#include<D3DX11.h>
#include<d3dx9.h>
#include"../../Utility/Macro.h"
//キューブマップ用のオブジェクト
class CubeMapObject
{
private://メンバ変数
	ID3D11Texture2D* pCubeTexture;
	ID3D11RenderTargetView* pRTV;
	ID3D11ShaderResourceView* pSRV;
	ID3D11Texture2D* pDepthTexture;
	ID3D11DepthStencilView* pDSV;

	D3D11_VIEWPORT vp;

	D3DXVECTOR3 cubeCenter;		//キューブマップの中心
	D3DXVECTOR3 cubeScale;		//キューブマップの大きさ
public:
	CubeMapObject()
	{
		pCubeTexture = nullptr;
		pRTV = nullptr;
		pSRV = nullptr;
		pDepthTexture = nullptr;
		pDSV = nullptr;
	}

	~CubeMapObject()
	{
		Destroy();
	}

	bool Create(ID3D11Device* pDevice, const D3DXVECTOR3& pos,const D3DXVECTOR3& scale);

	void Destroy()
	{
		MACRO::SAFE_RELEASE(pCubeTexture);
		MACRO::SAFE_RELEASE(pRTV);
		MACRO::SAFE_RELEASE(pSRV);
		MACRO::SAFE_RELEASE(pDepthTexture);
		MACRO::SAFE_RELEASE(pDSV);
	}

	//座標更新
	void UpdatePosition(const D3DXVECTOR3& pos)
	{
		cubeCenter = pos;
	}

	//キューブマップに書き込み開始
	void RenderStart(ID3D11DeviceContext* pContext);

	//書き込み終了してテクスチャに吐き出し
	void RenderEnd(ID3D11DeviceContext* pContext);

	//ピクセルシェーダにセット
	void SetTexture(ID3D11DeviceContext* pContext);

private:
};
