#ifndef _RenderPhysicsManager_h_
#define _RenderPhysicsManager_h_
#include"../Physics/Physics.h"
#include"../Graphics/GraphicsManager.h"

struct PhysicsObject
{
	const RigidState* state;	//���̂̏��
	TemplateDrawObject<DrawObject> drawObject;	//�`��p�f�[�^
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

//���̕`��̃}�l�[�W��
class RenderPhysicsManager
{
private://�萔��`
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
	PhysicsObjectList listTop; //�`��p���X�g�̐擪
	PhysicsObjectList* listEnd;

public:

	RenderPhysicsManager()
	{
		listEnd = nullptr;
		numPool = 0;
		numObjects = 0;
	}

	//���̂̏������ɓo�^����ID��Ԃ�
	int Link(const RigidState& state,const Collidable& coll);


	//�w�肵��ID�̃I�u�W�F�N�g������
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