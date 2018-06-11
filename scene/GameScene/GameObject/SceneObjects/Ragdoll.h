#pragma once

#include"../../../../framework/BaseObject/BaseObject.h"

//ラグドールのクラス
//パラメータはゆーきたんクラスの使いまわし
class Ragdoll : public BaseObject
{
public:

	Ragdoll();
	~Ragdoll();

	void Update() {};
	//初期化
	void Initialize(const D3DXVECTOR3& initPos = D3DXVECTOR3(0, 0, 0),		//初期の座標
		const D3DXQUATERNION& initRot = D3DXQUATERNION(0, 0, 0, 1));		//初期の姿勢

private:
	void InitRagdoll(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot);

};