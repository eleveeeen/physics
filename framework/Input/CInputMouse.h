//----------------------------------------------------
// マウス関連のヘッダー
//----------------------------------------------------

#ifndef _CInputMouse_h_
#define _CInputMouse_h_

//-------------------------------------------
//インクルード
//-------------------------------------------
#include<Windows.h>
#include"../DebugConsole/DebugConsole.h"

//-------------------------------------------
//定数定義
//-------------------------------------------

enum eMouse
{
	LEFT_HOLD		= 0x0000004,
	LEFT_TRIGGER	= 0x0000040,
	LEFT_RELEASE	= 0x0000400,
	RIGHT_HOLD		= 0x0000001,
	RIGHT_TRIGGER	= 0x0000010,
	RIGHT_RELEASE	= 0x0000100,
	MIDDLE_HOLD		= 0x0000002,
	MIDDLE_TRIGGER	= 0x0000020,
	MIDDLE_RELEASE	= 0x0000200,
	
};

//-------------------------------------------------
//マウス入力クラス　シングルトン
//最終更新日	2015/03/26
//-------------------------------------------------
class CInputMouse
{
private://メンバ変数
	HWND gameWnd;
	SHORT mouseState;		//マウスの情報
	POINT oldCursorPos;		//前フレームのマウス座標
	POINT nowCursorPos;		//現フレームのマウス座標
	
	int windowWidth;		//ウィンドウの横幅
	int windowHeight;		//ウィンドウの縦幅
	int centerWidth;		//ウィンドウの中心座標
	int centerHeight;		//ウィンドウの中心座標

public:

	//デストラクタ
	~CInputMouse(){}

	//----------------------------------
	//インスタンス呼び出し
	//引数			無し
	//戻り値		自身のポインタ
	//最終更新日	2015/03/26
	//----------------------------------
	static CInputMouse* GetInstance()
	{

		static CInputMouse singleton;
		return &singleton;
	}

	//----------------------------------
	//ウィンドウのハンドルセット
	//引数			ウィンドウのハンドル
	//戻り値		無し
	//最終更新日	2015/03/26
	//----------------------------------
	void SetWindowHandle(HWND hWnd)
	{
		gameWnd = hWnd;
	}

	//----------------------------------
	//ウィンドウのサイズセット
	//引数			ウィンドウのハンドル
	//戻り値		無し
	//最終更新日	2015/07/07
	//----------------------------------
	void SetWindowSize(int x, int y)
	{
		windowHeight = y;
		windowWidth = x;
		centerHeight = y >> 1;
		centerWidth = x >> 1;
	}

	//----------------------------------
	//マウスの入力更新
	//引数			ウィンドウハンドル
	//戻り値		無し
	//最終更新日	2015/03/26
	//----------------------------------
	void Update()
	{
		HWND fgWnd = GetForegroundWindow();//フォアグラウンドのウィンドウハンドル取得

		//ゲームのウィンドウがフォアグラウンドでなければreturn
		if (gameWnd != fgWnd)
			return;

		//今入ってるマウス情報を過去のマウス情報に移動
		mouseState = (mouseState << 12) & 0xF000;

		//現在のマウス情報取得
		mouseState |= (GetKeyState(VK_LBUTTON) & 0x80) >> 5;//左クリック
		mouseState |= (GetKeyState(VK_MBUTTON) & 0x80) >> 6;//ホイールクリック
		mouseState |= (GetKeyState(VK_RBUTTON) & 0x80) >> 7;//右クリック

		//トリガー取得
		mouseState |= (((mouseState << 4) ^ (mouseState >> 8))&(mouseState << 4)) & 0x0070;

		//リリース取得
		mouseState |= (((mouseState << 8) ^ (mouseState >> 4))&(mouseState >> 4)) & 0x0700;

		//マウス座標の更新
		UpdateCursorPos();

	}

	//----------------------------------
	//マウスのホールド情報取得
	//引数			マウスのコード
	//戻り値		入力情報
	//最終更新日	2015/03/26
	//----------------------------------
	BYTE GetMouseHold(SHORT code)
	{
		return (BYTE)(mouseState&code);
	}

	//----------------------------------
	//マウスのトリガー情報取得
	//引数			マウスのコード
	//戻り値		入力情報
	//最終更新日	2015/03/26
	//----------------------------------
	BYTE GetMouseTrigger(SHORT code)
	{
		return (BYTE)((mouseState&code) >> 4);
	}

	//----------------------------------
	//マウスのリリース情報取得
	//引数			マウスのコード
	//戻り値		入力情報
	//最終更新日	2015/03/26
	//----------------------------------
	BYTE GetMouseRelease(SHORT code)
	{
		return (BYTE)((mouseState&code) >> 8);
	}


	//----------------------------------
	//マウス座標の更新
	//引数			出力用の相対座標
	//戻り値		無し
	//最終更新日	2015/03/26
	//----------------------------------
	void UpdateCursorPos()
	{
		oldCursorPos.x = nowCursorPos.x;
		oldCursorPos.y = nowCursorPos.y;

		::GetCursorPos(&nowCursorPos);
		ScreenToClient(gameWnd, &nowCursorPos);
	
	}

	//----------------------------------
	//マウスの相対座標取得
	//引数			出力用の相対座標
	//戻り値		無し
	//最終更新日	2015/03/26
	//----------------------------------
	void GetRelativeCursorPos(LPPOINT pPos = nullptr)
	{
		if (pPos == nullptr)
			return;

		pPos->x = nowCursorPos.x - oldCursorPos.x;
		pPos->y = nowCursorPos.y - oldCursorPos.y;

	}

	//----------------------------------
	//マウスの絶対座標取得
	//引数			出力用の絶対座標
	//戻り値		出力用の絶対座標
	//最終更新日	2015/03/26
	//----------------------------------
	LPPOINT	GetAbsoluteCursorPos(LPPOINT pPos = nullptr)
	{
		if (pPos == nullptr)
			return &nowCursorPos;

		pPos->x = nowCursorPos.x;
		pPos->y = nowCursorPos.y;
		return pPos;
	}

	//----------------------------------
	//画面の中心からのベクトル取得
	//画面内のどの位置かを-1~1で表す
	//引数			出力用のポインタ
	//戻り値		無し
	//最終更新日	2015/07/07
	//----------------------------------
	void GetAbsoluteMouseVec(float* xVec,float* yVec)
	{
		*xVec = (float)(nowCursorPos.x - centerWidth) / centerWidth;
		*yVec = -(float)(nowCursorPos.y - centerHeight) / centerHeight;
	}
	

	void GetAbsoluteMousePos(float& x, float&y)
	{

		HWND hWnd = ::GetActiveWindow();
		POINT point;
		RECT rect;
		::GetCursorPos(&point);
		::ScreenToClient(hWnd, &point);
		::GetClientRect(hWnd, &rect);

		x = point.x - ((rect.right - rect.left) >> 1);
		y = ((rect.bottom - rect.top) >> 1) - point.y;
		
	}

private:

	//コンストラクタ
	CInputMouse()
	{
		gameWnd = NULL;
		mouseState = 0;
		oldCursorPos.x = 0;
		oldCursorPos.y = 0;
		nowCursorPos.x = 0;
		nowCursorPos.y = 0;
	}
};

inline CInputMouse* InputMouseManager()
{
	return CInputMouse::GetInstance();
}

#endif