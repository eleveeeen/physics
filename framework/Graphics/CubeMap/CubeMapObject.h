#pragma once
#include<D3DX11.h>
#include<d3dx9.h>
#include"../../Utility/Macro.h"
//�L���[�u�}�b�v�p�̃I�u�W�F�N�g
class CubeMapObject
{
private://�����o�ϐ�
	ID3D11Texture2D* pCubeTexture;
	ID3D11RenderTargetView* pRTV;
	ID3D11ShaderResourceView* pSRV;
	ID3D11Texture2D* pDepthTexture;
	ID3D11DepthStencilView* pDSV;

	D3D11_VIEWPORT vp;

	D3DXVECTOR3 cubeCenter;		//�L���[�u�}�b�v�̒��S
	D3DXVECTOR3 cubeScale;		//�L���[�u�}�b�v�̑傫��
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

	//���W�X�V
	void UpdatePosition(const D3DXVECTOR3& pos)
	{
		cubeCenter = pos;
	}

	//�L���[�u�}�b�v�ɏ������݊J�n
	void RenderStart(ID3D11DeviceContext* pContext);

	//�������ݏI�����ăe�N�X�`���ɓf���o��
	void RenderEnd(ID3D11DeviceContext* pContext);

	//�s�N�Z���V�F�[�_�ɃZ�b�g
	void SetTexture(ID3D11DeviceContext* pContext);

private:
};
