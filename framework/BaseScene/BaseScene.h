//------------------------
//シーンの基底クラス
//仮想クラスだから継承して使う
//------------------------
#ifndef _BaseScene_h_
#define _BaseScene_h_

class BaseScene
{

public:
	//シーン開始時に1度だけ入る処理
	virtual void SceneStart() = 0;
	//シーン終了時に1度だけ入る処理
	virtual void SceneEnd() = 0;

	//内部の初期化処理
	virtual int Initialize() = 0;
	//内部の終了処理
	virtual int Finalize() = 0;
	//内部の更新処理
	virtual BaseScene* Update() = 0;
	//内部の描画処理
	virtual void Render() = 0;
};





#endif