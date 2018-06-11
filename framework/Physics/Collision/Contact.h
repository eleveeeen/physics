#ifndef _Contact_h_
#define _Contact_h_

#include"../Base/Common.h"


float ContactBoxBox(
	D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
	void* shapeA, const D3DXMATRIX& transformA,
	void* shapeB, const D3DXMATRIX& transformB,
	float distanceThreshold = FLT_MAX);


float ContactBoxCapsule(
	D3DXVECTOR3& normal,D3DXVECTOR3& pointA,D3DXVECTOR3& pointB,
	void* shapeA, const D3DXMATRIX& transformA,
	void* shapeB, const D3DXMATRIX& transformB,
	float distanceThreshold = FLT_MAX);

float ContactBoxSphere(
	D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
	void* shapeA, const D3DXMATRIX& transformA,
	void* shapeB, const D3DXMATRIX& transformB,
	float distanceThreshold = FLT_MAX);

float ContactCapsuleCapsule(
	D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
	void* shapeA, const D3DXMATRIX& transformA,
	void* shapeB, const D3DXMATRIX& transformB,
	float distanceThreshold = FLT_MAX);

float ContactCapsuleSphere(
	D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
	void* shapeA, const D3DXMATRIX& transformA,
	void* shapeB, const D3DXMATRIX& transformB,
	float distanceThreshold = FLT_MAX);

float ContactSphereSphere(
	D3DXVECTOR3& normal, D3DXVECTOR3& pointA, D3DXVECTOR3& pointB,
	void* shapeA, const D3DXMATRIX& transformA,
	void* shapeB, const D3DXMATRIX& transformB,
	float distanceThreshold = FLT_MAX);




#endif