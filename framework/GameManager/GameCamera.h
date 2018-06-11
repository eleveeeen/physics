//カメラ関連のクラス

#ifndef _GameCamera_h_
#define _GameCamera_h_


#include"../Shader/ConstantBuffer/ConstantBufferManager.h"

//カメラ管理クラス　シングルトン
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

	//ビュー行列セット
	void SetCamera(const D3DXMATRIX& pMtx)
	{
		//カメラの座標記憶
		cameraPos.x = pMtx._41;
		cameraPos.y = pMtx._42;
		cameraPos.z = pMtx._43;
		//カメラ行列をビュー行列に変換
		D3DXMatrixInverse(&viewMatrix, NULL, &pMtx);

		//ビュー行列からカメラの方向ベクトル抽出
		cameraVec.x = viewMatrix._13;
		cameraVec.y = viewMatrix._23;
		cameraVec.z = viewMatrix._33;
		//念のためカメラの方向ベクトル正規化
		D3DXVec3Normalize(&cameraVec, &cameraVec);

		//定数バッファにセット
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