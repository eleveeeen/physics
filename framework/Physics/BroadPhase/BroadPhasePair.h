#ifndef _BroadPhasePair_h_
#define _BroadPhasePair_h_

#include"../Sort/SortData.h"
typedef SortData16 BroadphasePair;

inline void SetObjectIDA(BroadphasePair& pair, unsigned short i)
{
	pair.Set16(0, i);
}
inline void SetObjectIDB(BroadphasePair& pair, unsigned short i)
{
	pair.Set16(1, i);
}

inline void SetMotionMaskA(BroadphasePair& pair, unsigned char i)
{
	pair.Set8(4, i);
}
inline void SetMotionMaskB(BroadphasePair& pair, unsigned char i)
{
	pair.Set8(5, i);
}
inline void SetBroadphaseFlag(BroadphasePair& pair, unsigned char f)
{
	pair.Set8(6, (pair.Get8(6) & 0xf0) | (f & 0x0f));
}
inline void SetActive(BroadphasePair& pair, bool b)
{
	pair.Set8(6, (pair.Get8(6) & 0x0f) | ((b ? 1 : 0) << 4));
}
inline void SetContactID(BroadphasePair& pair, unsigned int i)
{
	pair.Set32(2, i);
}


inline unsigned short GetObjectIDA(const BroadphasePair& pair)
{
	return pair.Get16(0);
}
inline unsigned short GetObjectIDB(const BroadphasePair& pair)
{
	return pair.Get16(1);
}
inline unsigned char GetMotionMaskA(const BroadphasePair& pair)
{
	return pair.Get8(4);
}
inline unsigned char GetMotionMaskB(const BroadphasePair& pair)
{
	return pair.Get8(5);
}
inline unsigned char GetBroadphaseFlag(const BroadphasePair& pair)
{
	return pair.Get8(6) & 0x0f;
}
inline bool GetActive(const BroadphasePair& pair)
{
	return (pair.Get8(6) >> 4) != 0;
}
inline unsigned int GetContactID(const BroadphasePair &pair)
{
	return pair.Get32(2);
}



#endif