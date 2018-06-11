#ifndef _Mass_h_
#define _Mass_h_
//質量、慣性テンソル関連のヘッダー

#include"Common.h"
#include"../Collision/Box.h"
#include"../Collision/Capsule.h"
#include"../Collision/Sphere.h"
#include"../Collision/Cylinder.h"

namespace CalcMass
{
	//質量、慣性テンソル算出

	inline float MassBox(float density, const D3DXVECTOR3& halfExtent)
	{
		return density * halfExtent.x*halfExtent.y*halfExtent.z * 8;
	}

	D3DXMATRIX InertiaBox(const D3DXVECTOR3& halfExtent, float mass);

	inline float MassSphere(float density, float radius)
	{
			//4.0f / 3.0f
		return (1.333333f) * D3DX_PI * radius * radius * radius * density;
	}

	D3DXMATRIX InertiaSphere(float radius, float mass);

	inline float MassCylinder(float density, float halfLength, float radius)
	{
		return D3DX_PI * radius * radius * 2.0f * halfLength * density;
	}

	D3DXMATRIX InertiaCylinderX(float halfLength, float radius, float mass);
	D3DXMATRIX InertiaCylinderY(float halfLength, float radius, float mass);
	D3DXMATRIX InertiaCylinderZ(float halfLength, float radius, float mass);


	//質量の移動、回転、合成

	D3DXMATRIX Translate(float mass, const D3DXMATRIX& inertia, const D3DXVECTOR3& translation);

	D3DXMATRIX Rotate(const D3DXMATRIX& inertia, const D3DXMATRIX& rotate);

};

#endif