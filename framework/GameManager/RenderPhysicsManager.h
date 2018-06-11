#ifndef _RenderPhysicsManager_h_
#define _RenderPhysicsManager_h_
#include"../Physics/Physics.h"
#include"../Graphics/GraphicsManager.h"

struct PhysicsObject
{
	const RigidState* state;	//剛体の状態
	TemplateDrawObject<DrawObject> drawObject;	//描画用データ
	D3DXVECTOR3 scale;

	PhysicsObject()
	{
		Reset();
	}

	void Reset()
	{
		state = nullptr;
		scale.x = scale.y = scale.z = 0.0f;
		drawObject.data.bone = nullptr;
		drawObject.data.modelID = 0;
		drawObject.data.transform = GameMath::IdentityMatrix();
	}

};

//剛体描画のマネージャ
class RenderPhysicsManager
{
private://定数定義
	const static int MaxRenderObjects = 1000;

private:
	struct PhysicsObjectList
	{
		PhysicsObject object;
		PhysicsObjectList* next;
		PhysicsObjectList* prev;

		PhysicsObjectList()
		{
			next = nullptr;
			prev = nullptr;
		}

		void Reset()
		{
			next = nullptr;
			prev = nullptr;
			object.Reset();
		}

	};

private:
	PhysicsObjectList renderObjects[MaxRenderObjects];
	int idPool[MaxRenderObjects];
	int numObjects;
	int numPool;
	PhysicsObjectList listTop; //描画用リストの先頭
	PhysicsObjectList* listEnd;

public:

	RenderPhysicsManager()
	{
		listEnd = nullptr;
		numPool = 0;
		numObjects = 0;
	}

	//剛体の情報を元に登録してIDを返す
	int Link(const RigidState& state,const Collidable& coll);


	//指定したIDのオブジェクトを解除
	void Unlink(const int objID);

	void Update();



private:
	void Link(PhysicsObjectList& link)
	{
		if (listTop.next)
		{
			listEnd->next = &link;
			listEnd = listEnd->next;
		}
		else
		{
			listTop.next = &link;
			listEnd = &link;
		}
	}

};



#endif