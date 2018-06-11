#include"../Utility/Macro.h"
#include"GraphicsManager.h"

#include"../Renderer/MeshRenderer.h"
#include"../Shader/Shader/ShaderManager.h"
#include"../ResourceManager/Model/ModelManager.h"
#include"CubeMap\CubeMapManager.h"

GraphicsManager::~GraphicsManager()
{

	MACRO::SAFE_RELEASE(pRenderTargetView);
	MACRO::SAFE_RELEASE(pDepthStencilView);

	MACRO::SAFE_RELEASE(pBlendState);
	MACRO::SAFE_RELEASE(pDepthStencilState);
	MACRO::SAFE_RELEASE(pRasterizerState);

	MACRO::SAFE_RELEASE(pRenderTexture);
	MACRO::SAFE_RELEASE(pResolveTexture);
	MACRO::SAFE_RELEASE(pRenderSRV);
	MACRO::SAFE_RELEASE(pRenderTargetViewMSAA);
	MACRO::SAFE_RELEASE(pDepthStencilViewMSAA);
}


//初期化
//DX11のいつものやつ
HRESULT GraphicsManager::Initialize(ID3D11Device* setDevice, ID3D11DeviceContext* setDeviceContext, IDXGISwapChain* swapChain, UINT width, UINT height)
{

	pDevice = setDevice;
	pDeviceContext = setDeviceContext;
	pSwapChain = swapChain;

	//描画ターゲット作成
	if (FAILED(CreateRenderTarget(width,height)))
	{
		DEBUG::PrintfColor(EConsoleColor::H_RED, "描画ターゲットの作成に失敗\n");
		return E_FAIL;
	}
	DEBUG::PrintfColor(EConsoleColor::H_GREEN, "描画ターゲットの作成に成功\n");

	//深度バッファ作成
	if (FAILED(CreateDepthBuffer(width, height)))
	{
		DEBUG::PrintfColor(EConsoleColor::H_RED, "深度バッファの作成に失敗\n");
		return E_FAIL;
	}
	DEBUG::PrintfColor(EConsoleColor::H_GREEN, "深度バッファの作成に成功\n");

	//ラスタライザステート作成
	if (FAILED(CreateRasterizerState()))
	{
		DEBUG::PrintfColor(EConsoleColor::H_RED, "ラスタライザステートの作成に失敗\n");
		return E_FAIL;
	}
	DEBUG::PrintfColor(EConsoleColor::H_GREEN, "ラスタライザステートの作成に成功\n");
	pDeviceContext->RSSetState(pRasterizerState);

	//ブレンドステート作成
	if (FAILED(CreateBlendState()))
	{
		DEBUG::PrintfColor(EConsoleColor::H_RED, "ブレンドステートの作成に失敗\n");
		return E_FAIL;
	}
	DEBUG::PrintfColor(EConsoleColor::H_GREEN, "ブレンドステートの作成に成功\n");

	float blend[4] = { 0, 0, 0, 0 };
	pDeviceContext->OMSetBlendState(pBlendState, blend, 0xffffffff);

	//深度ステンシルステート作成
	if (FAILED(CreateDepthStencilState()))
	{
		DEBUG::PrintfColor(EConsoleColor::H_RED, "深度ステンシルステートの作成に失敗\n");
		return E_FAIL;
	}
	pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 0);
	DEBUG::PrintfColor(EConsoleColor::H_GREEN, "深度ステンシルステートの作成に成功\n");



	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, pDepthStencilView);

	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	pDeviceContext->RSSetViewports(1, &vp);

	pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	msaa.Initialize(pDevice, pDeviceContext);

	return S_OK;

}


HRESULT GraphicsManager::CreateDepthBuffer(UINT width, UINT height)
{
	D3D11_TEXTURE2D_DESC dc;
	dc.Width = width;
	dc.Height = height;
	dc.MipLevels = 1;
	dc.ArraySize = 1;
	dc.Format = DXGI_FORMAT_D32_FLOAT;
	dc.SampleDesc.Count = 1;
	dc.SampleDesc.Quality = 0;
	dc.Usage = D3D11_USAGE_DEFAULT;
	dc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dc.CPUAccessFlags = FALSE;
	dc.MiscFlags = FALSE;

	ID3D11Texture2D* pDepthStencil;
	if (FAILED(pDevice->CreateTexture2D(&dc, nullptr, &pDepthStencil)))
	{
		return E_FAIL;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Format = dc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsvDesc.Texture2D.MipSlice = 0;
	dsvDesc.Flags = 0;

	if (FAILED(pDevice->CreateDepthStencilView(pDepthStencil, &dsvDesc, &pDepthStencilView)))
	{
		MACRO::SAFE_RELEASE(pDepthStencil);
		return E_FAIL;
	}

	MACRO::SAFE_RELEASE(pDepthStencil);
	return S_OK;
}

HRESULT GraphicsManager::CreateRenderTarget(UINT width, UINT height)
{
	
	ID3D11Texture2D* backBuffer;

	if (FAILED(pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer)))
	{
		return E_FAIL;
	}

	if (FAILED(pDevice->CreateRenderTargetView(backBuffer, nullptr, &pRenderTargetView)))
	{
		return E_FAIL;
	}
	MACRO::SAFE_RELEASE(backBuffer);


	//MSAA用の設定

	DXGI_SAMPLE_DESC msaa;
	for (int i = 0; i < D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; ++i)
	{
		UINT quality;
		if (SUCCEEDED(pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_D24_UNORM_S8_UINT, i, &quality)))
		{
			if (0 < quality)
			{
				msaa.Count = i;
				msaa.Quality = quality - 1;
			}
		}

	}


	D3D11_TEXTURE2D_DESC desc;
	SecureZeroMemory(&desc, sizeof(desc));

	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.Width = width;
	desc.Height = height;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.SampleDesc = msaa;

	desc.MipLevels = 1;

	HRESULT hr = pDevice->CreateTexture2D(&desc, nullptr, &pRenderTexture);
	pDevice->CreateRenderTargetView(pRenderTexture, nullptr, &pRenderTargetViewMSAA);
	
	ID3D11Texture2D* pRenderTextureDepth = nullptr;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	pDevice->CreateTexture2D(&desc, nullptr, &pRenderTextureDepth);
	pDevice->CreateDepthStencilView(pRenderTextureDepth, nullptr, &pDepthStencilViewMSAA);
	MACRO::SAFE_RELEASE(pRenderTextureDepth);


	SecureZeroMemory(&desc, sizeof(desc));

	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Width = width;
	desc.Height = height;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.MipLevels = 1;

	pDevice->CreateTexture2D(&desc, nullptr, &pResolveTexture);
	pDevice->CreateShaderResourceView(pResolveTexture, nullptr, &pRenderSRV);

	return S_OK;
}

HRESULT GraphicsManager::CreateRasterizerState()
{
	D3D11_RASTERIZER_DESC RSDesc;
	RSDesc.FillMode = D3D11_FILL_SOLID;//普通に描画
	RSDesc.CullMode = D3D11_CULL_BACK;//裏面カリング
	RSDesc.FrontCounterClockwise = FALSE;//時計回り
	RSDesc.DepthBias = 0;//深度バイアス0
	RSDesc.DepthBiasClamp = 0;
	RSDesc.SlopeScaledDepthBias = 0;
	RSDesc.DepthClipEnable = TRUE;//深度クリッピング無し
	RSDesc.ScissorEnable = FALSE;//シザー矩形無し
	RSDesc.MultisampleEnable = FALSE;//マルチサンプリング無し
	RSDesc.AntialiasedLineEnable = FALSE;//ライン・アンチエイリアシング無し

	return pDevice->CreateRasterizerState(&RSDesc, &pRasterizerState);

}

HRESULT GraphicsManager::CreateBlendState()
{
	D3D11_BLEND_DESC BlendState;
	ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
	BlendState.AlphaToCoverageEnable = FALSE;
	BlendState.IndependentBlendEnable = FALSE;

	BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;



	return pDevice->CreateBlendState(&BlendState, &pBlendState);

}

HRESULT GraphicsManager::CreateDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC depthStencil;

	depthStencil.DepthEnable = TRUE;
	depthStencil.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencil.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencil.StencilEnable = FALSE;
	depthStencil.StencilReadMask = 0;
	depthStencil.StencilWriteMask = 0;

	depthStencil.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.FrontFace.StencilFunc = D3D11_COMPARISON_NEVER;

	depthStencil.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencil.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	return pDevice->CreateDepthStencilState(&depthStencil, &pDepthStencilState);

}


//レンダリング開始
void GraphicsManager::RenderStart()
{

	const float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	//描画ターゲット、深度ステンシルクリア
	pDeviceContext->ClearRenderTargetView(pRenderTargetViewMSAA, ClearColor);
	pDeviceContext->ClearDepthStencilView(pDepthStencilViewMSAA, D3D11_CLEAR_DEPTH, 1.0f, 0);

	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetViewMSAA, pDepthStencilViewMSAA);

	pDeviceContext->RSSetViewports(1, &vp);


}

void GraphicsManager::Draw2D()
{
	auto vertex = GetShaderManager().GetVertexShader(2);
	vertex->SetShader(pDeviceContext);
	return;	//使ってないから後回し
	for (auto list = pipeline.flatPipeline.Begin(); list != nullptr; list = list->next)
	{


	}
}

void GraphicsManager::Draw3D()
{
	//ボーン無し
	auto vertex = GetShaderManager().GetVertexShader(0);
	vertex->SetShader(pDeviceContext);
	for (auto list = pipeline.standardPipeline.Begin(); list != nullptr; list = list->next)
	{

		//ワールド行列更新
		GetCBMatrixManager().SetWorldMatrix(list->data.transform);

		//描画
		MeshRenderer::Render(*GetModelManager().GetModel(list->data.modelID),
			vertex->stride,
			vertex->offset,
			list->data.cubeMap);

	}

	//ボーンあり
	vertex = GetShaderManager().GetVertexShader(1);
	vertex->SetShader(pDeviceContext);
	
	for (auto list = pipeline.skinnedPipeline.Begin(); list != nullptr; list = list->next)
	{
		//ワールド行列更新
		GetCBMatrixManager().SetWorldMatrix(list->data.transform);

		//アニメーション行列更新
		list->data.bone->UpdateBone();
		GetBoneManager().SetAnimation(*list->data.bone);

		//描画
		MeshRenderer::Render(*GetModelManager().GetModel(list->data.modelID),
			vertex->stride,
			vertex->offset,
			list->data.cubeMap);

	}
}

//キューブマップに描画
//もっとスマートにしたかった
void GraphicsManager::DrawCubeMap()
{

	auto geometry = GetShaderManager().GetGeometryShader(2);
	geometry->SetShader(pDeviceContext);

	//キューブマップの座標更新（ごり押し）
	for (auto list = pipeline.standardPipeline.Begin(); list != nullptr; list = list->next)
	{
		if (list->data.cubeMap != -1)
		{

			D3DXVECTOR3 pos(list->data.transform.m[3]);
		
			GetCubeMapManager().UpdatePos(list->data.cubeMap, pos);
		}
	}
	for (auto list = pipeline.skinnedPipeline.Begin(); list != nullptr; list = list->next)
	{
		if (list->data.cubeMap != -1)
		{
			D3DXVECTOR3 pos(list->data.transform.m[3]);
			if (list->data.bone)
			{

				pos += list->data.bone->bone[0].absolute.pos;
				if (pos.y <= 0.2f)
					pos.y = 0.2f;
				
			}


			GetCubeMapManager().UpdatePos(list->data.cubeMap, pos);
		}
	}

	for (auto cubeList = GetCubeMapManager().GetRenderList().Begin(); cubeList != nullptr; cubeList = cubeList->next)
	{
		//描画開始
		cubeList->obj.RenderStart(pDeviceContext);

		//ボーン無し
		auto vertex = GetShaderManager().GetVertexShader(0);
		vertex->SetShader(pDeviceContext);
		for (auto list = pipeline.standardPipeline.Begin(); list != nullptr; list = list->next)
		{
			//キューブマップを貼る対象の場合は無視
			if (list->data.cubeMap == cubeList->thisNumber)
				continue;

			//ワールド行列更新
			GetCBMatrixManager().SetWorldMatrix(list->data.transform);

			//描画
			MeshRenderer::Render(*GetModelManager().GetModel(list->data.modelID),
				vertex->stride,
				vertex->offset,
				list->data.cubeMap);

		}

		//ボーンあり
		vertex = GetShaderManager().GetVertexShader(1);
		vertex->SetShader(pDeviceContext);

		for (auto list = pipeline.skinnedPipeline.Begin(); list != nullptr; list = list->next)
		{
			//キューブマップを貼る対象の場合は無視
			if (list->data.cubeMap == cubeList->thisNumber)
				continue;

			//ワールド行列更新
			GetCBMatrixManager().SetWorldMatrix(list->data.transform);

			//アニメーション行列更新
			list->data.bone->UpdateBone();
			GetBoneManager().SetAnimation(*list->data.bone);

			//描画
			MeshRenderer::Render(*GetModelManager().GetModel(list->data.modelID),
				vertex->stride,
				vertex->offset,
				list->data.cubeMap);

		}
		//描画終了
		cubeList->obj.RenderEnd(pDeviceContext);
	}



}

void GraphicsManager::SetShadingMode(ShadingMode mode)
{
	auto geometry = GetShaderManager().GetGeometryShader(0);
	geometry->LinkageClassInstance(0, (int)mode);
	geometry->SetShader(pDeviceContext);
}

//レンダリング終了
void GraphicsManager::RenderEnd()
{

	pDeviceContext->ResolveSubresource(pResolveTexture, 0, pRenderTexture, 0, DXGI_FORMAT_R8G8B8A8_UNORM);


	const float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const float msaaSlot = 5;


	ID3D11ShaderResourceView* dummy[] = { NULL };
	pDeviceContext->PSSetShaderResources(msaaSlot, 1, dummy);


	pDeviceContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//深度バッファのみクリア

	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetView, nullptr);


	pDeviceContext->PSSetShaderResources(msaaSlot, 1, &pRenderSRV);

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	GetCBMatrixManager().SetTextureMatrix(world);

	world._11 = 1080 / 2;
	world._22 = 720 / 2;
	
	GetCBMatrixManager().SetWorldMatrix(world);

	msaa.Draw();

	pSwapChain->Present(0, 0);

}

