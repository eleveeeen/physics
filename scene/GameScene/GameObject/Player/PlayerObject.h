#pragma once

#include"../../../../framework/BaseObject/BaseObject.h"

#include"PlayerCamera.h"

//プレイヤークラス
//剛体と紐付&FPS視点でそれっぽさを表現
class PlayerObject :public BaseObject
{

private:

	PlayerCamera camera;//カメラ

	D3DXVECTOR3 pick;	//掴んだ座標

	D3DXVECTOR3 oldPos; //前のフレームの座標

	int rigidID;			//自身と紐付けた剛体のID
	float cameraOffset;		//カメラの高さのオフセット

	float metallic;
	float roughness;

public:
	PlayerObject();
	~PlayerObject();

	void Update();
	void Initialize();
	

private:

	void UpdateMaterial();	//マテリアルのパラメータ更新
	void UpdateLight();		//ライト更新
	void UpdateCamera();	//カメラ更新
	void UpdatePick();		//掴む処理更新

};
