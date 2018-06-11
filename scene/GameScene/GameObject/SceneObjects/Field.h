#pragma once
#include"../../../../framework/BaseObject/BaseObject.h"

//剛体を閉じ込めるフィールド
//8つのカプセルで作成
class GameField :public BaseObject
{
private://メンバ変数
	TemplateDrawObject<DrawObject> capsule[8];
	int rigidID[8];
public://メンバ関数
	GameField();
	~GameField();

	void Initialize();

	void Update();

private://メンバ関数

	void InitRigidBody();//剛体設定


};