//�J�����֘A�̃N���X

#ifndef _GameCamera_h_
#define _GameCamera_h_


#include"../Shader/ConstantBuffer/ConstantBufferManager.h"

//�J�����Ǘ��N���X�@�V���O���g��
class GameCamera
{
private:
	D3DXMATRIX viewMatrix;
	D3DXVECTOR3 cameraPos;
	D3DXVECTOR3 cameraVec;
public:

	static GameCamera* GetInstance()
	{
		static GameCamera p;
		return &p;
	}

	//�r���[�s��Z�b�g
	void SetCamera(const D3DXMATRIX& pMtx)
	{
		//�J�����̍��W�L��
		cameraPos.x = pMtx._41;
		cameraPos.y = pMtx._42;
		cameraPos.z = pMtx._43;
		//�J�����s����r���[�s��ɕϊ�
		D3DXMatrixInverse(&viewMatrix, NULL, &pMtx);

		//�r���[�s�񂩂�J�����̕����x�N�g�����o
		cameraVec.x = viewMatrix._13;
		cameraVec.y = viewMatrix._23;
		cameraVec.z = viewMatrix._33;
		//�O�̂��߃J�����̕����x�N�g�����K��
		D3DXVec3Normalize(&cameraVec, &cameraVec);

		//�萔�o�b�t�@�ɃZ�b�g
		GetCBCameraManager().SetCameraPos(cameraPos);
		GetCBMatrixManager().SetViewMatrix(viewMatrix);

	}

	D3DXMATRIX* GetViewMatrix()
	{
		return &viewMatrix;
	}

	D3DXVECTOR3* GetCameraPos()
	{
		return &cameraPos;
	}

	D3DXVECTOR3* GetCameraVec()
	{
		return &cameraVec;
	}

private:

};

inline GameCamera* GetCameraMgr()
{
	return GameCamera::GetInstance();
}




#endif