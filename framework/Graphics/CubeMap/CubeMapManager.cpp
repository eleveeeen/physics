#include"CubeMapManager.h"


//������
bool CubeMapManager::Initialize(ID3D11Device* device, ID3D11DeviceContext* context)
{

	pDevice = device;
	pDeviceContext = context;


	return true;
}



//�L���[�u�}�b�v����
//������id��Ԃ��Ď��s��-1��Ԃ�
int CubeMapManager::Create(const D3DXVECTOR3& scale)
{

	//�v�[����ID������΃v�[������ID�����o��
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


//�L���[�u�}�b�v��j��
void CubeMapManager::Destroy(int id)
{
	if (cubeMap->pArray[id] == nullptr)
		return;

	
	//�L���[�u�}�b�v���
	cubeMap->pArray[id]->obj.Destroy();

	//�폜���ă��X�g���珜�O
	cubeMap->list.Erase(cubeMap->pArray[id]);

	cubeMap->pArray[id] = nullptr;

	//ID���v�[���ɓ����
	idPool[numPool++] = id;
	
}