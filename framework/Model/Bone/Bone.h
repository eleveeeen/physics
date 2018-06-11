#pragma once
//ボーンの構造体


#include<d3dx9.h>

//階層情報
struct BoneTree
{
	int parentID;	//親のインデックス番号	なければ-1
	int childID;	//子
	int siblingID;	//兄弟
	int myID;		//自身
	BoneTree()
	{
		parentID = -1;
		childID = -1;
		siblingID = -1;
		myID = -1;
	}
};

//回転、移動情報
struct BoneData
{
	D3DXQUATERNION rot;
	D3DXVECTOR3 pos;
};

