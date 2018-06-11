//----------------------------------------------------
// �L�[�{�[�h�֘A�̃w�b�_�[
//----------------------------------------------------

#ifndef _CInputKeyboard_h_
#define _CInputKeyboard_h_

//-------------------------------------------
//�C���N���[�h
//-------------------------------------------
#include<Windows.h>

//-------------------------------------------
//�萔��`
//-------------------------------------------
enum//���z�L�[�R�[�h�̃A���t�@�x�b�g�Ɛ����������Ȃ�����p��
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
//�L�[�{�[�h���\����
//�ŏI�X�V��	2015/03/26
//-------------------------------------------------
 struct SKeyboard
{
	BYTE hold[256];
	BYTE trigger[256];
	BYTE release[256];
	BYTE vec[2];//�㉺���E�̌X�����
};

//-------------------------------------------------
//�L�[�{�[�h���̓N���X�@�V���O���g��
//�ŏI�X�V��	2015/03/26
//-------------------------------------------------
class CInputKey
{
private://�萔��`

	//�X�e�B�b�N�݂����Ɉ������̂��ꂼ��̃L�[
	const static int VEC_LEFT	= VK_A;//��
	const static int VEC_RIGHT	= VK_D;//�E
	const static int VEC_UP		= VK_W;//��
	const static int VEC_DOWN	= VK_S;//��

private://�����o�ϐ�
	SKeyboard keyState;//�L�[�{�[�h���

public:

	//�f�X�g���N�^
	~CInputKey(){};//���炭����

	//----------------------------------
	//�C���X�^���X�Ăяo��
	//����			����
	//�߂�l		���g�̃|�C���^
	//�ŏI�X�V��	2015/03/26
	//----------------------------------
	static CInputKey* GetInstance()
	{
		static CInputKey singleton;
		return &singleton;
	}

	//----------------------------------
	//�L�[�{�[�h�̍X�V
	//����			����
	//�߂�l		����
	//�ŏI�X�V��	2015/03/26
	//----------------------------------
	void Update()
	{
		BYTE oldState[256];

		//�ߋ��̃L�[�����R�s�[
		memcpy(oldState, keyState.hold, sizeof(BYTE) * 256);
		ZeroMemory(keyState.hold, sizeof(BYTE) * 256);

		//�L�[�{�[�h�̏�Ԃ��擾
		GetKeyboardState(keyState.hold);

		//�R���p�C�����񂪊撣���ĕ��񏈗������Ƃ�����

		//�z�[���h�L�[���X�V
		for (int i = 0; i < 256; ++i)
		{
			keyState.hold[i] = keyState.hold[i] & 0x80;
		}

		//�g���K�[�L�[���X�V
		for (int i = 0; i < 256; ++i)
		{
			keyState.trigger[i] = (keyState.hold[i] ^ oldState[i]) & keyState.hold[i];
		}

		//�����[�X�L�[���X�V	
		for (int i = 0; i < 256; ++i)
		{
			keyState.release[i] = (keyState.hold[i] ^ oldState[i]) & oldState[i];
		}

		//�㉺�̌X���X�V 
		keyState.vec[0] = (keyState.hold[VEC_UP] >> 7) - (keyState.hold[VEC_DOWN] >> 7);
		//���E�̌X���X�V
		keyState.vec[1] = (keyState.hold[VEC_RIGHT] >> 7) - (keyState.hold[VEC_LEFT] >> 7);
	}

	//----------------------------------
	//�z�[���h���擾
	//����			�L�[�R�[�h
	//�߂�l		�L�[�̏��
	//�ŏI�X�V��	2015/03/26
	//----------------------------------
	BYTE GetKeyHold(BYTE keyCode)
	{
		return keyState.hold[keyCode];
	}

	//----------------------------------
	//�g���K�[���擾
	//����			�L�[�R�[�h
	//�߂�l		�L�[�̏��
	//�ŏI�X�V��	2015/03/26
	//----------------------------------
	BYTE GetKeyTrigger(BYTE keyCode)
	{
		return keyState.trigger[keyCode];
	}

	//----------------------------------
	//�����[�X���擾
	//����			�L�[�R�[�h
	//�߂�l		�L�[�̏��
	//�ŏI�X�V��	2015/03/26
	//----------------------------------
	BYTE GetKeyRelease(BYTE keyCode)
	{
		return keyState.release[keyCode];
	}

	//----------------------------------
	//���E�̃L�[���x�N�g����
	//����			����
	//�߂�l		-1~1��X���̃x�N�g��
	//�ŏI�X�V��	2015/05/07
	//----------------------------------
	signed char GetKeyVecX()
	{
		return keyState.vec[1];
	}


	//----------------------------------
	//�㉺�̃L�[���x�N�g����
	//����			����
	//�߂�l		-1~1��Y���̃x�N�g��
	//�ŏI�X�V��	2015/05/07
	//----------------------------------

	signed char GetKeyVecY()
	{
		return keyState.vec[0];
	}



private:
	//�R���X�g���N�^
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