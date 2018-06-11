#ifndef _Capsule_h_
#define _Capsule_h_

#include<d3dx9.h>

//���̗p�̃J�v�Z��
struct Capsule
{
	float halfLength;
	float radius;

	Capsule(){};
	Capsule(float halfLen, float rad)
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