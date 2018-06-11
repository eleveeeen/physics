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


//������
//DX11�̂����̂��
HRESULT GraphicsManager::Initialize(ID3D11Device* setDevice, ID3D11DeviceContext* setDeviceContext, IDXGISwapChain* swapChain, UINT width, UINT height)
{

	pDevice = setDevice;
	pDeviceContext = setDeviceContext;
	pSwapChain = swapChain;

	//�`��^�[�Q�b�g�쐬
	if (FAILED(CreateRenderTarget(width,height)))
	{
		DEBUG::PrintfColor(EConsoleColor::H_RED, "�`��^�[�Q�b�g�̍쐬�Ɏ��s\n");
		return E_FAIL;
	}
	DEBUG::PrintfColor(EConsoleColor::H_GREEN, "�`��^�[�Q�b�g�̍쐬�ɐ���\n");

	//�[�x�o�b�t�@�쐬
	if (FAILED(CreateDepthBuffer(width, height)))
	{
		DEBUG::PrintfColor(EConsoleColor::H_RED, "�[�x�o�b�t�@�̍쐬�Ɏ��s\n");
		return E_FAIL;
	}
	DEBUG::PrintfColor(EConsoleColor::H_GREEN, "�[�x�o�b�t�@�̍쐬�ɐ���\n");

	//���X�^���C�U�X�e�[�g�쐬
	if (FAILED(CreateRasterizerState()))
	{
		DEBUG::PrintfColor(EConsoleColor::H_RED, "���X�^���C�U�X�e�[�g�̍쐬�Ɏ��s\n");
		return E_FAIL;
	}
	DEBUG::PrintfColor(EConsoleColor::H_GREEN, "���X�^���C�U�X�e�[�g�̍쐬�ɐ���\n");
	pDeviceContext->RSSetState(pRasterizerState);

	//�u�����h�X�e�[�g�쐬
	if (FAILED(CreateBlendState()))
	{
		DEBUG::PrintfColor(EConsoleColor::H_RED, "�u�����h�X�e�[�g�̍쐬�Ɏ��s\n");
		return E_FAIL;
	}
	DEBUG::PrintfColor(EConsoleColor::H_GREEN, "�u�����h�X�e�[�g�̍쐬�ɐ���\n");

	float blend[4] = { 0, 0, 0, 0 };
	pDeviceContext->OMSetBlendState(pBlendState, blend, 0xffffffff);

	//�[�x�X�e���V���X�e�[�g�쐬
	if (FAILED(CreateDepthStencilState()))
	{
		DEBUG::PrintfColor(EConsoleColor::H_RED, "�[�x�X�e���V���X�e�[�g�̍쐬�Ɏ��s\n");
		return E_FAIL;
	}
	pDeviceContext->OMSetDepthStencilState(pDepthStencilState, 0);
	DEBUG::PrintfColor(EConsoleColor::H_GREEN, "�[�x�X�e���V���X�e�[�g�̍쐬�ɐ���\n");



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


	//MSAA�p�̐ݒ�

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
	RSDesc.FillMode = D3D11_FILL_SOLID;//���ʂɕ`��
	RSDesc.CullMode = D3D11_CULL_BACK;//���ʃJ�����O
	RSDesc.FrontCounterClockwise = FALSE;//���v���
	RSDesc.DepthBias = 0;//�[�x�o�C�A�X0
	RSDesc.DepthBiasClamp = 0;
	RSDesc.SlopeScaledDepthBias = 0;
	RSDesc.DepthClipEnable = TRUE;//�[�x�N���b�s���O����
	RSDesc.ScissorEnable = FALSE;//�V�U�[��`����
	RSDesc.MultisampleEnable = FALSE;//�}���`�T���v�����O����
	RSDesc.AntialiasedLineEnable = FALSE;//���C���E�A���`�G�C���A�V���O����

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


//�����_�����O�J�n
void GraphicsManager::RenderStart()
{

	const float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

	//�`��^�[�Q�b�g�A�[�x�X�e���V���N���A
	pDeviceContext->ClearRenderTargetView(pRenderTargetViewMSAA, ClearColor);
	pDeviceContext->ClearDepthStencilView(pDepthStencilViewMSAA, D3D11_CLEAR_DEPTH, 1.0f, 0);

	pDeviceContext->OMSetRenderTargets(1, &pRenderTargetViewMSAA, pDepthStencilViewMSAA);

	pDeviceContext->RSSetViewports(1, &vp);


}

void GraphicsManager::Draw2D()
{
	auto vertex = GetShaderManager().GetVertexShader(2);
	vertex->SetShader(pDeviceContext);
	return;	//�g���ĂȂ�������
	for (auto list = pipeline.flatPipeline.Begin(); list != nullptr; list = list->next)
	{


	}
}

void GraphicsManager::Draw3D()
{
	//�{�[������
	auto vertex = GetShaderManager().GetVertexShader(0);
	vertex->SetShader(pDeviceContext);
	for (auto list = pipeline.standardPipeline.Begin(); list != nullptr; list = list->next)
	{

		//���[���h�s��X�V
		GetCBMatrixManager().SetWorldMatrix(list->data.transform);

		//�`��
		MeshRenderer::Render(*GetModelManager().GetModel(list->data.modelID),
			vertex->stride,
			vertex->offset,
			list->data.cubeMap);

	}

	//�{�[������
	vertex = GetShaderManager().GetVertexShader(1);
	vertex->SetShader(pDeviceContext);
	
	for (auto list = pipeline.skinnedPipeline.Begin(); list != nullptr; list = list->next)
	{
		//���[���h�s��X�V
		GetCBMatrixManager().SetWorldMatrix(list->data.transform);

		//�A�j���[�V�����s��X�V
		list->data.bone->UpdateBone();
		GetBoneManager().SetAnimation(*list->data.bone);

		//�`��
		MeshRenderer::Render(*GetModelManager().GetModel(list->data.modelID),
			vertex->stride,
			vertex->offset,
			list->data.cubeMap);

	}
}

//�L���[�u�}�b�v�ɕ`��
//�����ƃX�}�[�g�ɂ���������
void GraphicsManager::DrawCubeMap()
{

	auto geometry = GetShaderManager().GetGeometryShader(2);
	geometry->SetShader(pDeviceContext);

	//�L���[�u�}�b�v�̍��W�X�V�i���艟���j
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
		//�`��J�n
		cubeList->obj.RenderStart(pDeviceContext);

		//�{�[������
		auto vertex = GetShaderManager().GetVertexShader(0);
		vertex->SetShader(pDeviceContext);
		for (auto list = pipeline.standardPipeline.Begin(); list != nullptr; list = list->next)
		{
			//�L���[�u�}�b�v��\��Ώۂ̏ꍇ�͖���
			if (list->data.cubeMap == cubeList->thisNumber)
				continue;

			//���[���h�s��X�V
			GetCBMatrixManager().SetWorldMatrix(list->data.transform);

			//�`��
			MeshRenderer::Render(*GetModelManager().GetModel(list->data.modelID),
				vertex->stride,
				vertex->offset,
				list->data.cubeMap);

		}

		//�{�[������
		vertex = GetShaderManager().GetVertexShader(1);
		vertex->SetShader(pDeviceContext);

		for (auto list = pipeline.skinnedPipeline.Begin(); list != nullptr; list = list->next)
		{
			//�L���[�u�}�b�v��\��Ώۂ̏ꍇ�͖���
			if (list->data.cubeMap == cubeList->thisNumber)
				continue;

			//���[���h�s��X�V
			GetCBMatrixManager().SetWorldMatrix(list->data.transform);

			//�A�j���[�V�����s��X�V
			list->data.bone->UpdateBone();
			GetBoneManager().SetAnimation(*list->data.bone);

			//�`��
			MeshRenderer::Render(*GetModelManager().GetModel(list->data.modelID),
				vertex->stride,
				vertex->offset,
				list->data.cubeMap);

		}
		//�`��I��
		cubeList->obj.RenderEnd(pDeviceContext);
	}



}

void GraphicsManager::SetShadingMode(ShadingMode mode)
{
	auto geometry = GetShaderManager().GetGeometryShader(0);
	geometry->LinkageClassInstance(0, (int)mode);
	geometry->SetShader(pDeviceContext);
}

//�����_�����O�I��
void GraphicsManager::RenderEnd()
{

	pDeviceContext->ResolveSubresource(pResolveTexture, 0, pRenderTexture, 0, DXGI_FORMAT_R8G8B8A8_UNORM);


	const float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const float msaaSlot = 5;


	ID3D11ShaderResourceView* dummy[] = { NULL };
	pDeviceContext->PSSetShaderResources(msaaSlot, 1, dummy);


	pDeviceContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
	pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);//�[�x�o�b�t�@�̂݃N���A

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

