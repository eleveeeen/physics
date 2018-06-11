#pragma once
//•’Ê‚Ìƒ‚ƒfƒ‹ƒNƒ‰ƒX
#include"ModelBase.h"
#include"Mesh\Mesh.h"

struct BoneObject;
class StandardModel :public ModelBase
{
private:
	Mesh mesh;
public:

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

	int GetNumVertexBuffer()const
	{
		return mesh.GetNumVertexBuffer();
	}

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

	virtual void CreateBone(std::string boneName,int numBones, BoneObject* bones) {};


private:

};