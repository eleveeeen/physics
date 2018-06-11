#ifndef _IntersectRay_h_
#define _IntersectRay_h_

#include"shape.h"
#include"Ray.h"
#include"Box.h"
#include"Sphere.h"
#include"Capsule.h"
#include"Cylinder.h"
//�I�u�W�F�ƃ��C�̌�������̊֐��Q

//�{�b�N�X
bool IntersectRayBox(const RayInput& ray, RayOutput& out, const Box& box, const D3DXMATRIX& transform);

//�X�t�B�A
bool IntersectRaySphere(const RayInput& ray, RayOutput& out, const Sphere& sphere, const D3DXMATRIX& transform);

//�J�v�Z��
bool IntersectRayCapsule(const RayInput& ray, RayOutput& out, const Capsule& capsule, const D3DXMATRIX& transform);

//�V�����_�[
bool IntersectRayCylinder(const RayInput& ray, RayOutput& out, const Cylinder& cylinder, const D3DXMATRIX& transform);





#endif