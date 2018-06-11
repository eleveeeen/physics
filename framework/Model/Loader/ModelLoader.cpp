#include"ModelLoader.h"
#include"../StandardModel.h"
#include"../SkinnedModel.h"
#include"../Bone/Bone.h"
#include"../../Utility/FileOperator.h"
#include"../../ResourceManager/Material/MaterialManager.h"
#include<d3dx9.h>
namespace ModelLoader
{

	ModelBase* CreateProduct(const unsigned char type)
	{
		const unsigned char Standard = 0xcc;
		const unsigned char Skinnned = 0xbb;
		switch (type)
		{
		case Standard:
			return new StandardModel;
		case Skinnned:
			return new SkinnedModel;
		}
		return nullptr;
	}
	ModelBase* Load(std::string name, ID3D11Device* pDev)
	{

		//	fread(void *buf, size_t size, size_t num, FILE *fp)
		std::string filePath = "resource/Model/" + name;
		FileOperator file;


		if (!file.Open(filePath.c_str(), "rb"))
		{
			DEBUG::PrintfColor(EConsoleColor::H_RED, "NotFindFile\n");
			return nullptr;
		}

		unsigned char checkFormat;
		file.Read(&checkFormat, sizeof(char), 1);

		if (checkFormat != 0x4c)
		{
			DEBUG::PrintfColor(EConsoleColor::H_RED, "Don'tMatchFormat\n");
			return nullptr;

		}

		unsigned char skinnedID;
		file.Read(&skinnedID, sizeof(char), 1);

		ModelBase* model = CreateProduct(skinnedID);
		if (model == nullptr)
		{
			return nullptr;
		}

		int numVertices, numPolygons, numMaterials;
		//頂点数取得
		file.Read(&numVertices, sizeof(int), 1);
		//ポリゴン数取得
		file.Read(&numPolygons, sizeof(int), 1);
		//マテリアル数取得
		file.Read(&numMaterials, sizeof(int), 1);


		//頂点情報取得
		D3DXVECTOR3* vertexArray = new D3DXVECTOR3[numVertices];
		D3DXVECTOR2* uvArray = new D3DXVECTOR2[numVertices];

		//頂点座標
		file.Read(vertexArray, sizeof(D3DXVECTOR3), numVertices);
		model->CreateVertexBuffer(pDev, vertexArray, sizeof(D3DXVECTOR3), numVertices);

		//法線
		file.Read(vertexArray, sizeof(D3DXVECTOR3), numVertices);
		model->CreateVertexBuffer(pDev, vertexArray, sizeof(D3DXVECTOR3), numVertices);

		//uv
		file.Read(uvArray, sizeof(D3DXVECTOR2), numVertices);
		model->CreateVertexBuffer(pDev, uvArray, sizeof(D3DXVECTOR2), numVertices);


		//ボーンの情報があるなら
		if (skinnedID == 0xbb)
		{
			D3DXVECTOR4* weightArray = new D3DXVECTOR4[numVertices];

			struct uVec4
			{
				int x, y, z, w;
			};
			uVec4* boneIndexArray = new uVec4[numVertices];

			//ボーンウェイト
			file.Read(weightArray, sizeof(D3DXVECTOR4), numVertices);
			model->CreateVertexBuffer(pDev, weightArray, sizeof(D3DXVECTOR4), numVertices);


			//あるならボーンインデックス
			file.Read(boneIndexArray, sizeof(uVec4), numVertices);
			model->CreateVertexBuffer(pDev, boneIndexArray, sizeof(uVec4), numVertices);

			delete[] weightArray;
			delete[] boneIndexArray;

		}

		//マテリアル
		FaceMat* face = new FaceMat[numMaterials];
		for (int i = 0; i < numMaterials; ++i)
		{
			//面情報
			file.Read(&face[i], sizeof(int), 2);

			//マテリアル名出力
			unsigned char strLength;
			char str[256];
			SecureZeroMemory(str, 256);
			file.Read(&strLength, sizeof(char), 1);
			file.Read(str, sizeof(char), strLength);
			face[i].matIndex = MaterialManager::GetInstance().CreateMaterial(str);


		}

		model->CreateFaceMaterial(numMaterials, face);


		//インデックスバッファ
		int* index = new int[numPolygons * 3];

		file.Read(index, sizeof(int) * 3, numPolygons);

		model->CreateIndexBuffer(pDev, index, numPolygons);

		//ボーンがあるのなら


		if (skinnedID == 0xbb)
		{
			//ボーンが0の場合の例外処理は変換時に済んでいる
			int numBones = 0;
			BoneObject* bones = nullptr;

			//ボーンの数読み込み
			file.Read(&numBones, sizeof(int), 1);
			bones = new BoneObject[numBones];
			
			//配列番号格納用
			//親、子、兄弟の順で格納
			int ids[3];
			const int parent = 0;
			const int child = 1;
			const int sibling = 2;

			//ボーン格納用
			//回転情報、座標の順で格納
			//rot:xyzw,pos:xyz
			float bone[7];
			
			for (int i = 0; i < numBones; ++i)
			{
				//id取得
				file.Read(ids, sizeof(int), 3);


				bones[i].tree.parentID = ids[parent];
				bones[i].tree.childID = ids[child];
				bones[i].tree.siblingID = ids[sibling];
				bones[i].tree.myID = i;

				//初期姿勢取得
				file.Read(bone, sizeof(float), 7);
				bones[i].SetAbsoluteBone(bone);
				D3DXQuaternionNormalize(&bones[i].absolute.rot, &bones[i].absolute.rot);
			}
			//親からの相対情報を算出
			BoneObject& root = bones[0];

			root.relative = root.absolute;

			for (int i = 1; i < numBones; ++i)
			{
				BoneObject& own = bones[i];
				const BoneObject& parentBone = bones[own.tree.parentID];
			
				//相対ボーン算出				
				own.relative = own.CalcRelativeBone(parentBone.absolute);
				D3DXQuaternionNormalize(&own.relative.rot, &own.relative.rot);

			}

			for (int i = 0; i < numBones; ++i)
			{
				bones[i].DetermineOffset();
			}

			std::string boneName;
			boneName = "Bone:" + name;
			model->CreateBone(boneName, numBones, bones);
		}


		file.Close();


		delete[] vertexArray;
		delete[] uvArray;
		delete[] index;

		model->name = name;

		return model;
	}


}


