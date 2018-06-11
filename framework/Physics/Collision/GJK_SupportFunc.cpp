#include"GJK_SupportFunc.h"
#include"Box.h"
#include"Capsule.h"
#include"Cylinder.h"
#include"Sphere.h"
#include"../../Utility/GameMath.h"

#define GJK_MARGIN 0.025f

//support func


//三角形
void GetSupportVertexTriangle(void* shape, const D3DXVECTOR3& seperatingAxis, D3DXVECTOR3& supportVertex)
{
	D3DXVECTOR3* v = (D3DXVECTOR3*)shape;

	float d0 = D3DXVec3Dot(&v[0], &seperatingAxis);
	float d1 = D3DXVec3Dot(&v[1], &seperatingAxis);
	float d2 = D3DXVec3Dot(&v[2], &seperatingAxis);
	int ret = 2;

	if (d0 > d1&&d0 > d2)
		ret = 0;
	else if (d1 > d2)
		ret = 1;

	supportVertex = v[ret] + GJK_MARGIN*GameMath::Normalize(seperatingAxis);
}


void GetSupportVertexTriangleWithThickness(void* shape, const D3DXVECTOR3& seperatingAxis, D3DXVECTOR3& supportVertex)
{
	D3DXVECTOR3* v = (D3DXVECTOR3*)shape;
	float d[6];
	d[0] = D3DXVec3Dot(&v[0], &seperatingAxis);
	d[1] = D3DXVec3Dot(&v[1], &seperatingAxis);
	d[2] = D3DXVec3Dot(&v[2], &seperatingAxis);
	d[3] = D3DXVec3Dot(&v[3], &seperatingAxis);
	d[4] = D3DXVec3Dot(&v[4], &seperatingAxis);
	d[5] = D3DXVec3Dot(&v[5], &seperatingAxis);

	int ret = 0;
	if (d[ret] < d[1])
		ret = 1;
	if (d[ret] < d[2])
		ret = 2;
	if (d[ret] < d[3])
		ret = 3;
	if (d[ret] < d[4])
		ret = 4;
	if (d[ret] < d[5])
		ret = 5;

	supportVertex = v[ret] + GJK_MARGIN * GameMath::Normalize(seperatingAxis);

}

//凸メッシュ
void GetSupportVertexConvex(void* shape, const D3DXVECTOR3& seperatingAxis, D3DXVECTOR3& supportVertex)
{
	/*
	用意してない
	*/

}


//ボックス
void GetSupportVertexBox(void* shape, const D3DXVECTOR3& seperatingAxis, D3DXVECTOR3& supportVertex)
{
	Box* box = (Box*)shape;
	D3DXVECTOR3 boxHalf = box->half + GameMath::GetVec3(GJK_MARGIN);

	//軸方向を調べて半径セット
	supportVertex[0] = seperatingAxis[0] > 0.0f ? boxHalf[0] : -boxHalf[0];
	supportVertex[1] = seperatingAxis[1] > 0.0f ? boxHalf[1] : -boxHalf[1];
	supportVertex[2] = seperatingAxis[2] > 0.0f ? boxHalf[2] : -boxHalf[2];

}

//カプセル
void GetSupportVertexCapsule(void* shape, const D3DXVECTOR3& seperatingAxis, D3DXVECTOR3& supportVertex)
{
	Capsule* capsule = (Capsule*)shape;
	D3DXVECTOR3 u(1.0f, 0.0f, 0.0f);

	float udotv = D3DXVec3Dot(&seperatingAxis, &u);
	D3DXVECTOR3 dir = u* (udotv > 0.0f ? capsule->halfLength : -capsule->halfLength);
	supportVertex = dir + GameMath::Normalize(seperatingAxis)*(capsule->radius + GJK_MARGIN);

}

//スフィア
void GetSupportVertexSphere(void* shape, const D3DXVECTOR3& seperatingAxis, D3DXVECTOR3& supportVertex)
{
	Sphere* sphere = (Sphere*)shape;
	supportVertex = GameMath::Normalize(seperatingAxis)*(sphere->radius + GJK_MARGIN);

}


//シリンダー
void GetSupportVertexCylinder(void* shape, const D3DXVECTOR3& seperatingAxis, D3DXVECTOR3& supportVertex)
{
	Cylinder* cylinder = (Cylinder*)shape;
	D3DXVECTOR3 u(1.0f, 0.0f, 0.0f);

	float udotv = D3DXVec3Dot(&seperatingAxis, &u);
	float s = seperatingAxis.y * seperatingAxis.y + seperatingAxis.z*seperatingAxis.z;

	if (s < 0.000001f)
	{
		supportVertex = u*(udotv > 0.0f ? cylinder->halfLength + GJK_MARGIN : -cylinder->halfLength - GJK_MARGIN);
	}
	else
	{
		D3DXVECTOR3 dir = u*(udotv > 0.0f ? cylinder->halfLength : -cylinder->halfLength);
		D3DXVECTOR3 vYZ = seperatingAxis;
		vYZ.x = 0.0f;
		vYZ /= sqrtf(s);
		supportVertex = dir + vYZ*(cylinder->radius) + GameMath::Normalize(seperatingAxis)*GJK_MARGIN;
	}

}



//center
void GetCenterTriangle(void* shape, D3DXVECTOR3& center)
{
	D3DXVECTOR3* v = (D3DXVECTOR3*)shape;
	center = (v[0] + v[1] + v[2]) / 3.0f;
}

void GetCenterTriangleWithThickness(void* shape, D3DXVECTOR3& center)
{
	D3DXVECTOR3* v = (D3DXVECTOR3*)shape;
	center = (v[0] + v[1] + v[2] + v[3] + v[4] + v[5]) / 6.0f;

}

void GetCenterConvex(void* shape, D3DXVECTOR3& center)
{
	/*
	まだ
	*/

}


void GetCenterBox(void* shape, D3DXVECTOR3& center)
{
	center.x = 0;
	center.y = 0;
	center.z = 0;
}

void GetCenterCapsule(void* shape, D3DXVECTOR3& center)
{
	center.x = 0;
	center.y = 0;
	center.z = 0;
}

void GetCenterSphere(void* shape, D3DXVECTOR3& center)
{
	center.x = 0;
	center.y = 0;
	center.z = 0;
}

void GetCenterCylinder(void* shape, D3DXVECTOR3& center)
{
	center.x = 0;
	center.y = 0;
	center.z = 0;
}
