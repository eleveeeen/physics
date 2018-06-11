#pragma once
#include"DrawObject.h"
#include"../TemplatePipeline/TemplatePipeline.h"
//描画用パイプライン
//今回はグラフィックマネージャ以外からは一切内部をいじれないようにしてみる

class GraphicsManager;

class GraphicsPipeline
{

private:

	friend class GraphicsManager;

	Template3DPipeline<DrawObject> standardPipeline;	//ボーン無し用
	Template3DPipeline<DrawObject> skinnedPipeline;		//ボーン有り用

	Template2DPipeline<DrawFlatObject> flatPipeline;	//2D用

public:

	//パイプラインを空にする
	void Crear()
	{
		standardPipeline.Clear();
		skinnedPipeline.Clear();
		flatPipeline.Clear();
	}

	//3Dオブジェクトセット
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

	//2Dオブジェクトセット
	void Set(TemplateDrawObject<DrawFlatObject>& obj)
	{
		obj.next = nullptr;
		flatPipeline.Push_Back(&obj);
	}




private:


};