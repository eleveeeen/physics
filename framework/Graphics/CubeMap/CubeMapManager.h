#pragma once 
#include"../../ResourceManager/ResourceManager.h"
#include"CubeMapObject.h"

//キューブマップマネージャ

class CubeMapManager
{
private://定数定義
	static const int CubeMapMax = 100;

private://メンバ変数
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;

	ResourceManager<CubeMapObject>* cubeMap;
	int numCubeMap;

	int idPool[CubeMapMax];//解放された環境マップのid格納用

	int numPool;//解放されたidの数

public:
	static CubeMapManager& GetInstance()
	{
		static CubeMapManager singleton;
		return singleton;
	}
	//初期化
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context);

	//キューブマップ生成
	//成功時idを返して失敗時-1を返す
	int Create(const D3DXVECTOR3& scale = D3DXVECTOR3(1, 1, 1));

	//キューブマップを破棄
	void Destroy(int id);

	//キューブマップをシェーダにセット
	void SetTexture(int id)
	{
		cubeMap->pArray[id]->obj.SetTexture(pDeviceContext);
	}

	//座標更新
	void UpdatePos(int id, const D3DXVECTOR3& pos = D3DXVECTOR3(0, 0, 0))
	{
		cubeMap->pArray[id]->obj.UpdatePosition(pos);
	}

	ListManager<CubeMapObject>& GetRenderList()
	{
		return cubeMap->list;
	}

	~CubeMapManager()
	{

		MACRO::SAFE_DELETE(cubeMap);
	}

private:

	CubeMapManager()
	{
		pDevice = nullptr;
		pDeviceContext = nullptr;
		SecureZeroMemory(idPool, sizeof(int)*CubeMapMax);
		numCubeMap = 0;
		numPool = 0;

		cubeMap = new ResourceManager<CubeMapObject>(CubeMapMax);

	}


};

inline CubeMapManager& GetCubeMapManager()
{
	return CubeMapManager::GetInstance();
}