//ゲーム内部のライト関連のクラス

#ifndef _GameLight_h_
#define _GameLight_h_

#include"../Shader/ConstantBuffer/ConstantBufferManager.h"

//色情報をまとめた構造体
struct LightColorObject
{
	D3DXVECTOR4 diffuse;	//拡散光
	D3DXVECTOR3 ambient;	//環境光
	LightColorObject()
	{
		diffuse.x = 1.0f;
		diffuse.y = 1.0f;
		diffuse.z = 1.0f;
		diffuse.w = 1.0f;

		ambient.x = 1.0f;
		ambient.y = 1.0f;
		ambient.z = 1.0f;
	}

};

//ライトの情報をまとめた構造体
struct LightObject
{
	LightColorObject color;	//色情報
	D3DXVECTOR3 direction;	//ライトの方向ベクトル

	LightObject()
	{
		direction.x = 0.0f;
		direction.y = 0.0f;
		direction.z = 1.0f;
	}

};

//ライト管理クラス　シングルトン
class GameLight
{
private://メンバ変数

public:

	static GameLight& GetInstance()
	{
		static GameLight singleton;
		return singleton;
	}

	//ライトの更新	
	//これを通さないとライトの情報は更新されない
	void UpdateLight()
	{
		GetCBLightManager().SetLight();
	}


	void SetLightDirection(const D3DXVECTOR3& pDir)
	{
		GetCBLightManager().UpdateLightDirection(pDir);
	}


	//全ての色情報をセット
	void SetLightColor(const D3DXVECTOR4& pDiffuse, const D3DXVECTOR3& pAmbient)
	{
		GetCBLightManager().UpdateLightColor(pDiffuse, pAmbient);
	}

	void SetLightColor(const D3DXVECTOR3& pDiffuse, const D3DXVECTOR3& pAmbient)
	{
		GetCBLightManager().UpdateLightColor(pDiffuse, pAmbient);
	}


	void SetLightColor(const LightColorObject& pColor)
	{
		GetCBLightManager().UpdateLightColor(pColor.diffuse, pColor.ambient);
	}

	//全ての情報をセット
	void SetLight(const LightObject& pLight)
	{
		SetLightDirection(pLight.direction);

		SetLightColor(pLight.color);
	}



private:


};

inline GameLight& GetLightMamager()
{
	return GameLight::GetInstance();
}

#endif