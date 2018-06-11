#ifndef _aabb_h_
#define _aabb_h_

#include"../Base/Common.h"
#include"../Sort/SortData.h"
typedef SortData16 Aabb16;
typedef SortData32 Aabb32;

template<class C>
inline void SetXMin(C& aabb, UInt16 i)
{
	aabb.Set16(0, i);
}

template<class C>
inline void SetXMax(C& aabb, UInt16 i)
{
	aabb.Set16(1, i);
}

template<class C>
inline void SetYMin(C& aabb, UInt16 i)
{
	aabb.Set16(2, i);
}

template<class C>
inline void SetYMax(C& aabb, UInt16 i)
{
	aabb.Set16(3, i);
}

template<class C>
inline void SetZMin(C& aabb, UInt16 i)
{
	aabb.Set16(4, i);
}

template<class C>
inline void SetZMax(C& aabb, UInt16 i)
{
	aabb.Set16(5, i);
}

template<class C>
inline void SetXYZMin(C& aabb, UInt16 i, int axis)
{
	aabb.Set16(axis << 1, i);
}

template<class C>
inline void SetXYZMax(C& aabb, UInt16 i, int axis)
{
	aabb.Set16((axis << 1) + 1, i);
}

template<class C>
inline UInt16 GetXMin(const C& aabb)
{
	return aabb.Get16(0);
}

template<class C>
inline UInt16 GetXMax(const C& aabb)
{
	return aabb.Get16(1);
}

template<class C>
inline UInt16 GetYMin(const C& aabb)
{
	return aabb.Get16(2);
}

template<class C>
inline UInt16 GetYMax(const C& aabb)
{
	return aabb.Get16(3);
}

template<class C>
inline UInt16 GetZMin(const C& aabb)
{
	return aabb.Get16(4);
}

template<class C>
inline UInt16 GetZMax(const C& aabb)
{
	return aabb.Get16(5);
}

template<class C>
inline UInt16 GetXYZMin(const C& aabb, int axis)
{
	return aabb.Get16(axis << 1);
}

template<class C>
inline UInt16 GetXYZMax(const C& aabb, int axis)
{
	return aabb.Get16((axis << 1) + 1);
}

template<class A,class B>
inline bool TestAabb(const A& aabbA, const B& aabbB)
{



	if (GetXMax(aabbA) < GetXMin(aabbB) || GetXMin(aabbA) > GetXMax(aabbB))
		return false;
	if (GetYMax(aabbA) < GetYMin(aabbB) || GetYMin(aabbA) > GetYMax(aabbB))
		return false;
	if (GetZMax(aabbA) < GetZMin(aabbB) || GetZMin(aabbA) > GetZMax(aabbB))
		return false;
	return true;
}

template<class A,class B>
inline A MergeAabb(const A& aabbA, const B& aabbB)
{
	A aabb = aabbA;
	SetXMin(aabb, min(GetXMin(aabbA), GetXMin(aabbB)));
	SetXMax(aabb, max(GetXMax(aabbA), GetXMax(aabbB)));
	SetYMin(aabb, min(GetYMin(aabbA), GetYMin(aabbB)));
	SetYMax(aabb, max(GetYMax(aabbA), GetYMax(aabbB)));
	SetZMin(aabb, min(GetZMin(aabbA), GetZMin(aabbB)));
	SetZMax(aabb, max(GetZMax(aabbA), GetZMax(aabbB)));
	return aabb;
}


#endif