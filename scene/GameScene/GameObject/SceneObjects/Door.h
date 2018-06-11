#pragma once


#include"../../../../framework/BaseObject/BaseObject.h"

//ドアのクラス
class Door:public BaseObject
{

private://メンバ変数
	int rigidBox;		//ドア部分の剛体のID
	int rigidCapsule;	//柱部分の剛体のID

	TemplateDrawObject<DrawObject> pillar; //柱部分のモデル

public://メンバ関数

	Door();
	~Door();

	void Initialize(const D3DXVECTOR3& initPos = D3DXVECTOR3(0, 0, 0),	//初期の座標
		const D3DXQUATERNION& initRot = D3DXQUATERNION(0, 0, 0, 1),		//初期の姿勢
		const float lowerAngle = -180.0f,const float upperAngle = 180.0f);	//ドアの回転範囲(-180~180)

	void Update();
	void Draw();

private://メンバ変数

	//剛体設定
	void InitRigidBody(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot,
		const float lowerAngle, const float upperAngle);

};