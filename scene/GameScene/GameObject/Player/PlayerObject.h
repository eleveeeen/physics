#pragma once

#include"../../../../framework/BaseObject/BaseObject.h"

#include"PlayerCamera.h"

//�v���C���[�N���X
//���̂ƕR�t&FPS���_�ł�����ۂ���\��
class PlayerObject :public BaseObject
{

private:

	PlayerCamera camera;//�J����

	D3DXVECTOR3 pick;	//�͂񂾍��W

	D3DXVECTOR3 oldPos; //�O�̃t���[���̍��W

	int rigidID;			//���g�ƕR�t�������̂�ID
	float cameraOffset;		//�J�����̍����̃I�t�Z�b�g

	float metallic;
	float roughness;

public:
	PlayerObject();
	~PlayerObject();

	void Update();
	void Initialize();
	

private:

	void UpdateMaterial();	//�}�e���A���̃p�����[�^�X�V
	void UpdateLight();		//���C�g�X�V
	void UpdateCamera();	//�J�����X�V
	void UpdatePick();		//�͂ޏ����X�V

};
