#ifndef _Box_h_
#define _Box_h_

#include <d3dx9.h>
//çÑëÃópÇÃBOX
struct Box
{
	D3DXVECTOR3 half;
	Box(){};
	Box(const D3DXVECTOR3 _half)
	{
		Set(_half);
	}
	Box(float x, float y, float z)
	{
		Set(x, y, z);
	}
	Box(float x)
	{
		Set(x);
	}

	void Set(const D3DXVECTOR3 _half)
	{
		half = _half;
	}
	void Set(float x, float y, float z)
	{
		half.x = x;
		half.y = y;
		half.z = z;
	}
	void Set(float x)
	{
		half.x = x;
		half.y = x;
		half.z = x;
	}

};



#endif