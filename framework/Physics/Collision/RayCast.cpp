#include"RayCast.h"
#include"ShapeIterator.h"
#include"../Base/Common.h"
#include"../../Utility/GameMath.h"
#include"RayFunc.h"
#include"../../DebugConsole/DebugConsole.h"
//前方の判定
void RayTraverseForward(
	const RayInput& ray, RayOutput& out, const Aabb16& rayAABB,
	BroadphaseProxy* proxies, int numProxies,
	RigidState* offsetRigidStates,
	Collidable* offsetCollidables,
	int axis, const D3DXVECTOR3& center, const D3DXVECTOR3& half)
{

	DEBUG::Printf("raystart(%f,%f,%f)\n", ray.startPos.x, ray.startPos.y, ray.startPos.z);
	DEBUG::Printf("rayend(%f,%f,%f)\n", ray.startPos.x + ray.dir.x, ray.startPos.y + ray.dir.y, ray.startPos.z + ray.dir.z);

	//ソートされているはずだから手前から順に判定
	for (int i = 0; i < numProxies; ++i)
	{
		BroadphaseProxy& proxy = proxies[i];
		
		//オブジェクトが自身より確実に奥にいる場合終了
		if (GetXYZMax(rayAABB, axis) < GetXYZMin(proxy, axis))
			return;

		D3DXVECTOR3 boundOnRay = ray.startPos + out.variable*ray.dir;

		D3DXVECTOR3 aabbMin = GameMath::ConvertCoordLocalToWorld(
			GameMath::GetVec3(GetXMin(proxy), GetYMin(proxy), GetZMin(proxy)),
			center, half);
		D3DXVECTOR3 aabbMax = GameMath::ConvertCoordLocalToWorld(
			GameMath::GetVec3(GetXMax(proxy), GetYMax(proxy), GetZMax(proxy)),
			center, half);


		if (GameMath::GetElem(boundOnRay, axis) < GameMath::GetElem(aabbMin,axis))
			return;

		//手前にいるから次へ
		if (GetXYZMax(proxy, axis) < GetXYZMin(rayAABB, axis))
			continue;


		UInt16 rigidbodyID = GetObjectID(proxy);
		UInt32 contactFillterSelf = GetSelf(proxy);
		UInt32 contactFilterTarget = GetTarget(proxy);
	
		//衝突判定
		float t = 1.0f;
		if ((ray.contactFilterSelf&contactFilterTarget) &&
			(ray.contactFilterTarget&contactFillterSelf) &&
			TestAabb(rayAABB, proxy) &&
			IntersectRayAABB(ray.startPos, ray.dir, (aabbMax + aabbMin)*0.5f, (aabbMax - aabbMin)*0.5f, t) &&
			t < out.variable)
		{
			DEBUG::Printf("%d\n", rigidbodyID);
			RigidState& state = offsetRigidStates[rigidbodyID];
			Collidable& coll = offsetCollidables[rigidbodyID];
			D3DXMATRIX transform = GameMath::Get4x4(state.GetOrientation(), state.GetPosition());

			RayOutput tOut = out;
			ShapeIterator ite(coll);
			for (UInt32 j = 0; j < coll.GetNumShapes(); ++j, ++ite)
			{
				const Shape& shape = *ite;
				D3DXMATRIX shapeTrns = shape.GetOffsetTransform()*transform;

				if (GetIntersectRayFunc(shape.GetType())(ray, tOut, shape, shapeTrns) && tOut.variable < out.variable)
				{
					out = tOut;
					out.shapeID = j;
 					out.objectID = rigidbodyID;
				}

			}

		}

	}

}


//後方の判定
void RayTraverseBackward(const RayInput& ray, RayOutput& out, const Aabb16& rayAABB,
	BroadphaseProxy* proxies, int numProxies,
	RigidState* offsetRigidStates,
	Collidable* offsetCollidables,
	int axis, const D3DXVECTOR3& center, const D3DXVECTOR3& half)
{

	DEBUG::Printf("raystart(%f,%f,%f)\n", ray.startPos.x, ray.startPos.y, ray.startPos.z);
	DEBUG::Printf("rayend(%f,%f,%f)\n", ray.startPos.x + ray.dir.x, ray.startPos.y + ray.dir.y, ray.startPos.z + ray.dir.z);


	//ソートされているはずだから奥から順に判定
	for (int i = numProxies - 1; i >= 0; --i)
	{
		BroadphaseProxy& proxy = proxies[i];

		//オブジェクトが自身より確実に手前にいる場合終了
		if (GetXYZMax(proxy, axis) < GetXYZMin(rayAABB, axis))
			return;

		D3DXVECTOR3 boundOnRay = ray.startPos + out.variable*ray.dir;

		D3DXVECTOR3 aabbMin = GameMath::ConvertCoordLocalToWorld(
			GameMath::GetVec3(GetXMin(proxy), GetYMin(proxy), GetZMin(proxy)),
			center, half);
		D3DXVECTOR3 aabbMax = GameMath::ConvertCoordLocalToWorld(
			GameMath::GetVec3(GetXMax(proxy), GetYMax(proxy), GetZMax(proxy)),
			center, half);


		if (GameMath::GetElem(aabbMax, axis) < GameMath::GetElem(boundOnRay, axis))
			return;

		if (GetXYZMax(rayAABB, axis) < GetXYZMin(proxy, axis))
			continue;

		UInt16 rigidbodyID = GetObjectID(proxy);
		UInt32 contactFillterSelf = GetSelf(proxy);
		UInt32 contactFilterTarget = GetTarget(proxy);

		float t = 1.0f;
#ifdef _DEBUG
		bool tes1 = (ray.contactFilterSelf&contactFilterTarget);
		bool tes2 = (ray.contactFilterTarget&contactFillterSelf);
		bool tes3 = TestAabb(rayAABB, proxy);
		bool tes4 = IntersectRayAABB(ray.startPos, ray.dir, (aabbMax + aabbMin)*0.5f, (aabbMax - aabbMin)*0.5f, t);
		bool tes5 = t < out.variable;

		if(tes1 && tes2 && tes3 && tes4 && tes5)
#else 
		//衝突判定
		if ((ray.contactFilterSelf&contactFilterTarget) &&
			(ray.contactFilterTarget&contactFillterSelf) &&
			TestAabb(rayAABB, proxy) &&
			IntersectRayAABB(ray.startPos, ray.dir, (aabbMax + aabbMin)*0.5f, (aabbMax - aabbMin)*0.5f, t) &&
			t < out.variable)
#endif
		{
			RigidState& state = offsetRigidStates[rigidbodyID];
			Collidable& coll = offsetCollidables[rigidbodyID];
			D3DXMATRIX transform = GameMath::Get4x4(state.GetOrientation(), state.GetPosition());

			RayOutput tOut = out;
			ShapeIterator ite(coll);
			for (UInt32 j = 0; j < coll.GetNumShapes(); ++j, ++ite)
			{
				DEBUG::Printf("%d\n", rigidbodyID);
				const Shape& shape = *ite;
				D3DXMATRIX shapeTrns = shape.GetOffsetTransform() *transform;

				if (GetIntersectRayFunc(shape.GetType())(ray, tOut, shape, shapeTrns) && tOut.variable < out.variable)
				{
					out = tOut;
					out.shapeID = j;
					out.objectID = rigidbodyID;
				}

			}

		}

	}
}
//レイキャストのエントリ
void CastRay(const RayInput& rayInput, RayOutput& rayOutput, const RayCastParam& param)
{
	
	BroadphaseProxy* proxies[] =
	{
		param.proxiesX,
		param.proxiesY,
		param.proxiesZ,
		param.proxiesXb,
		param.proxiesYb,
		param.proxiesZb
	};

	rayOutput.variable = 1.0f;
	rayOutput.contactFlag = false;


	D3DXVECTOR3 checkAxisVec = GameMath::AbsPerElem(rayInput.dir);
	
	//checkAxisVecで一番要素が小さい軸をしらべる
	int axis = ((checkAxisVec.x) < (checkAxisVec.y)) ? 0 : 1;
	
	if (checkAxisVec.z < GameMath::GetElem(checkAxisVec, axis))
	{
		axis = 2;
	}

	//レイのAABB
	D3DXVECTOR3 p1 = rayInput.startPos;
	D3DXVECTOR3 p2 = rayInput.startPos + rayInput.dir;
	D3DXVECTOR3 rayMin, rayMax;

	GameMath::ConvertCoordWorldToLocal(param.rangeCenter, param.rangeExtent,
		GameMath::MinPerElem(p1, p2), GameMath::MaxPerElem(p1, p2), rayMin, rayMax);

	Aabb16 rayAABB;
	SetXMin(rayAABB, rayMin.x);
	SetXMax(rayAABB, rayMax.x);
	SetYMin(rayAABB, rayMin.y);
	SetYMax(rayAABB, rayMax.y);
	SetZMin(rayAABB, rayMin.z);
	SetZMax(rayAABB, rayMax.z);

	int sign = GameMath::GetElem(rayInput.dir, axis) < 0.0f ? -1 : 1;//探索方向

	//aabb探索開始
	if (sign > 0)
	{
		RayTraverseForward(
			rayInput, rayOutput, rayAABB,
			proxies[axis], param.numProxies,
			param.offsetRigidStates, param.offsetCollidables,
			axis, param.rangeCenter, param.rangeExtent);
	}
	else
	{
		RayTraverseBackward(
			rayInput, rayOutput, rayAABB,
			proxies[axis + 3], param.numProxies,
			param.offsetRigidStates, param.offsetCollidables,
			axis, param.rangeCenter, param.rangeExtent);
	}

}