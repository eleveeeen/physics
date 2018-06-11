#pragma once
//モデルのローダー
#include<string>
#include<D3DX11.h>

class ModelBase;
namespace ModelLoader
{
	//モデル読み込み
	//失敗時はnullptrを返す
	ModelBase* Load(std::string path, ID3D11Device* pDev);
	
}