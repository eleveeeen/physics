#include"CubeMapManager.h"


//初期化
bool CubeMapManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{

	pDevice = device;
	pDeviceContext = context;


	return true;
}



//キューブマップ生成
//成功時idを返して失敗時-1を返す
int CubeMapManager::Create(const D3DXVECTOR3& scale)
{

	//プールにIDがあればプールからIDを取り出す
	int id = (numPool == 0) ?
		numCubeMap++ :		//id = numCubeMap
		idPool[--numPool];	//id = idPool[numPool-1]
	
	cubeMap->pArray[id] = new TemplateList<CubeMapObject>;

	if (!cubeMap->pArray[id]->obj.Create(pDevice, D3DXVECTOR3(0, 0, 0),scale))
	{
		MACRO::SAFE_DELETE(cubeMap->pArray[id]);
		return -1;
	};

	cubeMap->pArray[id]->thisNumber = id;
	cubeMap->list.Push_Back(cubeMap->pArray[id]);

	return id;


}


//キューブマップを破棄
void CubeMapManager::Destroy(int id)
{
	if (cubeMap->pArray[id] == nullptr)
		return;

	
	//キューブマップ解放
	cubeMap->pArray[id]->obj.Destroy();

	//削除してリストから除外
	cubeMap->list.Erase(cubeMap->pArray[id]);

	cubeMap->pArray[id] = nullptr;

	//IDをプールに入れる
	idPool[numPool++] = id;
	
}