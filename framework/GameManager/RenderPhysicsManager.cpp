#include"RenderPhysicsManager.h"

//���̂̃f�[�^�����ƂɃX�P�[�����̐ݒ�
inline void SetScale(D3DXVECTOR3& scale,const Collidable& coll)
{
	const Shape& shape = coll.GetShape(0);
	const int shapeID = coll.GetShapeID(0);

	switch (shapeID)
	{
	case 0://��
		scale.x = scale.y = scale.z = shape.GetSphere().radius;
		break;
	case 1://��
		scale = shape.GetBox().half;
		break;
	case 2://�J�v�Z��
	{
		const Capsule& cap = shape.GetCapsule();
		scale.x = cap.halfLength;
		scale.y = scale.z = cap.radius;
		break;
	}
	case 3://�~��
	{
		const Cylinder& cyl = shape.GetCylinder();
		scale.x = cyl.halfLength;
		scale.y = scale.z = cyl.radius;
		break;
	}
	}
}

//���̂̏������Ƀ��[���h�s��Z�o
inline D3DXMATRIX CalcWorldMatrix(const D3DXQUATERNION& rot,const D3DXVECTOR3& pos,const D3DXVECTOR3& scale)
{
	D3DXMATRIX out;
	D3DXMatrixRotationQuaternion(&out, &rot);
	

	//���[�J����X�����̃X�P�[��
	out._11 *= scale.x;
	out._12 *= scale.x;
	out._13 *= scale.x;

	//���[�J����Y�����̃X�P�[��
	out._21 *= scale.y;
	out._22 *= scale.y;
	out._23 *= scale.y;

	//���[�J����Z�����̃X�P�[��
	out._31 *= scale.z;
	out._32 *= scale.z;
	out._33 *= scale.z;

	//�ړ����
	out._41 = pos.x;
	out._42 = pos.y;
	out._43 = pos.z;

	return out;
}



int RenderPhysicsManager::Link(const RigidState& state, const Collidable& coll)
{
	//if (numPool == 0)
	//{
	//	objID = numObjects;
	//	++numObjects;
	//}
	//else
	//{
	//	--numPool;
	//	objID = idPool[numPool];
	//}
	int objID = (numPool == 0) ? numObjects++ : idPool[-numPool];

	PhysicsObjectList& obj = renderObjects[objID];
	obj.Reset();

	SetScale(obj.object.scale, coll);

	obj.object.state = &state;

	obj.object.drawObject.data.modelID = coll.GetShapeID(0);

	Link(obj);

	return objID;
}




void RenderPhysicsManager::Unlink(const int objID)
{
	PhysicsObjectList& obj = renderObjects[objID];

	//�v�[����ID�����ăC���N�������g
	idPool[numPool++] = objID;

	obj.object.Reset();
	PhysicsObjectList* prev = renderObjects[objID].prev;
	PhysicsObjectList* next = renderObjects[objID].next;

	//�擪�Ƀ_�~�[�f�[�^�����邩��null����͕K�v�Ȃ��͂�
	prev->next = next;

	if (next)
		next->prev = prev;


}

void RenderPhysicsManager::Update()
{
	for (auto now = this->listTop.next; now; now = now->next)
	{
		const RigidState& state = *now->object.state;
		now->object.drawObject.data.transform =
			CalcWorldMatrix(state.GetOrientation(), state.GetPosition(), now->object.scale);
		GetGraphicsPipeline().Set(now->object.drawObject);

	}



}