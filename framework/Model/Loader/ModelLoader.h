#pragma once
//���f���̃��[�_�[
#include<string>
#include<D3DX11.h>

class ModelBase;
namespace ModelLoader
{
	//���f���ǂݍ���
	//���s����nullptr��Ԃ�
	ModelBase* Load(std::string path, ID3D11Device* pDev);
	
}