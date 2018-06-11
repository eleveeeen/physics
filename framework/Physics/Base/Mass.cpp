#include"Mass.h"
#include"../../Utility/GameMath.h"
namespace CalcMass
{


	D3DXMATRIX InertiaBox(const D3DXVECTOR3& halfExtent, float mass)
	{
		D3DXVECTOR3 sqrSz = halfExtent*2.0f;
		sqrSz = GameMath::MulPerElem(sqrSz, sqrSz);
	
		D3DXMATRIX inertia = GameMath::IdentityMatrix();

		inertia._11 = (mass*(sqrSz[1] + sqrSz[2])) / 12.0f;
		inertia._22 = (mass*(sqrSz[0] + sqrSz[2])) / 12.0f;
		inertia._33 = (mass*(sqrSz[0] + sqrSz[1])) / 12.0f;

		return inertia;
	}


	D3DXMATRIX InertiaSphere(float radius, float mass)
	{
		D3DXMATRIX inertia = GameMath::IdentityMatrix();
		inertia._11 = inertia._22 = inertia._33 = 0.4f * mass * radius * radius;
		return inertia;
	}

	static inline D3DXMATRIX InertiaCylinder(float halfLength, float radius, float mass, int axis)
	{
		D3DXMATRIX inertia = GameMath::IdentityMatrix();
		inertia._11 = inertia._22 = inertia._33 = mass / 12.0f * (3.0f * radius * radius + 4.0f * halfLength * halfLength);
		inertia.m[axis][axis] = 0.5f * mass * radius *radius;
		return inertia;

	}

	D3DXMATRIX InertiaCylinderX(float halfLength, float radius, float mass)
	{
		return InertiaCylinder(halfLength, radius, mass, 0);
	}
	D3DXMATRIX InertiaCylinderY(float halfLength, float radius, float mass)
	{
		return InertiaCylinder(halfLength, radius, mass, 1);
	}
	D3DXMATRIX InertiaCylinderZ(float halfLength, float radius, float mass)
	{
		return InertiaCylinder(halfLength, radius, mass, 2);
	}


	//éøó ÇÃà⁄ìÆÅAâÒì]ÅAçáê¨

	D3DXMATRIX Translate(float mass, const D3DXMATRIX& inertia, const D3DXVECTOR3& translation)
	{
		D3DXMATRIX m = GameMath::CrossMatrix(translation);
		return GameMath::Add3x3(inertia, mass * (m*-m));

	}

	D3DXMATRIX Rotate(const D3DXMATRIX& inertia, const D3DXMATRIX& rotate)
	{
		return GameMath::Transpose3x3(rotate) * inertia * rotate;
	}

};