//------------------
//���C�u����
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
//�C���N���[�h
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


//�G���g���[�|�C���g windows ver 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpGmdLine, int nCmdShow)
{

#ifdef _DEBUG


	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	//���������[�N�ꏊ����p
//	_crtBreakAlloc = 570;

#endif



	DEBUG::InitDebugConsole();
	DEBUG::PrintfColor(EConsoleColor::H_WHITE, "�Z�b�g�A�b�v�J�n\n");


	//�Q�[���S�̂̏�����
	GameMain::GetInstance()->Setup(hInstance,720,1080);
	GameMain::GetInstance()->SetScene(new GameScene);

	DEBUG::PrintfColor(EConsoleColor::H_WHITE, "�Q�[���J�n\n");

	//�Q�[���J�n
	return GameMain::GetInstance()->Run();

	
}

//----------------------------------------------------------------------------------------------------------//
//�t���[�����[�N���g�p���Ă̔��ȓ_��																		//
//----------------------------------------------------------------------------------------------------------//
//�e���v���[�g���̏璷�ɂȂ�悤�Ȍ^���̂����ŏ�����p�ӂ���Ă�����̂�typedef�ŗp�ӂ��悤���ȂƎv�����B	//
//�V�F�[�_�[���肪�G����������S�ʓI�Ɍ��������K�v���ȂƎv�����B											//
//���I�V�F�[�_�����N������	��������																		//
//�E���_�V�F�[�_�̃{�[���̏���																				//
//�E�s�N�Z���V�F�[�_�̏���																					//
//�`��O�ɃV�F�[�_�[���̃\�[�g���K�v���ȂƎv�����B															//
//PBR�ɂ���ɂ��Ă������A������ŕ�����K�v������Ǝv������													//
//���f���̕����Œ��_��S�Č������Ă�������ǌ��������Ƀ}�e���A���P�ʂŊǗ����Ă������C������				//
//		���ヂ�f�����ł͂Ȃ��}�e���A�����̕`��ɐ؂�ւ��邽��												//
//		���̏ꍇ�P�ꃂ�f�����̂Ńp�[�c�̐؂�ւ��������ł���悤�ɂɂȂ�C������							//
//GameMath�̒��g���킯�Ďg���₷������K�v�����邩�ȂƎv����												//
//		�Œ�ł��x�N�g���A�N�H�[�^�j�I���A�s��A���̑��ł킯��												//
//���f�����e�L�X�g�`������o�C�i���`���ɂ���	�ύX����													//
//�}�e���A���Ǘ��N���X���ق���					�쐬����													//
//���}�b�v�̊Ǘ����肪�ˊэH���ɂȂ������玟��ł͏C��������												//
//�����_�����O�������f�B�t�@�[�h��t�H���[�h�v���X�ɂ��悤���ȂƎv�����B�i�D��x��j						//
//�t�@�T�[�h��A�_�v�^�[��p����GameObject������g���₷���`�ɂ�����										//
//�萔�o�b�t�@��MapUnmap���{�g���l�b�N�ɂȂ�̂ł͂Ȃ����Ǝw�E���ꂽ���玟��C��							//
//----------------------------------------------------------------------------------------------------------//
