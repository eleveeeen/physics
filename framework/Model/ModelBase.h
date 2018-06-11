#pragma once
//モデルのベースクラス

#include<string>
#include<D3DX11.h>

struct BoneObject;
struct FaceMat;
class ModelBase
{

public:

	std::string name;
	virtual ~ModelBase()
	{
	
	}

	virtual ID3D11Buffer* const* GetVertexBuffer() const{ return nullptr; }
	virtual ID3D11Buffer* GetIndexBuffer()const { return nullptr; }
	virtual FaceMat* GetFaceMaterial()const{ return nullptr; }
	virtual int GetNumFaceMaterial()const { return 0; }
	
	virtual int GetBoneID() const { return -1; }
	virtual int GetNumVertexBuffer()const { return 0; }


public://純粋仮想関数
	virtual int CreateVertexBuffer(ID3D11Device* dev, const void* vertices, unsigned int stride, unsigned int numVertices) = 0;
	virtual int CreateIndexBuffer(ID3D11Device* dev, const void* index, int numPolygons) = 0;
	virtual void CreateFaceMaterial(int numMaterials, FaceMat* mat) = 0;
	virtual void CreateBone(std::string boneName,int numBones, BoneObject* bones) = 0;
	
};

