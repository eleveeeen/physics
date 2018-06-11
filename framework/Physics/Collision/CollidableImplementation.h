#ifndef _CollidableImplementation_h_
#define _CollidableImplementation_h_

inline void Collidable::Reset()
{
	shapeBase = nullptr;
	numShapes = 0;
	maxShapes = 1;
	center[0] = 0.0f;
	center[1] = 0.0f;
	center[2] = 0.0f;
	half[0] = 0.0f;
	half[1] = 0.0f;
	half[2] = 0.0f;

}

inline void Collidable::Reset(Shape* base, unsigned short* ids, int n)
{
	shapeBase = base;
	numShapes = 0;
	maxShapes = n;
	for (int i = 0; i < n; ++i)
	{
		shapeIds[i] = ids[i];
	}
	center[0] = 0.0f;
	center[1] = 0.0f;
	center[2] = 0.0f;
	half[0] = 0.0f;
	half[1] = 0.0f;
	half[2] = 0.0f;

}

inline unsigned int Collidable::GetNumShapes()const
{
	return numShapes;
}

inline unsigned int Collidable::GetMaxNumShapes()const
{
	return maxShapes;
}

inline unsigned short Collidable::GetShapeID(int i)const
{
	return shapeIds[i];
}

inline const Shape& Collidable::GetShape(int i)const
{
	if (i > 0)
		return shapeBase[shapeIds[i - 1]];
	else
		return defShape;
}

inline Shape& Collidable::GetShape(int i)
{
	if (i > 0)
		return shapeBase[shapeIds[i - 1]];
	else
		return defShape;
}

inline Shape& Collidable::GetNewShape()
{
	if (numShapes == 0)
	{
		numShapes++;
		return defShape;
	}
	else
	{
		numShapes++;
		return shapeBase[shapeIds[numShapes - 2]];
	}
}

inline void Collidable::SetCenter(const D3DXVECTOR3& _center)
{
	center[0] = _center.x;
	center[1] = _center.y;
	center[2] = _center.z;
}

inline void Collidable::SetHalf(const D3DXVECTOR3& _half)
{
	half[0] = _half.x;
	half[1] = _half.y;
	half[2] = _half.z;
}

inline D3DXVECTOR3 Collidable::GetHalf()const
{
	return D3DXVECTOR3(half[0], half[1], half[2]);
}

inline D3DXVECTOR3 Collidable::GetCenter()const
{
	return D3DXVECTOR3(center[0], center[1], center[2]);
}


#endif