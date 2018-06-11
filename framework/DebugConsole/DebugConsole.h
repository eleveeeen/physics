//----------------------------------------------------
// �f�o�b�O���p�̃R���\�[���֘A�̃w�b�_�[
//----------------------------------------------------

#ifndef _DebugConsole_h_
#define _DebugConsole_h_

//-------------------------------------------
//�C���N���[�h
//-------------------------------------------
#include<Windows.h>
#include<string>

//-------------------------------------------
//�萔��`
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
//�f�o�b�O�p�R���\�[��	�V���O���g��
//�ŏI�X�V��	2015/03/12
//-------------------------------------------------
class CDebugConsole
{
private://�����o�ϐ�
	FILE*	_pFile;			//�t�@�C���|�C���^
	HANDLE	_stdOut;		//�o�͐�n���h��
	CHAR	_strBuf[256];	//������o�b�t�@
	WORD	_backColor;		//�w�i�F
	WORD	_fontColor;		//�����F

public:
	//----------------------------------
	//�C���X�^���X�Ăяo��
	//����			����
	//�߂�l		���g�̃|�C���^
	//�ŏI�X�V��	2015/03/12
	//----------------------------------
	static CDebugConsole* GetInstance()
	{
		static CDebugConsole singleton;
		return &singleton;
	}
	//�R���X�g���N�^


	//�f�X�g���N�^
	~CDebugConsole()
	{
		this->Destory();
	}

	//----------------------------------
	//�������
	//����			����
	//�߂�l		����
	//�ŏI�X�V��	2015/03/12
	//----------------------------------
	void Destory()
	{
		if (_stdOut == nullptr)
			return;

		if (_pFile != nullptr)
			fclose(_pFile);

		//�R���\�[���̉��
		::FreeConsole();
		_stdOut = nullptr;
	}

	//----------------------------------
	//���O�t�@�C���쐬�p�̃t�@�C���I�[�v��
	//����			�t�@�C����
	//�߂�l		����
	//�ŏI�X�V��	2015/03/12
	//----------------------------------
	void CreateDebugLog(LPCTSTR fileName)
	{
		fopen_s(&_pFile, fileName, "w");
	}
	//----------------------------------
	//�w�i�F�ύX
	//����			�F�̔ԍ�
	//�߂�l		����
	//�ŏI�X�V��	2015/03/12
	//----------------------------------
	void SetBackColor(WORD backColor = L_BLACK)
	{
		this->SetColor(backColor, _fontColor);
	}
	//----------------------------------
	//�����F�ύX
	//����			�F�̔ԍ�
	//�߂�l		����
	//�ŏI�X�V��	2015/03/12
	//----------------------------------
	void SetFontColor(WORD fontColor = L_WHITE)
	{
		this->SetColor(_backColor, fontColor);
	}
	//----------------------------------
	//�����A�w�i�F�ύX
	//����			�w�i�F�̔ԍ�,�����F�̔ԍ�
	//�߂�l		����
	//�ŏI�X�V��	2015/03/12
	//----------------------------------
	void SetColor(WORD backColor = L_BLACK, WORD fontColor = L_WHITE)
	{
		_backColor = backColor;
		_fontColor = fontColor;
		::SetConsoleTextAttribute(_stdOut, _fontColor | (backColor << 4));
	}

	//----------------------------------
	//������`��
	//����			������,�ψ���
	//�߂�l		�o�͂���������
	//�ŏI�X�V��	2015/03/12
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
	//��ʏ���
	//����			����
	//�߂�l		����
	//�ŏI�X�V��	2015/03/12
	//----------------------------------
	
	void Clear()
	{
		system("cls");
	}
	//----------------------------------
	//���閳����
	//����			����
	//�߂�l		����
	//�ŏI�X�V��	2015/03/12
	//----------------------------------
	void RemoveClose(void)
	{
		HMENU hMenu = GetSystemMenu(::GetConsoleWindow(), FALSE);
		::RemoveMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
	}
private:

	//�R���X�g���N�^
	CDebugConsole()
	{
		_pFile = nullptr;
		SMALL_RECT	rc = { 0, 0, 1024, 512 };
		COORD		cd = { rc.Right + 1, rc.Bottom + 1 };
		::AllocConsole();	//�R���\�[���̊m��
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
	//�ψ����ɕϊ�
	//����			����
	//�߂�l		����
	//�ŏI�X�V��	2015/03/12
	//----------------------------------
	inline	void	OutPutStringArgList(CONST LPSTR str, va_list argList)
	{
		char buf[1024];
		_vsnprintf_s(buf, _countof(buf), str, argList);
		OutputDebugStringA(buf);
		CDebugConsole::GetInstance()->Printf(buf);
	}
	//----------------------------------
	//�f�o�b�O�p�R���\�[���̏�����
	//����			����
	//�߂�l		����
	//�ŏI�X�V��	2015/03/12
	//----------------------------------

	inline void InitDebugConsole()
	{
		CDebugConsole::GetInstance()->RemoveClose();
	}
	//----------------------------------
	//������`��
	//����			������,�ψ���
	//�߂�l		����
	//�ŏI�X�V��	2015/03/12
	//----------------------------------
	inline void Printf(CONST LPSTR str, ...)
	{
		CDebugConsole::GetInstance()->SetColor();

		// �������`��
		va_list args;
		va_start( args, str );
		OutPutStringArgList(str, args);
		va_end(args);
	}
	//----------------------------------
	//�w��F�ŕ�����`��
	//����			������,�ψ���
	//�߂�l		����
	//�ŏI�X�V��	2015/03/12
	//----------------------------------
	inline void PrintfColor(WORD textColor, CONST LPSTR str, ...)
	{
		// �F���Z�b�g
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