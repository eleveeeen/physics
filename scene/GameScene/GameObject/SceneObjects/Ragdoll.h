#pragma once

#include"../../../../framework/BaseObject/BaseObject.h"

//���O�h�[���̃N���X
//�p�����[�^�͂�[������N���X�̎g���܂킵
class Ragdoll : public BaseObject
{
public:

	Ragdoll();
	~Ragdoll();

	void Update() {};
	//������
	void Initialize(const D3DXVECTOR3& initPos = D3DXVECTOR3(0, 0, 0),		//�����̍��W
		const D3DXQUATERNION& initRot = D3DXQUATERNION(0, 0, 0, 1));		//�����̎p��

private:
	void InitRagdoll(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot);

};