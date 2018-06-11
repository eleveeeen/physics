//------------------
//ライブラリ
//------------------

#pragma comment(lib,"d3dx9.lib")
#pragma comment (lib,"d3d11.lib")
#pragma comment (lib,"D3DCompiler.lib")
#pragma comment (lib,"dxguid.lib")
#pragma comment (lib,"DXGI.lib")
#ifdef _DEBUG
#pragma comment(lib,"d3dx11d.lib")
#else
#pragma comment(lib,"d3dx11.lib")
#endif
//--------------------
//インクルード
//--------------------
#ifdef _DEBUG
#include<crtdbg.h>
#endif
#include<D3DX11.h>
#include"framework\DebugConsole\DebugConsole.h"
#include"framework/GameMain/GameMain.h"
#include"scene\SceneList.h"

#include"framework\Utility\GameMath.h"
#include"framework\Input\CInputKeyboard.h"


//エントリーポイント windows ver 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpGmdLine, int nCmdShow)
{

#ifdef _DEBUG


	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	//メモリリーク場所特定用
//	_crtBreakAlloc = 570;

#endif



	DEBUG::InitDebugConsole();
	DEBUG::PrintfColor(EConsoleColor::H_WHITE, "セットアップ開始\n");


	//ゲーム全体の初期化
	GameMain::GetInstance()->Setup(hInstance,720,1080);
	GameMain::GetInstance()->SetScene(new GameScene);

	DEBUG::PrintfColor(EConsoleColor::H_WHITE, "ゲーム開始\n");

	//ゲーム開始
	return GameMain::GetInstance()->Run();

	
}

//----------------------------------------------------------------------------------------------------------//
//フレームワークを使用しての反省点等																		//
//----------------------------------------------------------------------------------------------------------//
//テンプレート等の冗長になるような型名のうち最初から用意されているものはtypedefで用意しようかなと思った。	//
//シェーダー周りが雑だったから全面的に見直しが必要だなと思った。											//
//動的シェーダリンクを検討	実装した																		//
//・頂点シェーダのボーンの処理																				//
//・ピクセルシェーダの処理																					//
//描画前にシェーダー毎のソートが必要だなと思った。															//
//PBRにするにしても金属、非金属で分ける必要があると思うため													//
//モデルの方をで頂点を全て結合していたけれど結合せずにマテリアル単位で管理してもいい気がした				//
//		今後モデル毎ではなくマテリアル毎の描画に切り替えるため												//
//		この場合単一モデル内のでパーツの切り替えを実装できるようにになる気がする							//
//GameMathの中身をわけて使いやすくする必要があるかなと思った												//
//		最低でもベクトル、クォータニオン、行列、その他でわける												//
//モデルをテキスト形式からバイナリ形式にする	変更した													//
//マテリアル管理クラスがほしい					作成した													//
//環境マップの管理周りが突貫工事になったから次回では修正したい												//
//レンダリング方式をディファードやフォワードプラスにしようかなと思った。（優先度低）						//
//ファサードやアダプターを用いてGameObject側から使いやすい形にしたい										//
//定数バッファのMapUnmapがボトルネックになるのではないかと指摘されたから次回修正							//
//----------------------------------------------------------------------------------------------------------//
