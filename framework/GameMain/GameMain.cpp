#include"GameMain.h"
#include"../Utility/Macro.h"
#include"../Input/CInputKeyboard.h"
#include"../Input/CInputMouse.h"
#include"../Utility/FpsControl.h"
#include"../DebugConsole/DebugConsole.h"
#include"../Shader/Shader/ShaderManager.h"
#include"../Graphics/GraphicsManager.h"
#include"../GameManager/GameLight.h"

//�R���X�g���N�^
GameMain::GameMain()
{
	pWindow = nullptr;


	windowMode = true;	//�Ƃ肠�����E�B���h�E�\��
	exitFlag = false;
}

GameMain::~GameMain()
{
	MACRO::SAFE_DELETE(pWindow);
	dx11App.Finalize();

	//�t���X�N���[���������ꍇ�E�B���h�E�ɖ߂��ĉ��
	if (!windowMode)
	{

	}

}



//�Z�b�g�A�b�v
HRESULT GameMain::Setup(HINSTANCE hInst,float windowHeight,float windowWidth)
{

	FPSControl()->Create();
	pWindow = new CWindow(hInst, nullptr, windowWidth, windowHeight);
	InputMouseManager()->SetWindowHandle(pWindow->GethWnd());
	InputMouseManager()->SetWindowSize(windowWidth, windowHeight);
	//11�̏���������
	dx11App.Initialize(pWindow->GethWnd(),windowWidth,windowHeight);
	//11�̕`��܂��̏���������
	GraphicsManager::GetInstance().Initialize(dx11App.GetDevice(), dx11App.GetDeviceContext(), dx11App.GetSwapChain(), windowWidth, windowHeight);
	
	//�ˉe�s��̃Z�b�g

	D3DXMATRIX ortho;
	D3DXMatrixIdentity(&ortho);
	ortho._11 = 2.0f / windowWidth;
	ortho._22 = 2.0f / windowHeight;
	ortho._33 = -1;

	D3DXMATRIX pers;
	D3DXMatrixPerspectiveFovLH(&pers, 3.1415f / 4.0f, windowWidth / windowHeight, 0.1f, 10000.0f);

	GetCBMatrixManager().SetProjMatrix(ortho, pers);
	
	//�f�t�H���g�̃��C�g�̃Z�b�g
	LightObject defaultLight;

	//���C�g�̐F���
	defaultLight.color.diffuse.x = 0.8f;
	defaultLight.color.diffuse.y = 0.8f;
	defaultLight.color.diffuse.z = 0.8f;
	defaultLight.color.diffuse.w = 1.0f;
	defaultLight.color.ambient.x = 0.2f;
	defaultLight.color.ambient.y = 0.2f;
	defaultLight.color.ambient.z = 0.2f;

	//���C�g�̕���
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

//�Q�[�����[�v�J�n

int GameMain::Run()
{

	while (!exitFlag)
	{
		//�E�B���h�E�X�V
		TransactMessage();
		
		if (FPSControl()->GetUpdate())
		{
			//�L�[�{�[�h�X�V
			InputKeyManager()->Update();
			//�}�E�X�X�V
			InputMouseManager()->Update();

			//�Q�[���X�V
			sceneManager.Execute();

			//FPS�o��
			char titlebar[100];
			sprintf(titlebar,"%s-FPS[%.1f]",WINDOW_TITLE,FPSControl()->GetAverageFps());
			pWindow->UpdateWindowsText(titlebar);
			//FPS����
			FPSControl()->CountFPS(60);
			FPSControl()->UpdateStart();

		}


		//�G�X�P�[�v�L�[���������E�B���h�E���j�����ꂽ��I��
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
	//���b�Z�[�W��o��
	if (PeekMessage(&msg, pWindow->GethWnd(), 0, 0, PM_REMOVE))
	{
		//���b�Z�[�W����
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}