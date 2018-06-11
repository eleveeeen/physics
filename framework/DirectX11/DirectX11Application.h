#ifndef _DirectX11Application_h_
#define _DirectX11Application_h_
//------------------------------------------
//DirectX11のデバイスとかを管理したクラス
//------------------------------------------

#include<D3DX11.h>

class DirectX11Application
{
private://メンバ変数
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;

	IDXGISwapChain* pSwapChain;

	IDXGIDevice1* pDXGI;
	IDXGIFactory* pFactory;
	IDXGIAdapter* pAdapter;


	UINT windowHeight;
	UINT windowWidth;
	HWND hWnd;
	D3D_FEATURE_LEVEL useFeatureLevel;



public:

	//初期化
	HRESULT Initialize(HWND hwnd, UINT x = 640, UINT y = 480);

	//終了
	void Finalize();

	~DirectX11Application();

	IDXGISwapChain* GetSwapChain()
	{
		return pSwapChain;
	}
	ID3D11Device* GetDevice()
	{
		return pDevice;
	}
	ID3D11DeviceContext* GetDeviceContext()
	{
		return pDeviceContext;
	}

private:
	//デバイスの作成
	HRESULT CreateDevice();
	//スワップチェイン作成
	HRESULT CreateSwapChain();


};




#endif