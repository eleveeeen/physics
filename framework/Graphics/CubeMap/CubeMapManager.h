#pragma once 
#include"../../ResourceManager/ResourceManager.h"
#include"CubeMapObject.h"

//�L���[�u�}�b�v�}�l�[�W��

class CubeMapManager
{
private://�萔��`
	static const int CubeMapMax = 100;

private://�����o�ϐ�
	ID3D11Device* pDevice;
	ID3D11DeviceContext* pDeviceContext;

	ResourceManager<CubeMapObject>* cubeMap;
	int numCubeMap;

	int idPool[CubeMapMax];//������ꂽ���}�b�v��id�i�[�p

	int numPool;//������ꂽid�̐�

public:
	static CubeMapManager& GetInstance()
	{
		static CubeMapManager singleton;
		return singleton;
	}
	//������
	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* context);

	//�L���[�u�}�b�v����
	//������id��Ԃ��Ď��s��-1��Ԃ�
	int Create(const D3DXVECTOR3& scale = D3DXVECTOR3(1, 1, 1));

	//�L���[�u�}�b�v��j��
	void Destroy(int id);

	//�L���[�u�}�b�v���V�F�[�_�ɃZ�b�g
	void SetTexture(int id)
	{
		cubeMap->pArray[id]->obj.SetTexture(pDeviceContext);
	}

	//���W�X�V
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