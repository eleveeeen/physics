#pragma once


#include"../../../../framework/BaseObject/BaseObject.h"

//�h�A�̃N���X
class Door:public BaseObject
{

private://�����o�ϐ�
	int rigidBox;		//�h�A�����̍��̂�ID
	int rigidCapsule;	//�������̍��̂�ID

	TemplateDrawObject<DrawObject> pillar; //�������̃��f��

public://�����o�֐�

	Door();
	~Door();

	void Initialize(const D3DXVECTOR3& initPos = D3DXVECTOR3(0, 0, 0),	//�����̍��W
		const D3DXQUATERNION& initRot = D3DXQUATERNION(0, 0, 0, 1),		//�����̎p��
		const float lowerAngle = -180.0f,const float upperAngle = 180.0f);	//�h�A�̉�]�͈�(-180~180)

	void Update();
	void Draw();

private://�����o�ϐ�

	//���̐ݒ�
	void InitRigidBody(const D3DXVECTOR3& pos, const D3DXQUATERNION& rot,
		const float lowerAngle, const float upperAngle);

};