#pragma once

#include"../../../../framework/BaseObject/BaseObject.h"

//�V�[�����ɒu����[������̃N���X
//���O�h�[��
class Yuuki :public BaseObject
{
public:

	Yuuki();
	~Yuuki();
	
	//���炭�����ōs���̂�DrawCall����
	void Update();

	//������
	void Initialize(const D3DXVECTOR3& initPos = D3DXVECTOR3(0, 0, 0),	   //�����̍��W
		const D3DXQUATERNION& initRot = D3DXQUATERNION(0, 0, 0, 1),		   //�����̎p��
		bool useCubemap = false);										   //�L���[�u�}�b�v���g�p���邩�ǂ��� false�Ȃ�X�t�B�A�}�b�v���g�p

private:
	void InitRagdoll();	//���O�h�[���p�̏�񏉊���


};