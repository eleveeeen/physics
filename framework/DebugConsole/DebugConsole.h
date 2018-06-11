//----------------------------------------------------
// デバッグ環境用のコンソール関連のヘッダー
//----------------------------------------------------

#ifndef _DebugConsole_h_
#define _DebugConsole_h_

//-------------------------------------------
//インクルード
//-------------------------------------------
#include<Windows.h>
#include<string>

//-------------------------------------------
//定数定義
//-------------------------------------------

enum EConsoleColor
{
	L_BLACK = 0,
	L_BLUE,
	L_GREEN,
	L_CYAN,
	L_RED,
	L_PURPLE,
	L_YELLOW,
	L_WHITE,
	H_BLACK,
	H_BLUE,
	H_GREEN,
	H_CYAN,
	H_RED,
	H_PURPLE,
	H_YELLOW,
	H_WHITE
};
//-------------------------------------------------
//デバッグ用コンソール	シングルトン
//最終更新日	2015/03/12
//-------------------------------------------------
class CDebugConsole
{
private://メンバ変数
	FILE*	_pFile;			//ファイルポインタ
	HANDLE	_stdOut;		//出力先ハンドル
	CHAR	_strBuf[256];	//文字列バッファ
	WORD	_backColor;		//背景色
	WORD	_fontColor;		//文字色

public:
	//----------------------------------
	//インスタンス呼び出し
	//引数			無し
	//戻り値		自身のポインタ
	//最終更新日	2015/03/12
	//----------------------------------
	static CDebugConsole* GetInstance()
	{
		static CDebugConsole singleton;
		return &singleton;
	}
	//コンストラクタ


	//デストラクタ
	~CDebugConsole()
	{
		this->Destory();
	}

	//----------------------------------
	//解放処理
	//引数			無し
	//戻り値		無し
	//最終更新日	2015/03/12
	//----------------------------------
	void Destory()
	{
		if (_stdOut == nullptr)
			return;

		if (_pFile != nullptr)
			fclose(_pFile);

		//コンソールの解放
		::FreeConsole();
		_stdOut = nullptr;
	}

	//----------------------------------
	//ログファイル作成用のファイルオープン
	//引数			ファイル名
	//戻り値		無し
	//最終更新日	2015/03/12
	//----------------------------------
	void CreateDebugLog(LPCTSTR fileName)
	{
		fopen_s(&_pFile, fileName, "w");
	}
	//----------------------------------
	//背景色変更
	//引数			色の番号
	//戻り値		無し
	//最終更新日	2015/03/12
	//----------------------------------
	void SetBackColor(WORD backColor = L_BLACK)
	{
		this->SetColor(backColor, _fontColor);
	}
	//----------------------------------
	//文字色変更
	//引数			色の番号
	//戻り値		無し
	//最終更新日	2015/03/12
	//----------------------------------
	void SetFontColor(WORD fontColor = L_WHITE)
	{
		this->SetColor(_backColor, fontColor);
	}
	//----------------------------------
	//文字、背景色変更
	//引数			背景色の番号,文字色の番号
	//戻り値		無し
	//最終更新日	2015/03/12
	//----------------------------------
	void SetColor(WORD backColor = L_BLACK, WORD fontColor = L_WHITE)
	{
		_backColor = backColor;
		_fontColor = fontColor;
		::SetConsoleTextAttribute(_stdOut, _fontColor | (backColor << 4));
	}

	//----------------------------------
	//文字列描画
	//引数			文字列,可変引数
	//戻り値		出力した文字数
	//最終更新日	2015/03/12
	//----------------------------------
	DWORD Printf(const std::string str, ...)
	{
		DWORD len;
		va_list args;
		va_start(args, str);
		vsprintf_s(_strBuf, _countof(_strBuf), str.c_str(), args);
		va_end(args);
		::WriteConsoleA(_stdOut, _strBuf, strlen(_strBuf), &len, NULL);
		return len;
		
	}


	//----------------------------------
	//画面消去
	//引数			無し
	//戻り値		無し
	//最終更新日	2015/03/12
	//----------------------------------
	
	void Clear()
	{
		system("cls");
	}
	//----------------------------------
	//閉じる無効化
	//引数			無し
	//戻り値		無し
	//最終更新日	2015/03/12
	//----------------------------------
	void RemoveClose(void)
	{
		HMENU hMenu = GetSystemMenu(::GetConsoleWindow(), FALSE);
		::RemoveMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
	}
private:

	//コンストラクタ
	CDebugConsole()
	{
		_pFile = nullptr;
		SMALL_RECT	rc = { 0, 0, 1024, 512 };
		COORD		cd = { rc.Right + 1, rc.Bottom + 1 };
		::AllocConsole();	//コンソールの確保
		_stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleScreenBufferSize(_stdOut, cd);
		SetConsoleWindowInfo(_stdOut, TRUE, &rc);
		_backColor = L_BLACK;
		_fontColor = L_WHITE;
		this->SetFontColor(_fontColor);
	}
};

namespace DEBUG
{
#ifdef _DEBUG
	//----------------------------------
	//可変引数に変換
	//引数			無し
	//戻り値		無し
	//最終更新日	2015/03/12
	//----------------------------------
	inline	void	OutPutStringArgList(CONST LPSTR str, va_list argList)
	{
		char buf[1024];
		_vsnprintf_s(buf, _countof(buf), str, argList);
		OutputDebugStringA(buf);
		CDebugConsole::GetInstance()->Printf(buf);
	}
	//----------------------------------
	//デバッグ用コンソールの初期化
	//引数			無し
	//戻り値		無し
	//最終更新日	2015/03/12
	//----------------------------------

	inline void InitDebugConsole()
	{
		CDebugConsole::GetInstance()->RemoveClose();
	}
	//----------------------------------
	//文字列描画
	//引数			文字列,可変引数
	//戻り値		無し
	//最終更新日	2015/03/12
	//----------------------------------
	inline void Printf(CONST LPSTR str, ...)
	{
		CDebugConsole::GetInstance()->SetColor();

		// 文字列を描画
		va_list args;
		va_start( args, str );
		OutPutStringArgList(str, args);
		va_end(args);
	}
	//----------------------------------
	//指定色で文字列描画
	//引数			文字列,可変引数
	//戻り値		無し
	//最終更新日	2015/03/12
	//----------------------------------
	inline void PrintfColor(WORD textColor, CONST LPSTR str, ...)
	{
		// 色をセット
		CDebugConsole::GetInstance()->SetFontColor( textColor );

		va_list args;
		va_start(args, str);
		OutPutStringArgList(str, args);
		va_end(args);
	}

#else

	inline	void	OutPutStringArgList(CONST LPSTR str, va_list argList)
	{
		 
	}

	inline void InitDebugConsole()
	{
 
	}

	inline void Printf(CONST LPSTR str, ...){
	 
	}

	inline void PrintfColor(WORD textColor, CONST LPSTR str, ...)
	{
 
	}
#endif
}

#endif