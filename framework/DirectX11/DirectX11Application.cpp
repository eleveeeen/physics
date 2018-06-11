#include"DirectX11Application.h"
#include"../Utility/Macro.h"
#include"../DebugConsole/DebugConsole.h"
#include"../Shader/Shader/ShaderManager.h"
#include"../Graphics/GraphicsManager.h"
#include"../Shader/ConstantBuffer/ConstantBufferManager.h"
#include"../ResourceManager/Texture/TextureManager.h"
#include"../ResourceManager/Model/ModelManager.h"
#include"../ResourceManager/Material/MaterialManager.h"
#include"../Renderer/MeshRenderer.h"
#include"../Graphics/CubeMap/CubeMapManager.h"

DirectX11Application::~DirectX11Application()
{

	MACRO::SAFE_RELEASE(pDevice);
	MACRO::SAFE_RELEASE(pDeviceContext);

	MACRO::SAFE_RELEASE(pSwapChain);

	MACRO::SAFE_RELEASE(pDXGI);

	MACRO::SAFE_RELEASE(pFactory);
	MACRO::SAFE_RELEASE(pAdapter);



}


HRESULT DirectX11Application::Initialize(HWND hwnd, UINT x, UINT y)
{
	hWnd = hwnd;
	windowHeight = y;
	windowWidth = x;



	if (FAILED(CreateDevice()))
	{
		DEBUG::PrintfColor(EConsoleColor::H_RED, "デバイスの作成失敗\n");
		return E_FAIL;
	}


	if (FAILED(CreateSwapChain()))
	{
		DEBUG::PrintfColor(EConsoleColor::H_RED, "スワップチェインの作成失敗\n");
		return E_FAIL;
	}



	//シェーダ初期化	
	GetShaderManager().Initialize(pDevice, pDeviceContext);

	//定数バッファ初期化
	GetCBManager().Initialize(pDevice, pDeviceContext);

	//テクスチャ初期化
	GetTextureManager().Initialize(pDevice, pDeviceContext);

	//マテリアル初期化
	GetMaterialManager().Initialize();

	//モデル初期化
	GetModelManager().Initialize(pDevice, pDeviceContext);

	//レンダラ初期化
	MeshRenderer::Initialize(pDeviceContext);

	//キューブマップ初期化
	GetCubeMapManager().Initialize(pDevice, pDeviceContext);

	DEBUG::PrintfColor(EConsoleColor::H_YELLOW, "DirectX11初期化終了\n");

	return S_OK;



}





HRESULT DirectX11Application::CreateDevice()
{
	//11の使える環境のみ対応
	D3D_FEATURE_LEVEL featureLever[] = { D3D_FEATURE_LEVEL_11_0 };

	//デバイス作成
	HRESULT hr = D3D11CreateDevice(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_SINGLETHREADED,
		featureLever,
		1,
		D3D11_SDK_VERSION,
		&pDevice,
		&useFeatureLevel,
		&pDeviceContext);

	if (FAILED(hr))
	{
		return E_FAIL;
	}

	//インターフェース取得
	if (FAILED(pDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&pDXGI)))
	{
		return E_FAIL;
	}

	//アダプター取得
	if (FAILED(pDXGI->GetAdapter(&pAdapter)))
	{
		return E_FAIL;
	}

	//ファクトリー取得

	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (pFactory == nullptr)
	{
		return E_FAIL;
	}

	if (FAILED(pFactory->MakeWindowAssociation(hWnd, 0)))
	{
		return E_FAIL;
	}

	
	
	return S_OK;

}


HRESULT DirectX11Application::CreateSwapChain()
{

	//MSAA
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


	DXGI_SWAP_CHAIN_DESC dc;
	ZeroMemory(&dc, sizeof(dc));


	dc.BufferDesc.Width = windowWidth;
	dc.BufferDesc.Height = windowHeight;
	dc.BufferDesc.RefreshRate.Numerator = 0;
	dc.BufferDesc.RefreshRate.Denominator = 1;

	dc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	dc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	dc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	dc.SampleDesc.Count = 1;
	dc.SampleDesc.Quality = 0;
	dc.SampleDesc = msaa;
	dc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
	dc.BufferCount = 1;
	dc.OutputWindow = hWnd;
	dc.Windowed = TRUE;//とりあえずウィンドウで初期化
	dc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	dc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	if (FAILED(pFactory->CreateSwapChain(pDevice, &dc, &pSwapChain)))
	{
		return E_FAIL;

	}



	return S_OK;

}




//終了処理
void DirectX11Application::Finalize()
{



}