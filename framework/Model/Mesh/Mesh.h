//���b�V���N���X
#pragma once 

#include<D3DX11.h>
#include<stdio.h>
#include"../../Utility/Macro.h"
#include"Vertex\MeshVertex.h"
#include"Polygon\MeshPolygon.h"



class Mesh
{
private://�����o�ϐ�
	MeshVertex vertex;

	MeshPolygon polygon;
	

public://�����o�֐�

	//���_�o�b�t�@�쐬
	int CreateVertexBuffer(ID3D11Device* dev, const void* vertices, unsigned int stride, unsigned int numVertices)
	{
		return vertex.CreateBuffer(dev, vertices, stride, numVertices);
	}

	//�C���f�b�N�X�o�b�t�@�쐬
	int CreateIndexBuffer(ID3D11Device* dev, const void* index, int numPolygons)
	{
		return polygon.CreateBuffer(dev, index, numPolygons);
	}
	//�|���S���̃}�e���A�����쐬
	void CreateFaceMaterial(int num,FaceMat* mat)
	{
		polygon.CreateFaceMat(num, mat);
		
	}

public://�Q�b�^�[

	//���_�o�b�t�@�擾
	ID3D11Buffer* const* GetVertexBuffer() const
	{
		return (vertex.vertexBuffer);
	}

	//���_�o�b�t�@���擾
	int GetNumVertexBuffer()const
	{
		return vertex.numBuffers;
	}

	//�C���f�b�N�X�o�b�t�@�擾
	ID3D11Buffer* GetIndexBuffer()const
	{
		return polygon.indexBuffer;
	}

	//�|���S���̃}�e���A�����擾
	FaceMat* GetFaceMaterial()const
	{
		return polygon.face;
	}

	//�|���S���̃}�e���A�����擾
	int GetNumFaceMaterial()const
	{
		return polygon.numMaterials;
	}


};
