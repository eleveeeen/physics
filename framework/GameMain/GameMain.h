#ifndef _GameMain_h_
#define _GameMain_h_

//�C���N���[�h
#include"../Window/CWindow.h"
#include"../SceneManager/SceneManager.h"
#include"../DirectX11/DirectX11Application.h"


class GameMain
{

private://�����o�ϐ�
	LPWINDOW pWindow;	//�Q�[���̃��C���E�B���h�E
	SceneManager sceneManager;	//�V�[���Ǘ�
	DirectX11Application dx11App;	//DirectX11�̂���

	bool windowMode;	//�E�B���h�E���[�h
	bool exitFlag;		//�Q�[�����[�v����̗��E�p
public://�����o�ϐ�

public:

	~GameMain();

	static GameMain* GetInstance()
	{
		static GameMain instance;
		return &instance;
	}

	HRESULT Setup(HINSTANCE hInst, float windowHeight = 480, float windowWidth = 640);//�Z�b�g�A�b�v����
	void SetScene(BaseScene* pScene);
	int Run();//�Q�[�����[�v�J�n


private:
	void TransactMessage();

	GameMain();

};



#endif