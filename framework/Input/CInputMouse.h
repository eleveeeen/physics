//----------------------------------------------------
// �}�E�X�֘A�̃w�b�_�[
//----------------------------------------------------

#ifndef _CInputMouse_h_
#define _CInputMouse_h_

//-------------------------------------------
//�C���N���[�h
//-------------------------------------------
#include<Windows.h>
#include"../DebugConsole/DebugConsole.h"

//-------------------------------------------
//�萔��`
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
//�}�E�X���̓N���X�@�V���O���g��
//�ŏI�X�V��	2015/03/26
//-------------------------------------------------
class CInputMouse
{
private://�����o�ϐ�
	HWND gameWnd;
	SHORT mouseState;		//�}�E�X�̏��
	POINT oldCursorPos;		//�O�t���[���̃}�E�X���W
	POINT nowCursorPos;		//���t���[���̃}�E�X���W
	
	int windowWidth;		//�E�B���h�E�̉���
	int windowHeight;		//�E�B���h�E�̏c��
	int centerWidth;		//�E�B���h�E�̒��S���W
	int centerHeight;		//�E�B���h�E�̒��S���W

public:

	//�f�X�g���N�^
	~CInputMouse(){}

	//----------------------------------
	//�C���X�^���X�Ăяo��
	//����			����
	//�߂�l		���g�̃|�C���^
	//�ŏI�X�V��	2015/03/26
	//----------------------------------
	static CInputMouse* GetInstance()
	{

		static CInputMouse singleton;
		return &singleton;
	}

	//----------------------------------
	//�E�B���h�E�̃n���h���Z�b�g
	//����			�E�B���h�E�̃n���h��
	//�߂�l		����
	//�ŏI�X�V��	2015/03/26
	//----------------------------------
	void SetWindowHandle(HWND hWnd)
	{
		gameWnd = hWnd;
	}

	//----------------------------------
	//�E�B���h�E�̃T�C�Y�Z�b�g
	//����			�E�B���h�E�̃n���h��
	//�߂�l		����
	//�ŏI�X�V��	2015/07/07
	//----------------------------------
	void SetWindowSize(int x, int y)
	{
		windowHeight = y;
		windowWidth = x;
		centerHeight = y >> 1;
		centerWidth = x >> 1;
	}

	//----------------------------------
	//�}�E�X�̓��͍X�V
	//����			�E�B���h�E�n���h��
	//�߂�l		����
	//�ŏI�X�V��	2015/03/26
	//----------------------------------
	void Update()
	{
		HWND fgWnd = GetForegroundWindow();//�t�H�A�O���E���h�̃E�B���h�E�n���h���擾

		//�Q�[���̃E�B���h�E���t�H�A�O���E���h�łȂ����return
		if (gameWnd != fgWnd)
			return;

		//�������Ă�}�E�X�����ߋ��̃}�E�X���Ɉړ�
		mouseState = (mouseState << 12) & 0xF000;

		//���݂̃}�E�X���擾
		mouseState |= (GetKeyState(VK_LBUTTON) & 0x80) >> 5;//���N���b�N
		mouseState |= (GetKeyState(VK_MBUTTON) & 0x80) >> 6;//�z�C�[���N���b�N
		mouseState |= (GetKeyState(VK_RBUTTON) & 0x80) >> 7;//�E�N���b�N

		//�g���K�[�擾
		mouseState |= (((mouseState << 4) ^ (mouseState >> 8))&(mouseState << 4)) & 0x0070;

		//�����[�X�擾
		mouseState |= (((mouseState << 8) ^ (mouseState >> 4))&(mouseState >> 4)) & 0x0700;

		//�}�E�X���W�̍X�V
		UpdateCursorPos();

	}

	//----------------------------------
	//�}�E�X�̃z�[���h���擾
	//����			�}�E�X�̃R�[�h
	//�߂�l		���͏��
	//�ŏI�X�V��	2015/03/26
	//----------------------------------
	BYTE GetMouseHold(SHORT code)
	{
		return (BYTE)(mouseState&code);
	}

	//----------------------------------
	//�}�E�X�̃g���K�[���擾
	//����			�}�E�X�̃R�[�h
	//�߂�l		���͏��
	//�ŏI�X�V��	2015/03/26
	//----------------------------------
	BYTE GetMouseTrigger(SHORT code)
	{
		return (BYTE)((mouseState&code) >> 4);
	}

	//----------------------------------
	//�}�E�X�̃����[�X���擾
	//����			�}�E�X�̃R�[�h
	//�߂�l		���͏��
	//�ŏI�X�V��	2015/03/26
	//----------------------------------
	BYTE GetMouseRelease(SHORT code)
	{
		return (BYTE)((mouseState&code) >> 8);
	}


	//----------------------------------
	//�}�E�X���W�̍X�V
	//����			�o�͗p�̑��΍��W
	//�߂�l		����
	//�ŏI�X�V��	2015/03/26
	//----------------------------------
	void UpdateCursorPos()
	{
		oldCursorPos.x = nowCursorPos.x;
		oldCursorPos.y = nowCursorPos.y;

		::GetCursorPos(&nowCursorPos);
		ScreenToClient(gameWnd, &nowCursorPos);
	
	}

	//----------------------------------
	//�}�E�X�̑��΍��W�擾
	//����			�o�͗p�̑��΍��W
	//�߂�l		����
	//�ŏI�X�V��	2015/03/26
	//----------------------------------
	void GetRelativeCursorPos(LPPOINT pPos = nullptr)
	{
		if (pPos == nullptr)
			return;

		pPos->x = nowCursorPos.x - oldCursorPos.x;
		pPos->y = nowCursorPos.y - oldCursorPos.y;

	}

	//----------------------------------
	//�}�E�X�̐�΍��W�擾
	//����			�o�͗p�̐�΍��W
	//�߂�l		�o�͗p�̐�΍��W
	//�ŏI�X�V��	2015/03/26
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
	//��ʂ̒��S����̃x�N�g���擾
	//��ʓ��̂ǂ̈ʒu����-1~1�ŕ\��
	//����			�o�͗p�̃|�C���^
	//�߂�l		����
	//�ŏI�X�V��	2015/07/07
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

	//�R���X�g���N�^
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