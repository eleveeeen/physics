//�Q�[�������̃��C�g�֘A�̃N���X

#ifndef _GameLight_h_
#define _GameLight_h_

#include"../Shader/ConstantBuffer/ConstantBufferManager.h"

//�F�����܂Ƃ߂��\����
struct LightColorObject
{
	D3DXVECTOR4 diffuse;	//�g�U��
	D3DXVECTOR3 ambient;	//����
	LightColorObject()
	{
		diffuse.x = 1.0f;
		diffuse.y = 1.0f;
		diffuse.z = 1.0f;
		diffuse.w = 1.0f;

		ambient.x = 1.0f;
		ambient.y = 1.0f;
		ambient.z = 1.0f;
	}

};

//���C�g�̏����܂Ƃ߂��\����
struct LightObject
{
	LightColorObject color;	//�F���
	D3DXVECTOR3 direction;	//���C�g�̕����x�N�g��

	LightObject()
	{
		direction.x = 0.0f;
		direction.y = 0.0f;
		direction.z = 1.0f;
	}

};

//���C�g�Ǘ��N���X�@�V���O���g��
class GameLight
{
private://�����o�ϐ�

public:

	static GameLight& GetInstance()
	{
		static GameLight singleton;
		return singleton;
	}

	//���C�g�̍X�V	
	//�����ʂ��Ȃ��ƃ��C�g�̏��͍X�V����Ȃ�
	void UpdateLight()
	{
		GetCBLightManager().SetLight();
	}


	void SetLightDirection(const D3DXVECTOR3& pDir)
	{
		GetCBLightManager().UpdateLightDirection(pDir);
	}


	//�S�Ă̐F�����Z�b�g
	void SetLightColor(const D3DXVECTOR4& pDiffuse, const D3DXVECTOR3& pAmbient)
	{
		GetCBLightManager().UpdateLightColor(pDiffuse, pAmbient);
	}

	void SetLightColor(const D3DXVECTOR3& pDiffuse, const D3DXVECTOR3& pAmbient)
	{
		GetCBLightManager().UpdateLightColor(pDiffuse, pAmbient);
	}


	void SetLightColor(const LightColorObject& pColor)
	{
		GetCBLightManager().UpdateLightColor(pColor.diffuse, pColor.ambient);
	}

	//�S�Ă̏����Z�b�g
	void SetLight(const LightObject& pLight)
	{
		SetLightDirection(pLight.direction);

		SetLightColor(pLight.color);
	}



private:


};

inline GameLight& GetLightMamager()
{
	return GameLight::GetInstance();
}

#endif