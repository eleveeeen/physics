#include"GameMain.h"
#include"../Utility/Macro.h"
#include"../Input/CInputKeyboard.h"
#include"../Input/CInputMouse.h"
#include"../Utility/FpsControl.h"
#include"../DebugConsole/DebugConsole.h"
#include"../Shader/Shader/ShaderManager.h"
#include"../Graphics/GraphicsManager.h"
#include"../GameManager/GameLight.h"

//コンストラクタ
GameMain::GameMain()
{
	pWindow = nullptr;


	windowMode = true;	//とりあえずウィンドウ表示
	exitFlag = false;
}

GameMain::~GameMain()
{
	MACRO::SAFE_DELETE(pWindow);
	dx11App.Finalize();

	//フルスクリーンだった場合ウィンドウに戻して解放
	if (!windowMode)
	{

	}

}



//セットアップ
HRESULT GameMain::Setup(HINSTANCE hInst,float windowHeight,float windowWidth)
{

	FPSControl()->Create();
	pWindow = new CWindow(hInst, nullptr, windowWidth, windowHeight);
	InputMouseManager()->SetWindowHandle(pWindow->GethWnd());
	InputMouseManager()->SetWindowSize(windowWidth, windowHeight);
	//11の初期化処理
	dx11App.Initialize(pWindow->GethWnd(),windowWidth,windowHeight);
	//11の描画まわりの初期化処理
	GraphicsManager::GetInstance().Initialize(dx11App.GetDevice(), dx11App.GetDeviceContext(), dx11App.GetSwapChain(), windowWidth, windowHeight);
	
	//射影行列のセット

	D3DXMATRIX ortho;
	D3DXMatrixIdentity(&ortho);
	ortho._11 = 2.0f / windowWidth;
	ortho._22 = 2.0f / windowHeight;
	ortho._33 = -1;

	D3DXMATRIX pers;
	D3DXMatrixPerspectiveFovLH(&pers, 3.1415f / 4.0f, windowWidth / windowHeight, 0.1f, 10000.0f);

	GetCBMatrixManager().SetProjMatrix(ortho, pers);
	
	//デフォルトのライトのセット
	LightObject defaultLight;

	//ライトの色情報
	defaultLight.color.diffuse.x = 0.8f;
	defaultLight.color.diffuse.y = 0.8f;
	defaultLight.color.diffuse.z = 0.8f;
	defaultLight.color.diffuse.w = 1.0f;
	defaultLight.color.ambient.x = 0.2f;
	defaultLight.color.ambient.y = 0.2f;
	defaultLight.color.ambient.z = 0.2f;

	//ライトの方向
	defaultLight.direction.x = -1;
	defaultLight.direction.y = 0;
	defaultLight.direction.z = 1;
	D3DXVec3Normalize(&defaultLight.direction, &defaultLight.direction);

	GetLightMamager().SetLight(defaultLight);
	GetLightMamager().UpdateLight();


	return S_OK;
}

void GameMain::SetScene(BaseScene* scene)
{
	sceneManager.ChangeScene(scene);

}

//ゲームループ開始

int GameMain::Run()
{

	while (!exitFlag)
	{
		//ウィンドウ更新
		TransactMessage();
		
		if (FPSControl()->GetUpdate())
		{
			//キーボード更新
			InputKeyManager()->Update();
			//マウス更新
			InputMouseManager()->Update();

			//ゲーム更新
			sceneManager.Execute();

			//FPS出力
			char titlebar[100];
			sprintf(titlebar,"%s-FPS[%.1f]",WINDOW_TITLE,FPSControl()->GetAverageFps());
			pWindow->UpdateWindowsText(titlebar);
			//FPS制御
			FPSControl()->CountFPS(60);
			FPSControl()->UpdateStart();

		}


		//エスケープキーを押すかウィンドウが破棄されたら終了
		if (InputKeyManager()->GetKeyTrigger(VK_ESCAPE))
		{
			exitFlag = true;
		}
		if (pWindow->GetWindowDestroy())
		{
			exitFlag = true;
		}

	}

	sceneManager.Finalize();

	return 0;

}


void GameMain::TransactMessage()
{
	MSG msg;
	//メッセージ取出し
	if (PeekMessage(&msg, pWindow->GethWnd(), 0, 0, PM_REMOVE))
	{
		//メッセージ処理
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}