//------------------------------
//ゲームオブジェクトの基底クラス
//------------------------------

#ifndef _BaseObject_h_
#define _BaseObject_h_

#include<d3dx9.h>
#include"../../framework/Graphics/GraphicsManager.h"
#include"../../framework/Graphics/CubeMap/CubeMapManager.h"
class BaseObject
{
protected://メンバ変数
	TemplateDrawObject<DrawObject> data;
public:
	virtual void Update() = 0;
	//最低限の描画
	virtual void Draw()
	{
		GetGraphicsPipeline().Set(data);
	}


	virtual void Finalize()
	{
		MACRO::SAFE_DELETE(data.data.bone);
		if (data.data.cubeMap != -1)
		{
			GetCubeMapManager().Destroy(data.data.cubeMap);
		}
	}


	//回転情報セット
	void SetRotation(const D3DXQUATERNION& rot)
	{
		//座標を一時避難
		D3DXVECTOR3 temp(data.data.transform.m[3]);
		D3DXMatrixRotationQuaternion(&data.data.transform, &rot);
		SetPosition(temp);
	}

	//座標セット
	void SetPosition(const D3DXVECTOR3& pos)
	{
		memcpy(&data.data.transform.m[3], pos, sizeof(float) * 3);
	}

	//回転&座標セット
	void SetTransform(const D3DXMATRIX& transform)
	{
		data.data.transform = transform;
	}
	//回転&座標セット
	void SetTransform(const D3DXQUATERNION& rot, const D3DXVECTOR3& pos)
	{
		D3DXMatrixRotationQuaternion(&data.data.transform, &rot);
		SetPosition(pos);
	}



	
protected:
	//モデルのID登録
	void SetModelID(int id)
	{
		data.data.modelID = id;
	}

	//ボーン登録
	void SetBone(BoneObjectEx& bone)
	{
		data.data.bone = &bone;
	}

	//キューブマップ登録
	void SetCubeMap(int id)
	{
		data.data.cubeMap = id;
	}



};


#endif

