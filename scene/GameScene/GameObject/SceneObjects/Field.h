#pragma once
#include"../../../../framework/BaseObject/BaseObject.h"

//���̂�����߂�t�B�[���h
//8�̃J�v�Z���ō쐬
class GameField :public BaseObject
{
private://�����o�ϐ�
	TemplateDrawObject<DrawObject> capsule[8];
	int rigidID[8];
public://�����o�֐�
	GameField();
	~GameField();

	void Initialize();

	void Update();

private://�����o�֐�

	void InitRigidBody();//���̐ݒ�


};