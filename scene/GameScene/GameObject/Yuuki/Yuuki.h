#pragma once

#include"../../../../framework/BaseObject/BaseObject.h"

//シーン内に置くゆーきたんのクラス
//ラグドール
class Yuuki :public BaseObject
{
public:

	Yuuki();
	~Yuuki();
	
	//恐らく内部で行うのはDrawCallだけ
	void Update();

	//初期化
	void Initialize(const D3DXVECTOR3& initPos = D3DXVECTOR3(0, 0, 0),	   //初期の座標
		const D3DXQUATERNION& initRot = D3DXQUATERNION(0, 0, 0, 1),		   //初期の姿勢
		bool useCubemap = false);										   //キューブマップを使用するかどうか falseならスフィアマップを使用

private:
	void InitRagdoll();	//ラグドール用の情報初期化


};