#pragma once
#include"DrawObject.h"
#include"../TemplatePipeline/TemplatePipeline.h"
//�`��p�p�C�v���C��
//����̓O���t�B�b�N�}�l�[�W���ȊO����͈�ؓ�����������Ȃ��悤�ɂ��Ă݂�

class GraphicsManager;

class GraphicsPipeline
{

private:

	friend class GraphicsManager;

	Template3DPipeline<DrawObject> standardPipeline;	//�{�[�������p
	Template3DPipeline<DrawObject> skinnedPipeline;		//�{�[���L��p

	Template2DPipeline<DrawFlatObject> flatPipeline;	//2D�p

public:

	//�p�C�v���C������ɂ���
	void Crear()
	{
		standardPipeline.Clear();
		skinnedPipeline.Clear();
		flatPipeline.Clear();
	}

	//3D�I�u�W�F�N�g�Z�b�g
	void Set(TemplateDrawObject<DrawObject>& obj)
	{
		obj.next = nullptr;
		if (obj.data.bone)
		{
			skinnedPipeline.Push_Back(&obj);
		}
		else
		{
			standardPipeline.Push_Back(&obj);
		}

	}

	//2D�I�u�W�F�N�g�Z�b�g
	void Set(TemplateDrawObject<DrawFlatObject>& obj)
	{
		obj.next = nullptr;
		flatPipeline.Push_Back(&obj);
	}




private:


};