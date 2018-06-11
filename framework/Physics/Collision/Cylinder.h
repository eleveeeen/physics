#ifndef _Cylinder_h_
#define _Cylinder_h_

#include<d3dx9.h>

//剛体用のシリンダー
struct Cylinder
{
	float halfLength;
	float radius;

	Cylinder(){};
	Cylinder(float halfLen, float rad)
	{
		halfLength = halfLen;
		radius = rad;
	}

	void Set(float halfLen, float rad)
	{
		halfLength = halfLen;
		radius = rad;
	}
};

#endif