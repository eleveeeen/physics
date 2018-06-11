#ifndef _Ray_h_
#define _Ray_h_
//レイ情報のヘッダー
//マウスカーソルで画面の中を触る時などに使用


#include"..\Base\Common.h"
#include"SubData.h"

//レイキャストのモード
enum eRayFacetMode
{
	eFrontOnly=0,		//前のみ
	eBackOnly,			//後ろのみ
	eFrontAndBack,		//両方

};


struct RayInput
{
	D3DXVECTOR3 startPos;
	D3DXVECTOR3 dir;
	UInt32		contactFilterSelf;
	UInt32		contactFilterTarget;
	UInt8		facetMode;

	void Reset()
	{
		contactFilterSelf = contactFilterTarget = 0xffffffff;
		facetMode = eRayFacetMode::eFrontOnly;
	}


};


struct RayOutput
{
	D3DXVECTOR3 contactPoint;
	D3DXVECTOR3 contactNormal;
	float		variable;
	UInt16		objectID;
	UInt8		shapeID;
	bool		contactFlag;
	SubData		subData;

};



#endif