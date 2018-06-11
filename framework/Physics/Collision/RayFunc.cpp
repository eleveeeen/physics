#include"RayFunc.h"
#include"IntersectRay.h"


bool IntersectRayFuncBox(
	const RayInput& ray, RayOutput& out,
	const Shape& shape, const D3DXMATRIX& transform)
{
	return IntersectRayBox(ray, out, shape.GetBox(), transform);

}


bool IntersectRayFuncSphere(
	const RayInput& ray, RayOutput& out,
	const Shape& shape, const D3DXMATRIX& transform)
{
	return IntersectRaySphere(ray, out, shape.GetSphere(), transform);

}


bool IntersectRayFuncCylinder(
	const RayInput& ray, RayOutput& out,
	const Shape& shape, const D3DXMATRIX& transform)
{
	return IntersectRayCylinder(ray, out, shape.GetCylinder(), transform);

}


bool IntersectRayFuncCapsule(
	const RayInput& ray, RayOutput& out,
	const Shape& shape, const D3DXMATRIX& transform)
{
	return IntersectRayCapsule(ray, out, shape.GetCapsule(), transform);

}


//Ray Intersection Function Table
IntersectRayFunc funcTblIntersectRay[eShapeType::eShapeCount] =
{
	IntersectRayFuncSphere,		//‹…
	IntersectRayFuncBox,		//” 
	IntersectRayFuncCapsule,	//ƒJƒvƒZƒ‹
	IntersectRayFuncCylinder,	//‰~’Œ
};



IntersectRayFunc GetIntersectRayFunc(UInt8 shapeType)
{
	return funcTblIntersectRay[shapeType];

}


int SetIntersectRayFunc(UInt8 shapeType, IntersectRayFunc func)
{

	if (shapeType >= eShapeType::eShapeCount)
		return -1;

	funcTblIntersectRay[shapeType] = func;

	return 0;
}


