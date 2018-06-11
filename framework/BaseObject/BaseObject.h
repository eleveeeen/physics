//------------------------------
//�Q�[���I�u�W�F�N�g�̊��N���X
//------------------------------

#ifndef _BaseObject_h_
#define _BaseObject_h_

#include<d3dx9.h>
#include"../../framework/Graphics/GraphicsManager.h"
#include"../../framework/Graphics/CubeMap/CubeMapManager.h"
class BaseObject
{
protected://�����o�ϐ�
	TemplateDrawObject<DrawObject> data;
public:
	virtual void Update() = 0;
	//�Œ���̕`��
	virtual void Draw()
	{
		GetGraphicsPipeline().Set(data);
	}


	virtual void Finalize()
	{
		MACRO::SAFE_DELETE(data.data.bone);
		if (data.data.cubeMap != -1)
		{
			GetCubeMapManager().Destroy(data.data.cubeMap);
		}
	}


	//��]���Z�b�g
	void SetRotation(const D3DXQUATERNION& rot)
	{
		//���W���ꎞ���
		D3DXVECTOR3 temp(data.data.transform.m[3]);
		D3DXMatrixRotationQuaternion(&data.data.transform, &rot);
		SetPosition(temp);
	}

	//���W�Z�b�g
	void SetPosition(const D3DXVECTOR3& pos)
	{
		memcpy(&data.data.transform.m[3], pos, sizeof(float) * 3);
	}

	//��]&���W�Z�b�g
	void SetTransform(const D3DXMATRIX& transform)
	{
		data.data.transform = transform;
	}
	//��]&���W�Z�b�g
	void SetTransform(const D3DXQUATERNION& rot, const D3DXVECTOR3& pos)
	{
		D3DXMatrixRotationQuaternion(&data.data.transform, &rot);
		SetPosition(pos);
	}



	
protected:
	//���f����ID�o�^
	void SetModelID(int id)
	{
		data.data.modelID = id;
	}

	//�{�[���o�^
	void SetBone(BoneObjectEx& bone)
	{
		data.data.bone = &bone;
	}

	//�L���[�u�}�b�v�o�^
	void SetCubeMap(int id)
	{
		data.data.cubeMap = id;
	}



};


#endif

