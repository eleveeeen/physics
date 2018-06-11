#pragma once
//���b�V���̒��_�f�[�^
#include<D3DX11.h>

#include"../../../Utility/Macro.h"
#include"../../../DebugConsole/DebugConsole.h"
struct MeshVertex
{
	ID3D11Buffer* vertexBuffer[15];
	int numBuffers;//���_�o�b�t�@�̐�

	MeshVertex()
	{
		SecureZeroMemory(vertexBuffer, sizeof(ID3D11Buffer*) * 15);
		numBuffers = 0;
	}

	~MeshVertex()
	{
		MACRO::SAFE_RELEASE_ARRAY(vertexBuffer, numBuffers);
	}

	//���_�f�[�^�A�o�C�g���A���_����n���ăo�b�t�@�쐬
	//���s������-1��Ԃ�
	int CreateBuffer(ID3D11Device* dev, const void* vertices, unsigned int stride, unsigned int numVertices)
	{
		if (numBuffers > 15)
			return -1;//error
		

		D3D11_BUFFER_DESC dc;
		dc.ByteWidth = stride * numVertices;
		dc.Usage = D3D11_USAGE_DEFAULT;
		dc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		dc.CPUAccessFlags = 0;
		dc.MiscFlags = 0;
		dc.StructureByteStride = 0;

		D3D11_SUBRESOURCE_DATA sub;
		sub.pSysMem = vertices;
		sub.SysMemPitch = 0;
		sub.SysMemSlicePitch = 0;

#ifdef _DEBUG

		HRESULT hr;
		hr = dev->CreateBuffer(&dc, &sub, &vertexBuffer[numBuffers]);
		if (FAILED(hr))
		{
			DEBUG::PrintfColor(EConsoleColor::H_RED, "���_�o�b�t�@�̍쐬�Ɏ��s\n");
			DEBUG::PrintfColor(EConsoleColor::H_RED, "Slot:%d\nStride:%d\nVertices:%d\n", numBuffers, stride, numVertices);

			return -1;
		}


#else	//�����[�X���͎��s���Ȃ��̂��o�Ȃ��O���
		dev->CreateBuffer(&dc, &sub, &vertexBuffer[numBuffers]);
#endif


		//�ꉞ���Ԗڂɓ���������Ԃ�
		return numBuffers++;
	}

};