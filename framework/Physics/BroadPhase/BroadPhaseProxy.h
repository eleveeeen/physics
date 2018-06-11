#ifndef _BroadPhaseProxy_h_
#define _BroadPhaseProxy_h_

#include "../Sort/SortData.h"

typedef SortData32 BroadphaseProxy;

inline void SetObjectID(BroadphaseProxy& proxy, unsigned short i)
{
	proxy.Set16(6, i);
}

inline void SetMotionMask(BroadphaseProxy& proxy, unsigned char i)
{
	proxy.Set8(14, i);
}

inline void SetProxyFlag(BroadphaseProxy& proxy, unsigned char i)
{
	proxy.Set8(15, i);
}

inline void SetSelf(BroadphaseProxy& proxy, unsigned int i)
{
	proxy.Set32(5, i);
}

inline void SetTarget(BroadphaseProxy& proxy, unsigned int i)
{
	proxy.Set32(6, i);
}



inline unsigned short GetObjectID(const BroadphaseProxy& proxy)
{
	return proxy.Get16(6);
}

inline unsigned char GetMotionMask(const BroadphaseProxy& proxy)
{
	return proxy.Get8(14);
}

inline unsigned char GetProxyFlag(const BroadphaseProxy& proxy)
{
	return proxy.Get8(15);
}

inline unsigned int GetSelf(const BroadphaseProxy& proxy)
{
	return proxy.Get32(5);
}

inline unsigned int GetTarget(const BroadphaseProxy& proxy)
{
	return proxy.Get32(6);
}




#endif