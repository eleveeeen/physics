#include"RenderPhysicsManager.h"

//剛体のデータをもとにスケール情報の設定
inline void SetScale(D3DXVECTOR3& scale,const Collidable& coll)
{
	const Shape& shape = coll.GetShape(0);
	const int shapeID = coll.GetShapeID(0);

	switch (shapeID)
	{
	case 0://球
		scale.x = scale.y = scale.z = shape.GetSphere().radius;
		break;
	case 1://箱
		scale = shape.GetBox().half;
		break;
	case 2://カプセル
	{
		const Capsule& cap = shape.GetCapsule();
		scale.x = cap.halfLength;
		scale.y = scale.z = cap.radius;
		break;
	}
	case 3://円柱
	{
		const Cylinder& cyl = shape.GetCylinder();
		scale.x = cyl.halfLength;
		scale.y = scale.z = cyl.radius;
		break;
	}
	}
}

//剛体の情報を元にワールド行列算出
inline D3DXMATRIX CalcWorldMatrix(const D3DXQUATERNION& rot,const D3DXVECTOR3& pos,const D3DXVECTOR3& scale)
{
	D3DXMATRIX out;
	D3DXMatrixRotationQuaternion(&out, &rot);
	

	//ローカルのX方向のスケール
	out._11 *= scale.x;
	out._12 *= scale.x;
	out._13 *= scale.x;

	//ローカルのY方向のスケール
	out._21 *= scale.y;
	out._22 *= scale.y;
	out._23 *= scale.y;

	//ローカルのZ方向のスケール
	out._31 *= scale.z;
	out._32 *= scale.z;
	out._33 *= scale.z;

	//移動情報
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

	//プールにIDを入れてインクリメント
	idPool[numPool++] = objID;

	obj.object.Reset();
	PhysicsObjectList* prev = renderObjects[objID].prev;
	PhysicsObjectList* next = renderObjects[objID].next;

	//先頭にダミーデータがあるからnull判定は必要ないはず
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