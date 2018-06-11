#include"CubeMapObject.h"
#include"../../DebugConsole/DebugConsole.h"
#include"../../Shader/ConstantBuffer/ConstantBufferManager.h"
#include"../../GameManager/GameCamera.h"

bool CubeMapObject::Create(ID3D11Device* pDevice, const D3DXVECTOR3& pos,const D3DXVECTOR3& scale)
{

	const float size = 512;

	const float mipLevel = 8;

	//�L���[�u�e�N�X�`���̍쐬
	D3D11_TEXTURE2D_DESC desc;
	desc.Width = size;
	desc.Height = size;
	desc.MipLevels = mipLevel;
	desc.ArraySize = 6;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;//�e�N�X�`���t�H�[�}�b�g
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET |	//�`��^�[�Q�b�g
		D3D11_BIND_SHADER_RESOURCE;				//�V�F�[�_�[���\�[�X

	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS |//�~�b�v�̎�������
		D3D11_RESOURCE_MISC_TEXTURECUBE;				//�L���[�u�e�N�X�`��



	HRESULT hr = pDevice->CreateTexture2D(&desc, nullptr, &pCubeTexture);

	if (FAILED(hr))
	{

		DEBUG::PrintfColor(EConsoleColor::H_RED, "Cube:�e�N�X�`���̍쐬�Ɏ��s\n");

		return false;

	}


	//6�ʂ̃����_�[�^�[�Q�b�g�쐬
	D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
	rtDesc.Format = desc.Format;//�e�N�X�`���Ɠ����ݒ�
	rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;//2D�e�N�X�`���z��
	rtDesc.Texture2DArray.FirstArraySlice = 0;
	rtDesc.Texture2DArray.ArraySize = 6;
	rtDesc.Texture2DArray.MipSlice = 0;

	hr = pDevice->CreateRenderTargetView(pCubeTexture, &rtDesc, &pRTV);
	if (FAILED(hr))
	{
		DEBUG::PrintfColor(EConsoleColor::H_RED, "Cube:�����_�[�^�[�Q�b�g�̍쐬�Ɏ��s\n");
		return false;
	}

	//�V�F�[�_�Ŋ��}�b�v�ɃA�N�Z�X���邽�߂̃V�F�[�_�[���\�[�X�쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
	ZeroMemory(&srDesc, sizeof(srDesc));

	srDesc.Format = desc.Format;
	srDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURECUBE;//�L���[�u�e�N�X�`��
	srDesc.TextureCube.MipLevels = mipLevel;
	srDesc.TextureCube.MostDetailedMip = 0;

	hr = pDevice->CreateShaderResourceView(pCubeTexture, &srDesc, &pSRV);

	if (FAILED(hr))
	{

		DEBUG::PrintfColor(EConsoleColor::H_RED, "Cube:�V�F�[�_�[���\�[�X�̍쐬�Ɏ��s\n");
		return false;
	}

	//�[�x�A�X�e���V���e�N�X�`���쐬


	desc.Width = size;
	desc.Height = size;
	desc.MipLevels = 1;
	desc.ArraySize = 6;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Format = DXGI_FORMAT_D32_FLOAT;//�e�N�X�`���t�H�[�}�b�g
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;//�[�x/�X�e���V��
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;//�L���[�u�e�N�X�`��

	hr = pDevice->CreateTexture2D(&desc, nullptr, &pDepthTexture);

	if (FAILED(hr))
	{
		DEBUG::PrintfColor(EConsoleColor::H_RED, "Cube:�[�x�A�X�e���V���e�N�X�`���̍쐬�Ɏ��s\n");
		return false;
	}


	D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
	dsDesc.Format = desc.Format;
	dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	dsDesc.Flags = 0;
	dsDesc.Texture2DArray.FirstArraySlice = 0;
	dsDesc.Texture2DArray.ArraySize = 6;
	dsDesc.Texture2DArray.MipSlice = 0;

	hr = pDevice->CreateDepthStencilView(pDepthTexture, &dsDesc, &pDSV);
	if (FAILED(hr))
	{
		DEBUG::PrintfColor(EConsoleColor::H_RED, "Cube:�[�x�A�X�e���V���r���[�̍쐬�Ɏ��s\n");
		return false;
	}

	//�r���[�|�[�g�̐ݒ�
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = desc.Width;
	vp.Height = desc.Height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	cubeCenter = pos;
	cubeScale = scale;

	return true;
}

//�L���[�u�}�b�v�ɏ������݊J�n
void CubeMapObject::RenderStart(ID3D11DeviceContext* pContext)
{

	D3DXMATRIX proj, view[6];
	//�ˉe�쐬
	D3DXMatrixPerspectiveFovLH(&proj, D3DXToRadian(90), 1.0f, 0.1f, 500.0f);


	//�r���[�s��쐬
	D3DXVECTOR3 look;
	D3DXVECTOR3 up;



	//+X
	look = cubeCenter + D3DXVECTOR3(1, 0, 0);
	up = D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&view[0], &cubeCenter, &look, &up);

	//-X
	
	look = cubeCenter + D3DXVECTOR3(-1, 0, 0);
	up = D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&view[1], &cubeCenter, &look, &up);

	//+Y

	look = cubeCenter + D3DXVECTOR3(0, 1, 0);
	up = D3DXVECTOR3(0, 0, -1);
	D3DXMatrixLookAtLH(&view[2], &cubeCenter, &look, &up);

	//-Y

	look = cubeCenter + D3DXVECTOR3(0, -1, 0);
	up = D3DXVECTOR3(0, 0, 1);
	D3DXMatrixLookAtLH(&view[3], &cubeCenter, &look, &up);

	//+Z

	look = cubeCenter + D3DXVECTOR3(0, 0, 1);
	up = D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&view[4], &cubeCenter, &look, &up);

	//-Z

	look = cubeCenter + D3DXVECTOR3(0, 0, -1);
	up = D3DXVECTOR3(0, 1, 0);
	D3DXMatrixLookAtLH(&view[5], &cubeCenter, &look, &up);

	D3DXMATRIX wv = GetCBMatrixManager().GetViewMatrix();


	//�V�F�[�_�ɍs����Z�b�g
	GetCBMatrixManager().SetCubeMapMatrix(proj, view, cubeCenter);


	//�`��^�[�Q�b�g�N���A
	float ClearColor[4] = { 0, 0, 1, 1 };
	pContext->ClearRenderTargetView(pRTV, ClearColor);
	pContext->ClearDepthStencilView(pDSV, D3D11_CLEAR_DEPTH, 1, 0);

	//�r���[�|�[�g�Z�b�g
	pContext->RSSetViewports(1, &vp);
	//�`��^�[�Q�b�g�Z�b�g
	pContext->OMSetRenderTargets(1, &pRTV, pDSV);


}

//�������ݏI�����ă~�b�v�}�b�v����
void CubeMapObject::RenderEnd(ID3D11DeviceContext* pContext)
{
	//�`��^�[�Q�b�g�N���A
	pContext->OMSetRenderTargets(0, nullptr, nullptr);

	//�~�b�v�}�b�v�쐬
	pContext->GenerateMips(pSRV);
}


void CubeMapObject::SetTexture(ID3D11DeviceContext* pContext)
{
	D3DXMATRIX cubeMat;
	D3DXMatrixIdentity(&cubeMat);
	cubeMat._11 = cubeScale.x;
	cubeMat._22 = cubeScale.y;
	cubeMat._33 = cubeScale.z;
	cubeMat._41 = cubeCenter.x;
	cubeMat._42 = cubeCenter.y;
	cubeMat._43 = cubeCenter.z;

	D3DXMatrixInverse(&cubeMat, nullptr, &cubeMat);

	GetCBMatrixManager().SetCubeLocalMatrix(cubeMat, cubeCenter);

	pContext->PSSetShaderResources(4, 1, &pSRV);
}