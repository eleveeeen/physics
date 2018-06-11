#ifndef _IntersectRay_h_
#define _IntersectRay_h_

#include"shape.h"
#include"Ray.h"
#include"Box.h"
#include"Sphere.h"
#include"Capsule.h"
#include"Cylinder.h"
//オブジェとレイの交差判定の関数群

//ボックス
bool IntersectRayBox(const RayInput& ray, RayOutput& out, const Box& box, const D3DXMATRIX& transform);

//スフィア
bool IntersectRaySphere(const RayInput& ray, RayOutput& out, const Sphere& sphere, const D3DXMATRIX& transform);

//カプセル
bool IntersectRayCapsule(const RayInput& ray, RayOutput& out, const Capsule& capsule, const D3DXMATRIX& transform);

//シリンダー
bool IntersectRayCylinder(const RayInput& ray, RayOutput& out, const Cylinder& cylinder, const D3DXMATRIX& transform);





#endif