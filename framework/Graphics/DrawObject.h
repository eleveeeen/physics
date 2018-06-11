#pragma once
//描画用データの構造体
#include<d3dx9.h>
#include"../Model/Bone/BoneObjectEx.h"
#include"../Utility/GameMath.h"

struct DrawObject
{
	D3DXMATRIX transform;	//座標とか回転
	int modelID;			//モデル
	BoneObjectEx* bone;		//ボーン情報　なければnull
	int cubeMap;			//キューブマップのID　なければ-1

	DrawObject()
	{
		transform = GameMath::IdentityMatrix();
		modelID = 0;
		bone = nullptr;
		cubeMap = -1;
	}


};

//2D用
struct DrawFlatObject
{
	D3DXVECTOR2 leftTop;//ポリゴンの左上の座標
	D3DXVECTOR2 size;//ポリゴンの幅

	D3DXVECTOR2 texSize;//uvのサイズ
	D3DXVECTOR2 uv;//オフセット

	float rot;//回転情報　radianじゃなくて角度
	int tex;
};