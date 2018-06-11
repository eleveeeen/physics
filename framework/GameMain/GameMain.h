#ifndef _GameMain_h_
#define _GameMain_h_

//インクルード
#include"../Window/CWindow.h"
#include"../SceneManager/SceneManager.h"
#include"../DirectX11/DirectX11Application.h"


class GameMain
{

private://メンバ変数
	LPWINDOW pWindow;	//ゲームのメインウィンドウ
	SceneManager sceneManager;	//シーン管理
	DirectX11Application dx11App;	//DirectX11のあれ

	bool windowMode;	//ウィンドウモード
	bool exitFlag;		//ゲームループからの離脱用
public://メンバ変数

public:

	~GameMain();

	static GameMain* GetInstance()
	{
		static GameMain instance;
		return &instance;
	}

	HRESULT Setup(HINSTANCE hInst, float windowHeight = 480, float windowWidth = 640);//セットアップ処理
	void SetScene(BaseScene* pScene);
	int Run();//ゲームループ開始


private:
	void TransactMessage();

	GameMain();

};



#endif