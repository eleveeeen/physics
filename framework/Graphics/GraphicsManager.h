//-------------------
//�`��Ǘ��N���X
//-------------------
#pragma once
#include"GraphicsPipeline.h"
#include"msaaProto.h"
class GraphicsManager
{
public://�萔��`

	enum ShadingMode
	{
		Flat,		//�t���b�g�V�F�[�f�B���O
		Gouraud,	//�O�[���[�V�F�[�f�B���O
	};


private://�����o�ϐ�
	
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;
	ID3D11SamplerState* pSamplerState;
	
	IDXGISwapChain* pSwapChain;
	ID3D11RenderTargetView* pRenderTargetView;
	ID3D11RenderTargetView* pRenderTargetViewMSAA;

	ID3D11Texture2D* pRenderTexture;
	ID3D11Texture2D* pResolveTexture;
	ID3D11ShaderResourceView* pRenderSRV;

	ID3D11DepthStencilView* pDepthStencilView;
	ID3D11DepthStencilView* pDepthStencilViewMSAA;

	ID3D11RasterizerState* pRasterizerState;
	ID3D11BlendState* pBlendState;
	ID3D11DepthStencilState* pDepthStencilState;

	D3D11_VIEWPORT vp;

	GraphicsPipeline pipeline;


public:

	static GraphicsManager& GetInstance()
	{
		static GraphicsManager singleton;
		return singleton;
	}

	GraphicsPipeline& GetPipeline()
	{
		return pipeline;
	}



	HRESULT Initialize(ID3D11Device* setDevice, ID3D11DeviceContext* setDeviceContext,
		IDXGISwapChain* swapChain, UINT width = 640, UINT height = 480);


	void RenderStart();
	void RenderEnd();

	void SetShadingMode(ShadingMode mode);

	void Draw3D();
	void Draw2D();

	//�L���[�u�}�b�v�Ƀ����_�����O
	void DrawCubeMap();

	//msaa�p�̕`��I�u�W�F�N�g
	//�f�B�t�@�[�h�������ɍ폜�\��
	MSAAObj msaa;

	//�~�b�v�}�b�v�̂��
	//RTV�؂�ւ��̂��


	~GraphicsManager();

private:

	GraphicsManager()
	{
		pDevice = nullptr;
		pDeviceContext = nullptr;
		pSamplerState = nullptr;

		pSwapChain = nullptr;
		pRenderTargetView = nullptr;
		pDepthStencilView = nullptr;

		pRasterizerState = nullptr;
		pBlendState = nullptr;
		pDepthStencilState = nullptr;

		pRenderSRV = nullptr;
		pRenderTexture = nullptr;
		pResolveTexture = nullptr;
		pRenderTargetViewMSAA = nullptr;
		pDepthStencilViewMSAA = nullptr;
	}

	HRESULT CreateDepthBuffer(UINT width, UINT height);
	HRESULT CreateRenderTarget(UINT width, UINT height);
	HRESULT CreateRasterizerState();
	HRESULT CreateBlendState();
	HRESULT CreateDepthStencilState();

};



inline GraphicsPipeline& GetGraphicsPipeline()
{
	return GraphicsManager::GetInstance().GetPipeline();
}