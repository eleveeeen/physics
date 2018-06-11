#ifndef _DirectX11Application_h_
#define _DirectX11Application_h_
//------------------------------------------
//DirectX11�̃f�o�C�X�Ƃ����Ǘ������N���X
//------------------------------------------

#include<D3DX11.h>

class DirectX11Application
{
private://�����o�ϐ�
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

	//������
	HRESULT Initialize(HWND hwnd, UINT x = 640, UINT y = 480);

	//�I��
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
	//�f�o�C�X�̍쐬
	HRESULT CreateDevice();
	//�X���b�v�`�F�C���쐬
	HRESULT CreateSwapChain();


};




#endif