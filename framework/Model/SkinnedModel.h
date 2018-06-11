#pragma once
//�{�[�����胂�f��
#include"ModelBase.h"
#include"Mesh\Mesh.h"
#include"Bone\ModelBone.h"
#include"../ResourceManager/Bone/BoneManager.h"

class SkinnedModel :public ModelBase
{

private:
	Mesh mesh;
	int boneID;
public://�Q�b�^�[
	virtual ~SkinnedModel()
	{

	}

	ID3D11Buffer* const* GetVertexBuffer()const
	{
		return mesh.GetVertexBuffer();
	}

	ID3D11Buffer* GetIndexBuffer()const
	{
		return mesh.GetIndexBuffer();
	}

	FaceMat* GetFaceMaterial()const
	{
		return mesh.GetFaceMaterial();
	}

	int GetNumFaceMaterial()const 
	{
		return mesh.GetNumFaceMaterial();
	}

	int GetBoneID()const
	{
		return boneID;
	}

	int GetNumVertexBuffer()const 
	{
		return mesh.GetNumVertexBuffer();
	}

public://����

	virtual int CreateVertexBuffer(ID3D11Device* dev, const void* vertices, unsigned int stride, unsigned int numVertices)
	{
		return mesh.CreateVertexBuffer(dev, vertices, stride, numVertices);
	}
	virtual int CreateIndexBuffer(ID3D11Device* dev, const void* index, int numPolygons)
	{
		return mesh.CreateIndexBuffer(dev, index, numPolygons);
	}

	virtual void CreateFaceMaterial(int numMaterials, FaceMat* mat)
	{
		mesh.CreateFaceMaterial(numMaterials, mat);
	}

	virtual void CreateBone(std::string boneName,int numBones, BoneObject* bones)
	{
		boneID = GetBoneManager().Create(boneName, numBones, bones);
	}


private:
};