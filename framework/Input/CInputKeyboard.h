//----------------------------------------------------
// キーボード関連のヘッダー
//----------------------------------------------------

#ifndef _CInputKeyboard_h_
#define _CInputKeyboard_h_

//-------------------------------------------
//インクルード
//-------------------------------------------
#include<Windows.h>

//-------------------------------------------
//定数定義
//-------------------------------------------
enum//仮想キーコードのアルファベットと数字部分がないから用意
{
	VK_0 = '0',
	VK_1,
	VK_2,
	VK_3,
	VK_4,
	VK_5,
	VK_6,
	VK_7,
	VK_8,
	VK_9,

	VK_A = 'A',
	VK_B,
	VK_C,
	VK_D,
	VK_E,
	VK_F,
	VK_G,
	VK_H,
	VK_I,
	VK_J,
	VK_K,
	VK_L,
	VK_M,
	VK_N,
	VK_O,
	VK_P,
	VK_Q,
	VK_R,
	VK_S,
	VK_T,
	VK_U,
	VK_V,
	VK_W,
	VK_X,
	VK_Y,
	VK_Z,
};

//-------------------------------------------------
//キーボード情報構造体
//最終更新日	2015/03/26
//-------------------------------------------------
 struct SKeyboard
{
	BYTE hold[256];
	BYTE trigger[256];
	BYTE release[256];
	BYTE vec[2];//上下左右の傾き情報
};

//-------------------------------------------------
//キーボード入力クラス　シングルトン
//最終更新日	2015/03/26
//-------------------------------------------------
class CInputKey
{
private://定数定義

	//スティックみたいに扱う時のそれぞれのキー
	const static int VEC_LEFT	= VK_A;//左
	const static int VEC_RIGHT	= VK_D;//右
	const static int VEC_UP		= VK_W;//上
	const static int VEC_DOWN	= VK_S;//下

private://メンバ変数
	SKeyboard keyState;//キーボード情報

public:

	//デストラクタ
	~CInputKey(){};//恐らく無し

	//----------------------------------
	//インスタンス呼び出し
	//引数			無し
	//戻り値		自身のポインタ
	//最終更新日	2015/03/26
	//----------------------------------
	static CInputKey* GetInstance()
	{
		static CInputKey singleton;
		return &singleton;
	}

	//----------------------------------
	//キーボードの更新
	//引数			無し
	//戻り値		無し
	//最終更新日	2015/03/26
	//----------------------------------
	void Update()
	{
		BYTE oldState[256];

		//過去のキー情報をコピー
		memcpy(oldState, keyState.hold, sizeof(BYTE) * 256);
		ZeroMemory(keyState.hold, sizeof(BYTE) * 256);

		//キーボードの状態を取得
		GetKeyboardState(keyState.hold);

		//コンパイラさんが頑張って並列処理されるといいな

		//ホールドキー情報更新
		for (int i = 0; i < 256; ++i)
		{
			keyState.hold[i] = keyState.hold[i] & 0x80;
		}

		//トリガーキー情報更新
		for (int i = 0; i < 256; ++i)
		{
			keyState.trigger[i] = (keyState.hold[i] ^ oldState[i]) & keyState.hold[i];
		}

		//リリースキー情報更新	
		for (int i = 0; i < 256; ++i)
		{
			keyState.release[i] = (keyState.hold[i] ^ oldState[i]) & oldState[i];
		}

		//上下の傾き更新 
		keyState.vec[0] = (keyState.hold[VEC_UP] >> 7) - (keyState.hold[VEC_DOWN] >> 7);
		//左右の傾き更新
		keyState.vec[1] = (keyState.hold[VEC_RIGHT] >> 7) - (keyState.hold[VEC_LEFT] >> 7);
	}

	//----------------------------------
	//ホールド情報取得
	//引数			キーコード
	//戻り値		キーの状態
	//最終更新日	2015/03/26
	//----------------------------------
	BYTE GetKeyHold(BYTE keyCode)
	{
		return keyState.hold[keyCode];
	}

	//----------------------------------
	//トリガー情報取得
	//引数			キーコード
	//戻り値		キーの状態
	//最終更新日	2015/03/26
	//----------------------------------
	BYTE GetKeyTrigger(BYTE keyCode)
	{
		return keyState.trigger[keyCode];
	}

	//----------------------------------
	//リリース情報取得
	//引数			キーコード
	//戻り値		キーの状態
	//最終更新日	2015/03/26
	//----------------------------------
	BYTE GetKeyRelease(BYTE keyCode)
	{
		return keyState.release[keyCode];
	}

	//----------------------------------
	//左右のキーをベクトルで
	//引数			無し
	//戻り値		-1~1のX軸のベクトル
	//最終更新日	2015/05/07
	//----------------------------------
	signed char GetKeyVecX()
	{
		return keyState.vec[1];
	}


	//----------------------------------
	//上下のキーをベクトルで
	//引数			無し
	//戻り値		-1~1のY軸のベクトル
	//最終更新日	2015/05/07
	//----------------------------------

	signed char GetKeyVecY()
	{
		return keyState.vec[0];
	}



private:
	//コンストラクタ
	CInputKey()
	{
		ZeroMemory(&keyState, sizeof(SKeyboard));
	}
};

inline CInputKey* InputKeyManager()
{
	return CInputKey::GetInstance();
}

#endif